#include "NyAR_core.h"

#define CODE_FILE "../../data/patt.hiro"
#define DATA_FILE "../../data/320x240ABGR.raw"
#define CAMERA_FILE "../../data/camera_para.dat"
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace NyARToolkitCPP;
using namespace std;

unsigned char* load_file_image(const char* i_filename)
{
	ifstream inf;
	unsigned char* result=NULL;
	try {
		inf.open(i_filename,ios::binary|ios::in);
		int size=320*240*4;
		result=new unsigned char[size];
		inf.read((char*)result,size);
		if(size!=inf.gcount()){
			throw exception();
		}
		inf.close();
	} catch(exception e){
		if(result!=NULL){
			delete result;
		}
		throw;
	}
	return result;
}

int main(int argc,char* argv[])
{
	{
	// AR用カメラパラメタファイルをロード
	NyARParam ap;
	//CPUのエンディアンを指定して下さい。
	ap.setEndian(TNyAREndian_LITTLE);
	ap.loadARParamFromFile(CAMERA_FILE);
	ap.changeScreenSize(320, 240);

	// AR用のパターンコードを読み出し
	unsigned char* buf;
	NyARRgbRaster_BGRA* ra;
	NyARSingleDetectMarker* ar;


	NyARCode code(16, 16);
	code.loadARPattFromFile(CODE_FILE);
	buf=load_file_image(DATA_FILE);
	ra = new NyARRgbRaster_BGRA(320, 240,false);
	ra->wrapBuffer(buf);
	// １パターンのみを追跡するクラスを作成
	ar = new NyARSingleDetectMarker(&ap,&code, 80.0,ra->getBufferType());
	ar->setContinueMode(false);

	NyARTransMatResult result_mat;

	ar->detectMarkerLite(*ra, 100);
	ar->getTransmationMatrix(result_mat);
	printf("%s",
		"performance test of NyARToolkit.\n"
		"This test measures processing time of marker detection and transform matrix calculation 1000 times.\n");
	printf("Marker confidence\n cf=%f\n",ar->getConfidence());
	printf("Transform Matrix\n");
	printf(
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n",
		result_mat.m00,result_mat.m01,result_mat.m02,result_mat.m03,
		result_mat.m10,result_mat.m11,result_mat.m12,result_mat.m13,
		result_mat.m20,result_mat.m21,result_mat.m22,result_mat.m23);
		
	//DWORD st=GetTickCount();

	// マーカーを検出1000回分の処理時間を計測
	for (int i = 0; i < 1000; i++) {
		// 変換行列を取得
		ar->detectMarkerLite(*ra, 100);
		ar->getTransmationMatrix(result_mat);
	}
	//printf("done.\ntotal=%u[ms]\n",GetTickCount()-st);
	printf("done.\ntotal=--[ms]\n");
	delete ra;
	delete ar;
	delete buf;
	}
	return 0;
}
