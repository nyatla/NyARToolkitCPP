/* 
* PROJECT: NyARToolkitCPP
* --------------------------------------------------------------------------------
*
* The NyARToolkitCPP is C++ version NyARToolkit class library.
* Copyright (C)2008-2009 Ryo Iizuka
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* 
* For further information please contact.
*	http://nyatla.jp/nyatoolkit/
*	<airmail(at)ebony.plala.or.jp> or <nyatla(at)nyatla.jp>
* 
*/
#include "NyARGLUtil.h"

namespace NyARUtils
{
	NyARGLUtil::NyARGLUtil()
	{
		this->view_scale_factor=0.025;
		this->view_distance_min=0.1;//#define VIEW_DISTANCE_MIN		0.1			// Objects closer to the camera than this will not be displayed.
		this->view_distance_max=100.0;//#define VIEW_DISTANCE_MAX		100.0		// Objects further away from the camera than this will not be displayed.

	}
	/**
	 * ラスタのGLタイプを取得する。
	 * @param i_buffer_type
	 * @return
	 * @throws NyARException
	 */
	int NyARGLUtil::getGlPixelFormat(int i_buffer_type)
	{
		switch(i_buffer_type){
		case NyARToolkitCPP::NyARBufferType::BYTE1D_B8G8R8_24:
			return GL_BGR_EXT;
		case NyARToolkitCPP::NyARBufferType::BYTE1D_R8G8B8_24:
			return GL_RGB;
		case NyARToolkitCPP::NyARBufferType::BYTE1D_B8G8R8X8_32:
			return GL_BGRA_EXT;

		default:
			throw new NyARToolkitCPP::NyARException();
		}
	}
	

	/**
	 * GLNyARRaster_RGBをバックグラウンドに書き出す。
	 * @param image
	 * @param zoom
	 */
	void NyARGLUtil::drawBackGround(const NyARToolkitCPP::INyARRgbRaster& i_raster, double i_zoom)
	{
		GLboolean lightingSave;
		GLboolean depthTestSave;
		GLint texEnvModeSave;
		const NyARToolkitCPP::TNyARIntSize& rsize=i_raster.getSize();

		// Prepare an orthographic projection, set camera position for 2D drawing, and save GL state.
		glGetTexEnviv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,&texEnvModeSave); // Save GL texture environment mode.
		if (texEnvModeSave != GL_REPLACE) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		lightingSave = glIsEnabled(GL_LIGHTING); // Save enabled state of lighting.
		if (lightingSave == GL_TRUE) {
			glDisable(GL_LIGHTING);
		}
		depthTestSave = glIsEnabled(GL_DEPTH_TEST); // Save enabled state of depth test.
		if (depthTestSave == GL_TRUE) {
			glDisable(GL_DEPTH_TEST);
		}
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0,rsize.w, 0.0,rsize.h);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		arglDispImageStateful(i_raster, i_zoom);

		// Restore previous projection, camera position, and GL state.
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		if (depthTestSave) {
			glEnable(GL_DEPTH_TEST); // Restore enabled state of depth test.
		}
		if (lightingSave) {
			glEnable(GL_LIGHTING); // Restore enabled state of lighting.
		}
		if (texEnvModeSave != GL_REPLACE) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,texEnvModeSave); // Restore GL texture environment mode.
		}
		glEnd();
	}

	/**
	 * arglDispImageStateful関数モドキ
	 * @param image
	 * @param zoom
	 */
	void NyARGLUtil::arglDispImageStateful(const NyARToolkitCPP::INyARRgbRaster& i_raster, double zoom)
	{
		const NyARToolkitCPP::TNyARIntSize& rsize = i_raster.getSize();
		float zoomf;
		GLint params[4];
		zoomf = (float) zoom;
		glDisable(GL_TEXTURE_2D);
		glGetIntegerv(GL_VIEWPORT, params);
		glPixelZoom(zoomf * ((float) (params[2]) / (float) rsize.w), -zoomf * ((float) (params[3]) / (float) rsize.h));
		glRasterPos2f(0.0f, (float) rsize.h);
		//glWindowPos2f(0.0f, (float) rsize.h);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glDrawPixels(rsize.w,rsize.h,getGlPixelFormat(i_raster.getBufferType()), GL_UNSIGNED_BYTE, (GLvoid*)i_raster.getBuffer());
	}
	

	void NyARGLUtil::setScaleFactor(double i_new_value)
	{
		this->view_scale_factor = i_new_value;
	}

	void NyARGLUtil::setViewDistanceMin(double i_new_value)
	{
		this->view_distance_min = i_new_value;
	}

	void NyARGLUtil::setViewDistanceMax(double i_new_value)
	{
		this->view_distance_max = i_new_value;
	}

	/**
	 * void arglCameraFrustumRH(const ARParam *cparam, const double focalmin, const double focalmax, GLdouble m_projection[16])
	 * 関数の置き換え
	 * NyARParamからOpenGLのProjectionを作成します。
	 * @param i_arparam
	 * @param o_gl_projection
	 * double[16]を指定して下さい。
	 */
	void NyARGLUtil::toCameraFrustumRH(const NyARToolkitCPP::NyARParam& i_arparam,GLdouble* o_gl_projection)
	{
		NyARToolkitCPP::NyARMat trans_mat(3, 4);
		NyARToolkitCPP::NyARMat icpara_mat(3, 4);
		double p[3][3], q[4][4];
		int i, j;

		const NyARToolkitCPP::TNyARIntSize& size=i_arparam.getScreenSize();
		const int width = size.w;
		const int height = size.h;
		
		i_arparam.getPerspectiveProjectionMatrix().decompMat(icpara_mat, trans_mat);

		double* icpara = icpara_mat.getArray();
		double* trans = trans_mat.getArray();
		for (i = 0; i < 4; i++) {
			icpara[1*4+i] = (height - 1) * (icpara[2*4+i]) - icpara[1*4+i];
		}

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				p[i][j] = icpara[i*4+j] / icpara[2*4+2];
			}
		}
		q[0][0] = (2.0 * p[0][0] / (width - 1));
		q[0][1] = (2.0 * p[0][1] / (width - 1));
		q[0][2] = -((2.0 * p[0][2] / (width - 1)) - 1.0);
		q[0][3] = 0.0;

		q[1][0] = 0.0;
		q[1][1] = -(2.0 * p[1][1] / (height - 1));
		q[1][2] = -((2.0 * p[1][2] / (height - 1)) - 1.0);
		q[1][3] = 0.0;

		q[2][0] = 0.0;
		q[2][1] = 0.0;
		q[2][2] = (view_distance_max + view_distance_min) / (view_distance_min - view_distance_max);
		q[2][3] = 2.0 * view_distance_max * view_distance_min / (view_distance_min - view_distance_max);

		q[3][0] = 0.0;
		q[3][1] = 0.0;
		q[3][2] = -1.0;
		q[3][3] = 0.0;

		for (i = 0; i < 4; i++) { // Row.
			// First 3 columns of the current row.
			for (j = 0; j < 3; j++) { // Column.
				o_gl_projection[i + j * 4] = q[i][0] * trans[0*4+j] + q[i][1] * trans[1*4+j] + q[i][2] * trans[2*4+j];
			}
			// Fourth column of the current row.
			o_gl_projection[i + 3 * 4] = q[i][0] * trans[0*4+3] + q[i][1] * trans[1*4+3] + q[i][2] * trans[2*4+3] + q[i][3];
		}
		return;
	}
	
	
	
	/**
	 * NyARTransMatResultをOpenGLの行列へ変換します。
	 * @param i_ny_result
	 * @param o_gl_result
	 * @throws NyARException
	 */
	void NyARGLUtil::toCameraViewRH(const NyARToolkitCPP::NyARTransMatResult& i_ny_result, GLdouble* o_gl_result)
	{
		o_gl_result[0 + 0 * 4] = i_ny_result.m00; 
		o_gl_result[0 + 1 * 4] = i_ny_result.m01;
		o_gl_result[0 + 2 * 4] = i_ny_result.m02;
		o_gl_result[0 + 3 * 4] = i_ny_result.m03;
		o_gl_result[1 + 0 * 4] = -i_ny_result.m10;
		o_gl_result[1 + 1 * 4] = -i_ny_result.m11;
		o_gl_result[1 + 2 * 4] = -i_ny_result.m12;
		o_gl_result[1 + 3 * 4] = -i_ny_result.m13;
		o_gl_result[2 + 0 * 4] = -i_ny_result.m20;
		o_gl_result[2 + 1 * 4] = -i_ny_result.m21;
		o_gl_result[2 + 2 * 4] = -i_ny_result.m22;
		o_gl_result[2 + 3 * 4] = -i_ny_result.m23;
		o_gl_result[3 + 0 * 4] = 0.0;
		o_gl_result[3 + 1 * 4] = 0.0;
		o_gl_result[3 + 2 * 4] = 0.0;
		o_gl_result[3 + 3 * 4] = 1.0;
		if (view_scale_factor != 0.0) {
			o_gl_result[12] *= view_scale_factor;
			o_gl_result[13] *= view_scale_factor;
			o_gl_result[14] *= view_scale_factor;
		}
		return;
	}	
	
};