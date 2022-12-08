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
		//�A�v���P�[�V�����t���[�����[�N�̏�����
		return;
	}
	/**
	* �A�v���P�[�V�����t���[�����[�N�̃n���h���i�}�[�J�o���j
	*/
protected:
	void onEnterHandler(const INyIdMarkerData &i_code)
	{
		const NyIdMarkerData_RawBit &code = (const NyIdMarkerData_RawBit&)i_code;
		if (code.length > 4)
		{
			//4�o�C�g�ȏ�̎���int�ϊ����Ȃ��B
			this->current_id = -1;//undefined_id
		}
		else
		{
			this->current_id = 0;
			//�ő�4�o�C�g�q���ĂP��int�l�ɕϊ�
			for (int i = 0; i < code.length; i++)
			{
				this->current_id = (this->current_id << 8) | code.packet[i];
			}
		}
		this->transmat = NULL;
	}
	/**
	* �A�v���P�[�V�����t���[�����[�N�̃n���h���i�}�[�J���Łj
	*/
protected:
	void onLeaveHandler()
	{
		this->current_id = -1;
		this->transmat = NULL;
		return;
	}
	/**
	* �A�v���P�[�V�����t���[�����[�N�̃n���h���i�}�[�J�X�V�j
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
	//AR�p�J�����p�����^�t�@�C�������[�h
	{
		NyARParam ap;
		ap.loadARParamFromFile(camera_file);
		ap.changeScreenSize(320, 240);


		// �����C���[�W�̓ǂݏo��(320x240 BGRA��RAW�f�[�^)
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

