#include "NyAR_core.h"

#define CODE_FILE "../../data/patt.hiro"
#define DATA_FILE "../../data/320x240ABGR.raw"
#define CAMERA_FILE "../../data/camera_para.dat"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "SingleNyIdMarkerProcessor.h"
#include "NyIdMarkerDataEncoder_RawBit.h"
#include "NyARRgbRaster_RGB.h"
using namespace NyARToolkitCPP;
using namespace std;

unsigned char* load_file_image(const char* i_filename)
{
	ifstream inf;
	unsigned char* result=NULL;
	try {
		inf.open(i_filename,ios::binary+ios::in);
		int size=320*240*3;
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


class MarkerProcessor : public SingleNyIdMarkerProcesser
{
public:
	const NyARTransMatResult* transmat;
	int current_id;
private:
	NyIdMarkerDataEncoder_RawBit _encoder;
public:
	MarkerProcessor(const NyARParam *i_cparam, int i_raster_format):SingleNyIdMarkerProcesser()
	{
		this->transmat=NULL;
		this->current_id=-1;
		initInstance(i_cparam,&this->_encoder,10.0,i_raster_format);
		//アプリケーションフレームワークの初期化
		return;
	}
	/**
	* アプリケーションフレームワークのハンドラ（マーカ出現）
	*/
protected:
	void onEnterHandler(const INyIdMarkerData &i_code)
	{
		const NyIdMarkerData_RawBit &code = (const NyIdMarkerData_RawBit&)i_code;
		if (code.length > 4)
		{
			//4バイト以上の時はint変換しない。
			this->current_id = -1;//undefined_id
		}
		else
		{
			this->current_id = 0;
			//最大4バイト繋げて１個のint値に変換
			for (int i = 0; i < code.length; i++)
			{
				this->current_id = (this->current_id << 8) | code.packet[i];
			}
		}
		this->transmat = NULL;
	}
	/**
	* アプリケーションフレームワークのハンドラ（マーカ消滅）
	*/
protected:
	void onLeaveHandler()
	{
		this->current_id = -1;
		this->transmat = NULL;
		return;
	}
	/**
	* アプリケーションフレームワークのハンドラ（マーカ更新）
	*/
protected:
	void onUpdateHandler(const NyARSquare &i_square, const NyARTransMatResult &result)
	{
		this->transmat = &result;
	}
};


const char* data_file = "../../data/320x240NyId.raw";
const char* camera_file = "../../data/camera_para.dat";
void main()
{
	//AR用カメラパラメタファイルをロード
	{
		NyARParam ap;
		ap.loadARParamFromFile(camera_file);
		ap.changeScreenSize(320, 240);


		// 試験イメージの読み出し(320x240 BGRAのRAWデータ)
		unsigned char* img=load_file_image(data_file);

		NyARRgbRaster_BGRA ra(320, 240,false);
		ra.wrapBuffer(img);
		MarkerProcessor pr(&ap, ra.getBufferType());
		pr.detectMarker(ra);
		delete img;
	}
	_CrtDumpMemoryLeaks();
	return;
}

