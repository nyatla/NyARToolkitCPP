#include <windows.h>
#include <stdio.h>
#include <stdlib.h>	
#include "NyAR_core.h"
#include "NyARGLUtil.h"
#include "NyWin32Capture.h"

using namespace NyARToolkitCPP;
using namespace NyWin32Capture;
using namespace NyARUtils;



namespace MyApplication
{
	static void drawCube(void)
	{
		// Colour cube data.
		static GLuint polyList = 0;
		float fSize = 0.5f;
		long f, i;	
		const GLfloat cube_vertices [8][3] = {
		{1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
		{1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };
		const GLfloat cube_vertex_colors [8][3] = {
		{1.0, 1.0, 1.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
		{1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0} };
		GLint cube_num_faces = 6;
		const short cube_faces [6][4] = {
		{3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };
		
		if (!polyList) {
			polyList = glGenLists (1);
			glNewList(polyList, GL_COMPILE);
			glBegin (GL_QUADS);
			for (f = 0; f < cube_num_faces; f++)
				for (i = 0; i < 4; i++) {
					glColor3f (cube_vertex_colors[cube_faces[f][i]][0], cube_vertex_colors[cube_faces[f][i]][1], cube_vertex_colors[cube_faces[f][i]][2]);
					glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
				}
			glEnd ();
			glColor3f (0.0, 0.0, 0.0);
			for (f = 0; f < cube_num_faces; f++) {
				glBegin (GL_LINE_LOOP);
				for (i = 0; i < 4; i++)
					glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
				glEnd ();
			}
			glEndList ();
		}
		
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.5);
		glDisable(GL_LIGHTING);
		glCallList(polyList);
		glPopMatrix();

	}


	/************************************************************
		OpenGL callback functions
	*************************************************************/
	
	static HANDLE app_mux;
	static NyARSingleDetectMarker* app_ar;
	static NyARGLUtil* app_nyar_gl;
	static NyARParam* app_cparam;
	static NyARCode* app_code;
	static NyARRgbRaster_BGRA* app_raster;
	//
	static GLdouble app_gl_camera_frustum_rh[16];
	static GLdouble app_gl_camera_view_rh[16];

	bool app_is_marker_exist;


	//OpenGL callback

	static void Quit(void)
	{
		exit(0);
	}
	static void Keyboard(unsigned char key, int x, int y)
	{
		switch (key) {
			case 0x1B:						// Quit.
			case 'Q':
			case 'q':
				Quit();
				break;
			default:
				break;
		}
	}
	static void Idle(void)
	{
	}
	static void Reshape(int w, int h)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, (GLsizei) w, (GLsizei) h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	static void Display(void)
	{

		// Select correct buffer for this context.
		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		WaitForSingleObject(app_mux,INFINITE);
		
		//draw background
		app_nyar_gl->drawBackGround(*app_raster,1.0);

		if(app_is_marker_exist){
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixd(app_gl_camera_frustum_rh);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixd(app_gl_camera_view_rh);
			drawCube();
		}
		ReleaseMutex(app_mux);
		glutSwapBuffers();
	}
	
	/************************************************************
		Camera callback functions
	*************************************************************/

	static void onCaptureImage(
		const CaptureDevice* i_sender,BYTE *pBuffer, long BufferLen)
	{
		WaitForSingleObject(app_mux,INFINITE);
		
		const TNyARIntSize& s=app_raster->getSize();
		//flip virtical
		int stride=s.w*4;
		BYTE* inptr =pBuffer+(s.h-1)*stride;
		BYTE* outptr=(BYTE*)app_raster->getBuffer();
		for(int i=s.h-1;i>=0;i--){
			memcpy(outptr,inptr,stride);
			inptr -=stride;
			outptr+=stride;
		}
		//detection
		app_is_marker_exist=app_ar->detectMarkerLite(*app_raster,100);
		if(app_is_marker_exist){
			if(app_ar->getConfidence()<0.50){
				app_is_marker_exist=false;
			}else{
				NyARTransMatResult mat;
				app_ar->getTransmationMatrix(mat);
				app_nyar_gl->toCameraViewRH(mat,app_gl_camera_view_rh);
			}
		}
		ReleaseMutex(app_mux);
		glutPostRedisplay();
	}


	static int gl_main(
		CaptureDevice& i_camera,
		int i_width,int i_height,
		const char* i_camera_file,const char* i_patt_file)
	{
		//System resource
		app_is_marker_exist=false;
		app_mux=CreateMutex(NULL,FALSE,NULL);

		//NyAR initialize

		app_cparam=new NyARParam();
		app_code=new NyARCode(16,16);

		app_cparam->loadARParamFromFile(i_camera_file);
		app_cparam->changeScreenSize(i_width,i_height);
		
		app_code->loadARPattFromFile(i_patt_file);

		app_raster=new NyARRgbRaster_BGRA(i_width,i_height,true);

		app_ar=new NyARSingleDetectMarker(app_cparam,app_code,80.0,app_raster->getBufferType());


		app_nyar_gl=new NyARGLUtil();

		//prepare parametors
		app_nyar_gl->toCameraFrustumRH(
			*app_cparam,
			app_gl_camera_frustum_rh);


		//Camera setup
		i_camera.openDevice();
		i_camera.setVideoFormat(i_width,i_height,MEDIASUBTYPE_RGB32,30.0);

		//OpenGL initialize
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(i_width, i_height);
		glutCreateWindow("SimpleLite - NyARToolkitCPP+OpenGL");
		glEnable(GL_DEPTH_TEST);
		glutDisplayFunc(Display);
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(Keyboard);

		//start camera
		i_camera.startCaptureCallback(onCaptureImage);

		glutMainLoop();

		i_camera.stopCapture();
		i_camera.closeDevice();

		delete app_nyar_gl;
		delete app_ar;
		delete app_cparam;
		delete app_code;
		delete app_raster;

		DeleteObject(app_mux);
		return (0);
	}
};




/*
	This program requires glut32.dll.
	Please check the extlib/glut/readme.txt to setup compile environment.
	--
	glut32.dllをSimpleLiteのプロジェクトのあるフォルダにコピーしてください。
	コンパイルにはglut.h等も要るので、extlib/glutフォルダにあるreadme.txtを読んで
	インストールしてね。

*/
int main(int argc, char** argv)
{
	CoInitialize(NULL);
	{
		//カメラの取得
		CaptureDeviceList devlist;

		if(devlist.getNumberOfDevice()<1){
			perror("This system has not Capture device.");
		}
		//OpenGLのinit
		glutInit(&argc,argv);


		MyApplication::gl_main(
			*(devlist.getDevice(0)),
			640,480,
			"../../Data/camera_para.dat","../../Data/patt.hiro");
	}
	CoUninitialize();
	_CrtDumpMemoryLeaks();	
	return (0);
}

