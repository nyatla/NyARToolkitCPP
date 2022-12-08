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


/**
How to Compile

This project requires Windows PSDK 7.1
Download from Microsoft website.
http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=35aeda01-421d-4ba5-b44b-543dc8c33a20

�R���p�C���ɂ���
���̃R�[�h���R���p�C�����邽�߂ɂ́AWindows PSDK 7.1���K�v�ł��B
Microsoft��Web�T�C�g����_�E�����[�h���Ă��������B

http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=35aeda01-421d-4ba5-b44b-543dc8c33a20

baseclass�t�H���_�̉��́APSDK7.1��directshow/multimedia�f�B���N�g���̃t�@�C�����Q�Ƃ���
���܂��B�C���X�g�[���f�B���N�g�����Ⴄ�ꍇ�́A�ꏊ�����������Ă��������B
*/
#include "NyWin32Capture.h"
#include "qedit_def.h"	//qedit.h�������Ă�����΂邽�߂̃w�b�_
#include <stdio.h>
#include <conio.h>
#include <exception>
#include <vector>
#include <streams.h>
#include <assert.h>


using namespace std;

namespace NyWin32Capture
{
	/*	�ȈՃX�}�[�g�|�C���^
	*/
	template <class T> class AutoReleaseComPtr
	{
	private:
		AutoReleaseComPtr(const AutoReleaseComPtr& );
		AutoReleaseComPtr& operator=(const AutoReleaseComPtr&);
	public:
		T* ptr;
		AutoReleaseComPtr<T>()
		{
			this->ptr=NULL;
		}
		AutoReleaseComPtr<T>(T* i_ptr)
		{
			this->ptr=i_ptr;
		}
		virtual ~AutoReleaseComPtr<T>()
		{
			if(this->ptr!=NULL)
			{
				this->ptr->Release();
				this->ptr=NULL;
			}
		}
		T* operator->()
		{
			return this->ptr;
		}
		operator T*() const
		{
			return this->ptr;
		}
		T* detach()
		{
			T* ret;
			ret=this->ptr;
			this->ptr=NULL;
			return ret;
		}
		void release()
		{
			this->ptr->Release();
			this->ptr=NULL;
		}
	};



	/*ICaptureGraphBuilder2����IAMStreamConfig�C���^�t�F�C�X���擾����B
	*/
	static bool mGetIAMStreamConfig(IPin* i_pin,IAMStreamConfig** o_config)
	{
		HRESULT hr;
		hr=i_pin->QueryInterface(IID_IAMStreamConfig, (void **)o_config);
		if(FAILED(hr)){
			return false;
		}
		return true;
	}
	/*	Pin�̃J�e�S�����擾����B
	*/
	static void mGetPinCategory(IPin* pPin,GUID& Category)
	{
		AutoReleaseComPtr<IKsPropertySet> pKs;
		HRESULT hr = pPin->QueryInterface(IID_IKsPropertySet, (void **)&(pKs.ptr));
		if (!SUCCEEDED(hr))
		{
			throw NyWin32CaptureException();
		}
		DWORD cbReturned;
		hr = pKs->Get(AMPROPSETID_Pin, AMPROPERTY_PIN_CATEGORY, NULL, 0, &Category, sizeof(GUID), &cbReturned);
	}

	/**/
	static IPin* mFindPinByDirection(IBaseFilter *pFilter, PIN_DIRECTION PinDir,int i_index)
	{
		int index=i_index;

		AutoReleaseComPtr<IEnumPins> pEnum;
		HRESULT hr = pFilter->EnumPins(&(pEnum.ptr));
		if (FAILED(hr))
		{
			return NULL;
		}
		AutoReleaseComPtr<IPin> pPin;
		while(pEnum->Next(1, &(pPin.ptr), 0) == S_OK)
		{
			PIN_DIRECTION PinDirThis;
			if(pPin->QueryDirection(&PinDirThis)!=S_OK){
				continue;
			}
			if(PinDir != PinDirThis)
			{
				pPin.release();
				continue;
			}
			if(index>0)
			{
				index--;
				pPin.release();
				continue;
			}
			IPin* result;
			result=pPin.detach();
			return result;
		}
		return NULL;  
	}


	static IPin* mFindPinByCategory(IBaseFilter* i_filter,const GUID& Category)
	{
		HRESULT hr;
		AutoReleaseComPtr<IEnumPins> pEnum;
		if (SUCCEEDED(i_filter->EnumPins(&(pEnum.ptr))))
		{
			AutoReleaseComPtr<IPin> pPin;
			while (hr = pEnum->Next(1, &(pPin.ptr), 0), hr == S_OK)
			{
				PIN_DIRECTION ThisPinDir;
				hr = pPin->QueryDirection(&ThisPinDir);
				if (FAILED(hr))
				{
					throw NyWin32CaptureException();
				}
				GUID pin_category;
				mGetPinCategory(pPin, pin_category);

				if(Category==pin_category)
				{
					return pPin.detach();
				}
				pPin.release();
			}
		}
		// ��v����s�����Ȃ��B
		return NULL;
	}

	/*	�O���t�̎��S�Ẵt�B���^�s����ؒf����B
	*/
	static void mDisconnectAll(IGraphBuilder* i_graph)
	{
		HRESULT hr = S_OK;
		if (i_graph==NULL)
		{
			throw NyWin32CaptureException();
		}

		AutoReleaseComPtr<IEnumFilters> enum_filter;
		hr = i_graph->EnumFilters(&(enum_filter.ptr));
		if(!SUCCEEDED(hr))
		{
			throw NyWin32CaptureException();
		}
		AutoReleaseComPtr<IBaseFilter> filter;
		while(S_OK == enum_filter->Next(1, &(filter.ptr), NULL))
		{
			AutoReleaseComPtr<IEnumPins> enum_pins;
			hr = filter->EnumPins(&(enum_pins.ptr));
			if(!SUCCEEDED(hr))
			{
				continue;
			}
			AutoReleaseComPtr<IPin> pin;
			while (S_OK == enum_pins->Next(1, &(pin.ptr), NULL))
			{
				AutoReleaseComPtr<IPin> pin2;
				if (S_OK == pin->ConnectedTo(&(pin2.ptr)))
				{
					// pins are connected, to disconnect filters, both pins must be disconnected
					hr = i_graph->Disconnect(pin);
					hr = i_graph->Disconnect(pin2);
				}
				pin.release();
			}
			filter.release();
		}
		return;
	}


	class CaptureImageCallback: public CUnknown, public ISampleGrabberCB
	{
	private:
		CaptureDevice* _sender;
		OnCaptureImage _callback;
	public:
		DECLARE_IUNKNOWN;

		STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv)
		{
			if( riid == IID_ISampleGrabberCB ){
				return GetInterface((ISampleGrabberCB*)this, ppv);
			}
			return CUnknown::NonDelegatingQueryInterface(riid, ppv);
		}
		STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample)
		{
			return E_NOTIMPL;
		}

		STDMETHODIMP BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen)
		{
			this->_callback(this->_sender,pBuffer,BufferLen);
			return S_OK;
		}
		// �R���X�g���N�^
		CaptureImageCallback(CaptureDevice* i_sender,OnCaptureImage i_callback) : CUnknown("SGCB", NULL)
		{
			this->_sender=i_sender;
			this->_callback=i_callback;
		}

	};






}

//NyWin32CaptureException
//
namespace NyWin32Capture
{
	NyWin32CaptureException::NyWin32CaptureException():exception()
	{
	}
	NyWin32CaptureException::NyWin32CaptureException(exception e):exception(e)
	{
	}
	NyWin32CaptureException::NyWin32CaptureException(const char* m)
	{
		perror(m);
	}
}

//VideoFormat
//
namespace NyWin32Capture
{
	VideoFormat::VideoFormat(AM_MEDIA_TYPE* i_pmt,const VIDEO_STREAM_CONFIG_CAPS& i_scc)
	{
		this->_scc=(VIDEO_STREAM_CONFIG_CAPS*)malloc(sizeof(VIDEO_STREAM_CONFIG_CAPS));
		this->_pmt=i_pmt;
		*(this->_scc)=i_scc;
	}
	VideoFormat::~VideoFormat()
	{
		DeleteMediaType(this->_pmt);
		free(this->_scc);
	}
	int VideoFormat::getWidth()const
	{
		return reinterpret_cast<VIDEOINFOHEADER*>(this->_pmt->pbFormat)->bmiHeader.biWidth;
	}
	int VideoFormat::getHeight()const
	{
		return reinterpret_cast<VIDEOINFOHEADER*>(this->_pmt->pbFormat)->bmiHeader.biHeight;
	}
	double VideoFormat::getRate()const
	{
		return 10000000.0/((double)reinterpret_cast<VIDEOINFOHEADER*>(this->_pmt->pbFormat)->AvgTimePerFrame);
	}
	const GUID& VideoFormat::getMediaSubType()const
	{
		return this->_pmt->subtype;
	}
	const AM_MEDIA_TYPE* VideoFormat::getMediaType()const
	{
		return reinterpret_cast<AM_MEDIA_TYPE*>(this->_pmt);
	}
	const VIDEOINFOHEADER* VideoFormat::getVideoInfoHeader()const
	{
		return reinterpret_cast<VIDEOINFOHEADER*>(this->_pmt->pbFormat);
	}
	const BITMAPINFOHEADER* VideoFormat::getBitmapInfoHeader()const
	{
		return &(reinterpret_cast<VIDEOINFOHEADER*>(this->_pmt->pbFormat)->bmiHeader);
	}
	void VideoFormat::initBITMAPINFOHEADER(int i_width,int i_height,const GUID& i_media_subtype,BITMAPINFOHEADER& o_bmih)
	{
		int bi_count;
		if(IsEqualGUID(i_media_subtype,MEDIASUBTYPE_RGB24)){
			bi_count=24;
		}else if(i_media_subtype==MEDIASUBTYPE_RGB32){
			bi_count=32;
		}else{
			throw NyWin32CaptureException();
		}
		o_bmih.biSize=sizeof(BITMAPINFOHEADER);
		o_bmih.biWidth=i_width;
		o_bmih.biHeight=i_height;
		o_bmih.biPlanes=1;
		o_bmih.biBitCount=bi_count;
		o_bmih.biCompression=0;
		o_bmih.biSizeImage=i_width*i_height*(bi_count/8);
		o_bmih.biXPelsPerMeter=0;
		o_bmih.biYPelsPerMeter=0;
		o_bmih.biClrUsed=0;
		o_bmih.biClrImportant=0;
	}
}

//VideoFormatList
//
namespace NyWin32Capture
{
	VideoFormatList::VideoFormatList()
	{
		this->_list=new std::vector<VideoFormat*>();
	}


	void VideoFormatList::clear()
	{
		for(int i=this->_list->size()-1;i>=0;i--){
			delete (*this->_list)[i];
		}
		this->_list->clear();
	}

	void VideoFormatList::update(IAMStreamConfig* i_config)
	{
		this->clear();
		//�t�H�[�}�b�g�̐����擾
		int iCount;
		int iSize;
		VIDEO_STREAM_CONFIG_CAPS scc;
		HRESULT hr;
		hr =i_config->GetNumberOfCapabilities( &iCount, &iSize );
		if(FAILED(hr) ||sizeof(scc) != iSize){
			throw NyWin32CaptureException();
		}
		for(int i=0;i<iCount;i++){
			AM_MEDIA_TYPE *pmt;
			hr = i_config->GetStreamCaps(i, &pmt,reinterpret_cast<BYTE*>(&scc));
			if(hr != S_OK)
			{
				DeleteMediaType(pmt);
				continue;
			}
			if (pmt->formattype != FORMAT_VideoInfo)
			{
				DeleteMediaType(pmt);
				continue;
			}
			VideoFormat* fmt=new VideoFormat(pmt,scc);
			//�ǉ�
			this->_list->push_back(fmt);
		}
		return;
	}
	VideoFormatList::~VideoFormatList()
	{
		this->clear();
		delete this->_list;
	}
	/**
	�t�H�[�}�b�g�L�[�ɏ]���āA�t�H�[�}�b�g����������B
	*/
	const VideoFormat* VideoFormatList::getFormat(int i_width,int i_height,const GUID& i_format)const
	{
		//��v���邻����ۂ��̂�T���B
		int l=(int)this->_list->size();
		for(int i=0;i<l;i++){
			const VideoFormat* f=((*this->_list)[i]);
			const VIDEOINFOHEADER* v=f->getVideoInfoHeader();
			if(i_width!=v->bmiHeader.biWidth)
			{
				continue;
			}
			if(i_height!=v->bmiHeader.biHeight)
			{
				continue;
			}
			if(i_format!=f->getMediaSubType())
			{
				continue;
			}
			return f;
		}
		return NULL;
	}
	const VideoFormat* VideoFormatList::getFormat(int i_width,int i_height)const
	{
		//��v���邻����ۂ��̂�T���B
		int l=(int)this->_list->size();
		for(int i=0;i<l;i++){
			const VideoFormat* f=((*this->_list)[i]);
			const VIDEOINFOHEADER* v=f->getVideoInfoHeader();
			if(i_width!=v->bmiHeader.biWidth)
			{
				continue;
			}
			if(i_height!=v->bmiHeader.biHeight)
			{
				continue;
			}
			return f;
		}
		return NULL;
	}
	const VideoFormat* VideoFormatList::getFormat(int i_index)const
	{
		return (*this->_list)[i_index];
	}
	int VideoFormatList::getNumberOfFormat()const
	{
		return (int)this->_list->size();
	}



}


namespace NyWin32Capture
{
	CaptureDevice::CaptureDevice(IMoniker* i_moniker)
	{
		//HRESULT�`�F�b�N���ĂȂ��Ƃ��낪����B
		HRESULT hr;
		hr=i_moniker-> BindToObject( 0, 0, IID_IBaseFilter, (void**)&this->ds_res.sorce.filter);
		this->_moniker =i_moniker;
		{	//name�̎擾
			IPropertyBag* bag;
			hr=this->_moniker->BindToStorage(NULL,NULL,IID_IPropertyBag,(void**)&bag);
			if(!SUCCEEDED(hr)){
				throw NyWin32CaptureException();
			}
			VARIANT varName;
			varName.vt = VT_BSTR;
			hr=bag->Read(L"FriendlyName",&varName,NULL);
			size_t len=wcslen(varName.bstrVal)+1;
			this->_allocated_res.name=new WCHAR[len];
			wcscpy_s(this->_allocated_res.name,len,varName.bstrVal);
			VariantClear(&varName);
			bag->Release();
		}
		this->_image_cb=NULL;
		this->_status  =ST_CLOSED;
	}
	CaptureDevice::~CaptureDevice()
	{
		switch(this->_status)
		{
		case ST_IDLE:
			this->closeDevice();
			break;
		case ST_RUN:
			this->stopCapture();
			this->closeDevice();
			break;
		default:
			break;
		}
		delete [] this->_allocated_res.name;
		this->_moniker->Release();
		this->ds_res.sorce.filter->Release();
	}
	void CaptureDevice::mStartCapture(BOOL i_set_bffer_samples)
	{
		//��ԃ`�F�b�N
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		HRESULT hr;
		//�s��2�{���擾
		AutoReleaseComPtr<IPin> dest_pin(mFindPinByDirection(this->ds_res.render.filter, PINDIR_INPUT,0));

		//�L���v�`���O���t��ڑ�
		hr=this->ds_res.graph_builder.graph->Connect(this->ds_res.sorce.pin,dest_pin);
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		//����̃L���v�`�������擾
		hr=this->ds_res.render.grab->GetConnectedMediaType(&this->_capture_mediatype);
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}
		FreeMediaType(this->_capture_mediatype);

		//�����_���̊J�n
		hr=this->ds_res.render.grab->SetBufferSamples(i_set_bffer_samples);	// �O���u�J�n
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		hr=this->ds_res.graph_builder.mc->Run();               // �����_�����O�J�n
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		this->_status=ST_RUN;
		return;
	}
	void CaptureDevice::startCapture()
	{
		mStartCapture(TRUE);
	}

	void CaptureDevice::startCaptureCallback(OnCaptureImage i_callback)
	{
		assert(i_callback!=NULL);
		//��ԃ`�F�b�N
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		HRESULT hr;
		try{
			//�R�[���o�b�N�I�u�W�F�N�g���B
			//�R�[���o�b�N���Z�b�g����B
			this->_image_cb=new CaptureImageCallback(this,i_callback);
			hr=this->ds_res.render.grab->SetCallback(this->_image_cb,1);
			hr=this->ds_res.render.grab->SetBufferSamples(FALSE);
		}catch(...){
			//���s�����Ƃ��̓R�[���o�b�N�֘A�̃I�u�W�F�N�g���폜
			this->ds_res.render.grab->SetCallback(NULL,1);
			this->_image_cb->Release();
			this->_image_cb=NULL;
		}
		mStartCapture(FALSE);
	}

	void CaptureDevice::stopCapture()
	{
		//��ԃ`�F�b�N
		if(this->_status!=ST_RUN){
			throw NyWin32CaptureException();
		}
		//HRESULT�`�F�b�N���ĂȂ��Ƃ��낪����B
		HRESULT hr;

		//�t�B���^���~�߂�
		hr=this->ds_res.graph_builder.mc->Pause();
		//�R�[���o�b�N���w�肳��Ă���Ή���
		if(this->_image_cb!=NULL){
			this->ds_res.render.grab->SetCallback(NULL,1);
			this->_image_cb=NULL;
		}
		//��~����
		hr=this->ds_res.graph_builder.mc->Stop();

		//�t�B���^���̑Spin��ؒf
		mDisconnectAll(this->ds_res.graph_builder.graph);

		this->_status=ST_IDLE;
		return;
	}
	const AM_MEDIA_TYPE& CaptureDevice::getMediaType()const
	{
		if(this->_status!=ST_RUN){
			throw NyWin32CaptureException();
		}
		return this->_capture_mediatype;
	}
	/*	�L���v�`���f�o�C�X���I�[�v�����܂��B
	*/
	void CaptureDevice::openDevice()
	{
		//��ԃ`�F�b�N
		if(this->_status!=ST_CLOSED)
		{
			throw NyWin32CaptureException();
		}

		HRESULT hr;

		// �O���o�t�B���^�����
		AutoReleaseComPtr<IBaseFilter> pF;
		hr=CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (LPVOID *)&(pF.ptr));
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		AutoReleaseComPtr<ISampleGrabber> pGrab;
		hr=pF -> QueryInterface( IID_ISampleGrabber, (void **)&(pGrab.ptr));
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		//�L���v�`���O���t�����  
		AutoReleaseComPtr<IGraphBuilder> pGraph;
		hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,IID_IGraphBuilder , (void **) &(pGraph.ptr));
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		AutoReleaseComPtr<IMediaControl> pMC;
		hr=pGraph -> QueryInterface( IID_IMediaControl, (LPVOID *) &(pMC.ptr));
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}
		// �L���v�`���t�B���^���t�B���^�O���t�ɒǉ�
		hr=pGraph -> AddFilter(this->ds_res.sorce.filter, L"CaptureFilter");
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		//�T���v���O���o�̒ǉ�
		hr=pGraph -> AddFilter(pF, L"SampleGrabber");
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}

		//Config�𓾂�(STILL��PREVIEW�̏�)
		AutoReleaseComPtr<IPin> pin;
		pin.ptr=mFindPinByCategory(this->ds_res.sorce.filter,PIN_CATEGORY_CAPTURE);
		if(pin.ptr==NULL){
			pin.ptr=mFindPinByCategory(this->ds_res.sorce.filter,PIN_CATEGORY_STILL);
		}
		AutoReleaseComPtr<IAMStreamConfig> config;
		if(!mGetIAMStreamConfig(pin,&config.ptr)){
			throw NyWin32CaptureException();
		}

	
		//�����J���|�C���^����؂藣��
		this->ds_res.render.filter=pF.detach();
		this->ds_res.render.grab=pGrab.detach();

		this->ds_res.graph_builder.graph=pGraph.detach();
		this->ds_res.graph_builder.mc=pMC.detach();

		this->ds_res.sorce.pin=pin.detach();
		this->ds_res.sorce.config=config.detach();

		this->_status=ST_IDLE;
		return;
	}
	void CaptureDevice::closeDevice()
	{
		//��ԃ`�F�b�N
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		//�C���^�t�F�C�X�̃����[�X
		DWORD d;
		d=this->ds_res.sorce.config->Release();
		d=this->ds_res.sorce.pin->Release();
		d=this->ds_res.graph_builder.mc->Release();
		d=this->ds_res.graph_builder.graph->Release();
		d=this->ds_res.render.filter->Release();
		d=this->ds_res.render.grab->Release();

		this->_status=ST_CLOSED;
	}


	void CaptureDevice::getVideoFormatList(VideoFormatList& o_list)const
	{
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		o_list.update(this->ds_res.sorce.config);
	}
	//i_buf�ɂ́Athis->_capture_mediatype.lSampleSize���傫�ȃT�C�Y�̃o�b�t�@��^���邱�ƁB
	bool CaptureDevice::captureImage(void* i_buf,long i_buf_size)
	{
		if(this->_status!=ST_RUN){
			throw NyWin32CaptureException();
		}
		if(this->_image_cb!=NULL){
			throw NyWin32CaptureException();
		}

		HRESULT hr;
		long n=i_buf_size==0?this->_capture_mediatype.lSampleSize:i_buf_size;

		hr = this->ds_res.render.grab -> GetCurrentBuffer(&n,(long *)i_buf);
		return SUCCEEDED(hr);
	}
	bool CaptureDevice::setVideoFormat(int i_width,int i_height,const GUID& i_media_subtype,double i_rate)
	{
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		VideoFormatList list;
		this->getVideoFormatList(list);
		const VideoFormat* vf;
		//�T�|�[�g���Ă�t�H�[�}�b�g������
		vf=list.getFormat(i_width,i_height,i_media_subtype);
		bool result;
		if(vf!=NULL){
			result=this->setVideoFormat(*vf,i_rate);
		}else{
			vf=list.getFormat(i_width,i_height);
			if(vf==NULL){
				result=false;
			}
			result=this->setVideoFormat(*vf,i_media_subtype,i_rate);
		}
		return result;
	}
	bool CaptureDevice::setVideoFormat(const VideoFormat& i_format,double i_rate)
	{
		return setVideoFormat(i_format,i_format.getMediaType()->subtype,i_rate);
	}
	bool CaptureDevice::setVideoFormat(const VideoFormat& i_format,const GUID& i_media_subtype,double i_rate)
	{
		if(this->_status!=ST_IDLE){
			throw NyWin32CaptureException();
		}
		//�T���v���O���o�̎󂯓���t�H�[�}�b�g�ݒ�
		HRESULT hr;
		AM_MEDIA_TYPE* nmt=CreateMediaType(i_format.getMediaType());
		nmt->subtype=i_media_subtype;
		hr=this->ds_res.render.grab->SetMediaType(nmt);
		if(!SUCCEEDED(hr)){
			DeleteMediaType(nmt);
			return false;
		}
		//StreamConfig�̐ݒ�
		nmt->subtype=i_format.getMediaSubType();
		VIDEOINFOHEADER* vf=reinterpret_cast<VIDEOINFOHEADER*>(nmt->pbFormat);
		vf->AvgTimePerFrame=(REFERENCE_TIME)(10000000.0/i_rate);
		hr=this->ds_res.sorce.config->SetFormat(nmt);
		if(!SUCCEEDED(hr)){
			DeleteMediaType(nmt);
			return false;
		}
		DeleteMediaType(nmt);
		return true;
	}
	const WCHAR* CaptureDevice::getName()const
	{
		if(this->_status==ST_CLOSED){
			throw NyWin32CaptureException();
		}
		return this->_allocated_res.name;
	}
	void* CaptureDevice::getUserValue()const
	{
		return this->_user_value;
	}
	void CaptureDevice::setUserValue(void* i_user_value)
	{
		this->_user_value=i_user_value;
	}

}

namespace NyWin32Capture
{
//		bool startCaptureCallback(ICaptureImageListener i_listener);

	void CaptureDeviceList::createDeviceList()
	{
		//���X�g�̍\�z
		HRESULT hr;
		// ---- �L���v�`���t�B���^�̏��� ----
		// �L���v�`���f�o�C�X��T��
		ULONG cFetched;
		// �f�o�C�X�񋓎q���쐬
		AutoReleaseComPtr<ICreateDevEnum> pDevEnum;
		hr=CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,IID_ICreateDevEnum, (void ** ) &(pDevEnum.ptr));
		if(!SUCCEEDED(hr)){
			throw NyWin32CaptureException();
		}
		// �r�f�I�L���v�`���f�o�C�X�񋓎q���쐬
		AutoReleaseComPtr<IEnumMoniker> pClassEnum;
		hr=pDevEnum -> CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &(pClassEnum.ptr), 0);
		if(hr!=S_OK){
			// do nothing
		}else{
			IMoniker * pMoniker = NULL;
			for(;;)
			{
				hr=pClassEnum -> Next(1, &pMoniker, &cFetched);
				if(hr!=S_OK){
					break;
				}
				CaptureDevice* cd=new CaptureDevice(pMoniker);
				this->_list->push_back(cd);
			}
		}
	}
	void CaptureDeviceList::releaseDeviceList()
	{
		for(unsigned int i=0;i<this->_list->size();i++){
			delete (*this->_list)[i];
		}
		(*this->_list).clear();
	}


	CaptureDeviceList::CaptureDeviceList()
	{
		this->_list=new vector<CaptureDevice*>;
		this->createDeviceList();
	}
	CaptureDeviceList::~CaptureDeviceList()
	{
		this->releaseDeviceList();
		delete this->_list;
	}
	//	i_index�Ԗڂ̃L���v�`���f�o�C�X���擾���܂��B
	CaptureDevice* CaptureDeviceList::getDevice(int i_index)const
	{
		return (*this->_list)[i_index];
	}
	//	�f�o�C�X�̐����擾���܂��B
	int CaptureDeviceList::getNumberOfDevice()const
	{
		return this->_list->size();
	}
};

