/* 
 * PROJECT: NyWin32Capture
 * --------------------------------------------------------------------------------
 * The MIT License
 * Copyright (c) 2010 R.Iizuka NyARToolkit project
 * airmail(at)ebony.plala.or.jp
 * http://nyatla.jp/nyartoolkit/
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */
#pragma once
#include <exception>
#include <vector>
#include <windows.h>
#include <strmif.h>
#include <amvideo.h>
#include <dshow.h>
#include <uuids.h>


namespace NyWin32Capture
{
	/**	���̃N���X�́ANyWin32CaptureException�̗�O���`���܂��B
	*/
	class NyWin32CaptureException :public std::exception
	{
	public:
		NyWin32CaptureException();
		NyWin32CaptureException(exception e);
		NyWin32CaptureException(const char* m);
	};
}

namespace NyWin32Capture
{
	/**	���̃N���X�́A1�̃r�f�I�t�H�[�}�b�g���i�[���A�e�l�ւ̃A�N�Z�X���񋟂��܂��B
	*/
	class VideoFormat
	{
	private:
		//�R�s�[�R���X�g���N�^�폜
		VideoFormat(const VideoFormat& );
		VideoFormat& operator=(const VideoFormat&);
	private:
		AM_MEDIA_TYPE *_pmt;
		VIDEO_STREAM_CONFIG_CAPS* _scc;
	public:
		VideoFormat(AM_MEDIA_TYPE* i_pmt,const VIDEO_STREAM_CONFIG_CAPS& i_scc);
		virtual ~VideoFormat();
		int getWidth()const;
		int getHeight()const;
		double getRate()const;
		const GUID& getMediaSubType()const;
		const AM_MEDIA_TYPE* VideoFormat::getMediaType()const;
		const VIDEOINFOHEADER* getVideoInfoHeader()const;
		/**	�t�H�[�}�b�g��BITMAPINFOHEADER��Ԃ��܂��B
		*/
		const BITMAPINFOHEADER* getBitmapInfoHeader()const;
	public:
		/**	�w�肵���p�����[�^�ŁABITMAPINFOHEADER�\���̂����������܂��B
		*/
		static void initBITMAPINFOHEADER(int i_width,int i_height,const GUID& i_media_subtype,BITMAPINFOHEADER& o_bmih);
	};
}

namespace NyWin32Capture
{
	/**	���̃N���X�́AVideofirmat�̃��X�g���`���܂��B
	*/
	class VideoFormatList
	{
	private:
		//�R�s�[�R���X�g���N�^�폜
		VideoFormatList(const VideoFormatList& );
		VideoFormatList& operator=(const VideoFormatList&);
	private:
		std::vector<VideoFormat*> *_list;
		void clear();
	public:
		VideoFormatList();
		void update(IAMStreamConfig* i_config);
		virtual ~VideoFormatList();
		/**	���̊֐��́A�w�肵���t�H�[�}�b�g(���A�����A���f�B�A�T�u�^�C�v)�ɍ��v����VideoFormat�����X�g���猟�����܂��B
			����������΁A���̃t�H�[�}�b�g�ւ̃|�C���^��Ԃ��܂��B
			������Ȃ���΁ANULL��Ԃ��܂��B
		*/
		const VideoFormat* getFormat(int i_width,int i_height,const GUID& i_media_type)const;
		/**	���̊֐��́A�w�肵���t�H�[�}�b�g(���A����)�ɍ��v����VideoFormat�����X�g���猟�����܂��B
			����������΁A���̃t�H�[�}�b�g�ւ̃|�C���^��Ԃ��܂��B
			������Ȃ���΁ANULL��Ԃ��܂��B
		*/
		const VideoFormat* getFormat(int i_width,int i_height)const;
		/**	���̊֐��́Ai_index�Ԗڂ�VideoFormat��Ԃ��܂��B
		*/
		const VideoFormat* getFormat(int i_index)const;
		/**	���X�g�����L����VideoFormat�̐���Ԃ��܂��B
		*/
		int getNumberOfFormat()const;
	};
}


struct ISampleGrabber;


namespace NyWin32Capture
{

	//class IMediaControl;

	class CaptureImageCallback;
	class CaptureDevice;
	/**	startCaptureCallback���Ăяo���R�[���o�b�N�֐��ł��B
		���̊֐��̒�����Start,Stop���̊֐����Ăяo���Ȃ��ł��������B
	*/
	typedef void (*OnCaptureImage)(const CaptureDevice* i_sender,BYTE *pBuffer, long BufferLen);



	/**	���̃N���X�́A�L���v�`���f�o�C�X�P�𐧌䂵�܂��B
		���̃N���X�͂R�̏��ST_RUN,ST_IDLE,ST_CLOSED�������܂��B
		�X�e�[�^�X�ƁA�����ω�������֐����̊֌W�͈ȉ��̒ʂ�ł��B

		
		|function name      | status transition
		|-------------------+---------------------
		|openDevice         | ST_CLOSED -> ST_IDLE
		|(setting functions)| -
		|startDevice        | ST_IDLE   -> ST_RUN
		|(capture funstion )| -
		|stopDevice         | ST_RUN    -> ST_IDLE
		|closeDevice        | ST_IDLE   -> ST_CLOSED

	*/
	class CaptureDevice
	{
	private:
		const static int ST_CLOSED=0;
		const static int ST_IDLE  =1;
		const static int ST_RUN=2;
	private:
		IMoniker*    _moniker;
		AM_MEDIA_TYPE _capture_mediatype;
		struct{
			struct{
				IBaseFilter* filter;
				IAMStreamConfig* config;
				IPin* pin;
			}sorce;
			struct{
				IMediaControl* mc;
				IGraphBuilder* graph;
			}graph_builder;
			struct{
				IBaseFilter* filter;
				ISampleGrabber* grab;
			}render;
		}ds_res;
		struct{
			WCHAR* name;
		}_allocated_res;
		int _status;
		CaptureImageCallback* _image_cb;
		void* _user_value;
		void mStartCapture(BOOL i_set_bffer_samples);
	public:
		CaptureDevice(IMoniker* i_moniker);
		virtual ~CaptureDevice();
		/**	�������[�h�ŃL���v�`���������X�^�[�g���܂��B
			���̊֐��ŊJ�n�����L���v�`���́AcaptureImage�֐��ŃC���[�W���擾���邱�Ƃ��ł��܂��B
		*/
		void startCapture();
		/**	�񓯊����[�h�ŃL���v�`���������X�^�[�g���܂��B
			���̊֐��ŊJ�n�����L���v�`���́A�C���[�W��i_callback�֐��ɔ񓯊��Œʒm���܂��B
			captureImage�֐����g�����Ƃ͂ł��܂���B
		*/
		void startCaptureCallback(OnCaptureImage i_callback);
		/**	�L���v�`�����~�����܂��B
		*/
		void stopCapture();
		/**	�L���v�`���f�o�C�X���I�[�v�����܂��B
		*/
		void openDevice();
		/**	�L���v�`���f�o�C�X���N���[�Y���܂��B
		*/
		void closeDevice();
		/** �f�o�C�X�̒񋟂ł���r�f�I�t�H�[�}�b�g�̈ꗗ���Ao_list�����ɕԂ��܂��B
			���̊֐��́AST_IDLE,ST_RUN�X�e�[�^�X�̂Ƃ������g�p�\�ł��B
		*/
		void getVideoFormatList(VideoFormatList& o_list)const;
		/** �L���v�`���C���[�W���擾���܂��B
			���̊֐��́AstartCapture���炵�΂炭�̊Ԏ��s���邱�Ƃ�����܂��B
			���̊֐��́AstartCapture�ŊJ�n���A����ST_RUN�X�e�[�^�X�̂Ƃ������g�p�\�ł��B
		*/
		bool captureImage(void* i_buf,long i_buf_size=0);
		/** �L���v�`���C���[�W�̃t�H�[�}�b�g���w�肵�܂��B
			���̊֐��́AST_IDLE�X�e�[�^�X�̂Ƃ������g�p�\�ł��B
		*/
		bool setVideoFormat(int i_width,int i_height,const GUID& i_media_subtype,double i_rate);
		/** �L���v�`���C���[�W�̃t�H�[�}�b�g���w�肵�܂��B
			���̊֐��́AST_IDLE�X�e�[�^�X�̂Ƃ������g�p�\�ł��B
		*/
		bool setVideoFormat(const VideoFormat& i_format,double i_rate);
		/** i_format��media_subtype�t�B�[���h���������ăL���v�`���C���[�W�̃t�H�[�}�b�g���w�肵�܂��B
			���̊֐��́AST_IDLE�X�e�[�^�X�̂Ƃ������g�p�\�ł��B
		*/
		bool setVideoFormat(const VideoFormat& i_format,const GUID& i_media_subtype,double i_rate);

		/**	�L���v�`���摜���f�B�A�^�C�v���擾���܂��B
		*/
		const AM_MEDIA_TYPE& getMediaType()const;
		/**	���[�U��`�l���擾���܂��B
		*/
		void* getUserValue()const;
		/**	���[�U��`�l��ݒ肵�܂��B���̒l�́AgetUserValue�Ŏ擾�ł��܂��B
			�񓯊��֐��ɔC�ӂ̒l��n�������Ƃ��Ɏg�p���܂��B
		*/
		void setUserValue(void* i_user_value);
		/**	���̃f�o�C�X�̒l���擾���܂��B
		*/
		const WCHAR* getName()const;
	};
}

namespace NyWin32Capture
{
	/**	���̃N���X�́A�L���v�`���f�o�C�X�̃��X�g���`���A�L���v�`���f�o�C�X�����L���܂��B
	*/
	class CaptureDeviceList
	{
	private:
		void createDeviceList();
		void releaseDeviceList();
		std::vector<CaptureDevice*> *_list;
	public:
		CaptureDeviceList();
		virtual ~CaptureDeviceList();
		//	i_index�Ԗڂ̃L���v�`���f�o�C�X���擾���܂��B
		CaptureDevice* getDevice(int i_index)const;
		//	�f�o�C�X�̐����擾���܂��B
		int getNumberOfDevice()const;
	};
}