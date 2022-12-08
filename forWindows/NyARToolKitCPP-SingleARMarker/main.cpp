#include "NyAR_core.h"
#include "SingleARMarkerProcessor.h"


#define CODE_FILE "../../data/patt.hiro"
#define DATA_FILE "../../data/320x240ABGR.raw"
#define CAMERA_FILE "../../data/camera_para.dat"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace NyARToolkitCPP;
using namespace std;

unsigned char* load_file_image(const char* i_filename)
{
	ifstream inf;
	unsigned char* result=NULL;
	try {
		inf.open(i_filename,ios::binary+ios::in);
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


class MarkerProcessor :public SingleARMarkerProcesser
	{	
	public:
		const NyARTransMatResult* transmat;
		int current_code;

	public:
		MarkerProcessor(const NyARParam* i_cparam,int i_raster_format)
		{
			//アプリケーションフレームワークの初期化
			initInstance(i_cparam,i_raster_format);
			current_code=-1;
			return;
		}
	protected:
		void onEnterHandler(int i_code)
		{
			current_code=i_code;
		}
		void onLeaveHandler()
		{
			current_code=-1;
			this->transmat=NULL;
			return;			
		}
		void onUpdateHandler(const NyARSquare& i_square, const NyARTransMatResult& result)
		{
			this->transmat=&result;
		}
	};






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


	NyARCode code(16, 16);
	code.loadARPattFromFile(CODE_FILE);
	buf=load_file_image(DATA_FILE);
	ra = new NyARRgbRaster_BGRA(320, 240,false);
	ra->wrapBuffer(buf);
	//ラップするバッファをセットする。

    MarkerProcessor* pr = new MarkerProcessor(&ap, ra->getBufferType());
    NyARCode* codes[1];
    codes[0]=new NyARCode(16,16);
    codes[0]->loadARPattFromFile(CODE_FILE);
    pr->setARCodeTable(codes,1,16,80.0);
    pr->detectMarker(*ra);
	delete codes[0];
	delete ra;
	delete pr;
	delete buf;
	}

	_CrtDumpMemoryLeaks();
	return 0;
}