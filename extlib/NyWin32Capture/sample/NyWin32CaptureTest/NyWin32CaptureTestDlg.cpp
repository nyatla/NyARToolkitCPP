// NyWin32CaptureTestDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include <assert.h>
#include "NyWin32Capture.h"
#include "NyWin32CaptureTest.h"
#include "NyWin32CaptureTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace NyWin32Capture;

/*	�A�v���P�[�V�����R���g���[��
*/
class AppCtrl
{
public:
	CWnd* _wnd;
	CaptureDeviceList* devlist;
	CaptureDevice* dev;
	BITMAPINFOHEADER dibheader;
public:
	AppCtrl(CWnd* i_app_window)
	{
		CoInitialize(NULL);

		this->_wnd=i_app_window;
		CButton* bn=(CButton*)_wnd->GetDlgItem(ID_SWITCH);
		bn->SetWindowText(_T("start capture"));
		//�J�����L���v�`���f�o�C�X�̃��X�g�����B
		devlist=new CaptureDeviceList();
		//�L���v�`���f�o�C�X�̃��X�g����1�Ԗڂ̃J�����𓾂�B
		int nod=this->devlist->getNumberOfDevice();
		if(nod<1){
			throw std::exception("This computer has not Capture device.");
		}
		//0�Ԗڂ̃J������������B
		CaptureDevice* d=devlist->getDevice(0);
		d->openDevice();
		//�L���v�`���t�H�[�}�b�g��ݒ�
		SetupCaptureFormat(d,this->dibheader);
		//�R�[���o�b�N���[�h�̎��Ɏg�����[�U��`�l��ݒ�
		d->setUserValue(this);
		this->dev=d;

	}
	virtual ~AppCtrl()
	{
		delete devlist;
		//�J�����L���v�`���f�o�C�X�̃��X�g���J��
		CoUninitialize();


	}
	//�P�Ԗڂ̃J������QVGA�ŃL���v�`���ł���悤�ɐݒ肷��B
	bool SetupCaptureFormat(CaptureDevice* d,BITMAPINFOHEADER& bmiheader)
	{
		//�t�H�[�}�b�g���X�g�𓾂�B
		VideoFormatList lt;
		d->getVideoFormatList(lt);
		int nof=lt.getNumberOfFormat();
		if(nof<1){
			return "The device has not Video format.";
		}
		//�C���[�W�̃t�H�[�}�b�g��ݒ�B���̃^�C�v��uuids.h�ɏ����Ă���B
		const VideoFormat* vf=lt.getFormat(320,240,MEDIASUBTYPE_RGB24);
		if(vf==NULL){
			//�J�������t�H�[�}�b�g�������ĂȂ��ꍇ�̓C���e���W�F���g�ڑ��������B
			d->setVideoFormat(320,240,MEDIASUBTYPE_RGB24,30.0);
			VideoFormat::initBITMAPINFOHEADER(320,240,MEDIASUBTYPE_RGB24,bmiheader);
		}else{
			//�J�������t�H�[�}�b�g�������Ă�ꍇ�͂��̂܂܎g��
			d->setVideoFormat(*vf,30.0);
			bmiheader=*(vf->getBitmapInfoHeader());
		}
		return true;
	}
	//�������ăC���[�W���擾���āA�E�C���h�E�ɕ`��
	void DrawBitmap()
	{
		//�C���[�W�𓯊��擾
		const AM_MEDIA_TYPE& mt=dev->getMediaType();
		BYTE* buf;
		buf=new BYTE[mt.lSampleSize];
		this->dev->captureImage(buf);
		//�`��
		DrawBitmap(buf);
		delete[] buf;
	}
	//data�̓��e���E�C���h�E�ɕ`��
	void DrawBitmap(void* data)
	{
		//DIB��BITMAP�ɕϊ����ĕ`��
		CDC* dc=this->_wnd->GetDC();
		CBitmap bmp;
		BITMAPINFO bmi;
		bmi.bmiHeader=this->dibheader;
		int l=SetDIBitsToDevice(
			dc->m_hDC,0,0,this->dibheader.biWidth,this->dibheader.biHeight,
			0,0,0,this->dibheader.biHeight,data,&bmi,DIB_RGB_COLORS);
		this->_wnd->ReleaseDC(dc);
	}
	void Start(bool is_async)
	{
		if(!is_async){
			this->dev->startCapture();	//�����擾�̎����Ȃ炱����
		}else{
			this->dev->startCaptureCallback(AppCtrl::OnCaptureImage);		//�񓯊��擾�̎����Ȃ炱����
		}
	}
	void Stop()
	{
		this->dev->stopCapture();
	}
	static void OnCaptureImage(const CaptureDevice* i_sender,BYTE *pBuffer, long BufferLen)
	{
		//SendMessage�Ŕ�΂�
		AppCtrl* inst=(AppCtrl*)(i_sender->getUserValue());
		DWORD ret;
		//GDI�Ɠ������Ƃ�Ƃ��Ƀf�b�g���b�N����\�������邩��B
		if(SendMessageTimeout(inst->_wnd->m_hWnd,WM_USER+39,0,(LPARAM)pBuffer,SMTO_ABORTIFHUNG,1000,&ret)==0){
			OutputDebugString(_T("Error on SendMessageTimeout"));
		}
	}
};


//�N���X�����J�������Ȃ��̂ŃO���[�o���ϐ��ɂ��Ă��邯�ǁA���ۂ�
//�e�N���X�̃����o�ɂ��������悢�ł��B
AppCtrl* appctrl;




// CNyWin32CaptureTestDlg �_�C�A���O




CNyWin32CaptureTestDlg::CNyWin32CaptureTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNyWin32CaptureTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNyWin32CaptureTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNyWin32CaptureTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_SWITCH, &CNyWin32CaptureTestDlg::OnBnClickedSwitch)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
    ON_MESSAGE((WM_USER+39),CNyWin32CaptureTestDlg::OnASyncCapture)//�L���v�`��
END_MESSAGE_MAP()


// CNyWin32CaptureTestDlg ���b�Z�[�W �n���h��

BOOL CNyWin32CaptureTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	this->is_start=false;

	appctrl=new AppCtrl(this);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CNyWin32CaptureTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialog::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CNyWin32CaptureTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//�{�^���������ꂽ�Ƃ�
void CNyWin32CaptureTestDlg::OnBnClickedSwitch()
{
	//�L���v�`��ON��OFF�̐؂�ւ�
	CButton* bn=(CButton*)this->GetDlgItem(ID_SWITCH);
	this->is_start=(!this->is_start);
	bn->SetWindowText(this->is_start?_T("stop capture"):_T("start capture"));
/*	//�������s�e�X�g
	if(this->is_start){
		appctrl->Start(false);
		this->SetTimer(123,100,NULL);
	}else{
		this->KillTimer(123);
		appctrl->Stop();
	}*/
	//�񓯊������Ȃ炱����
	if(this->is_start){
		appctrl->Start(true);
	}else{
		appctrl->Stop();
	}
}

void CNyWin32CaptureTestDlg::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDialog::OnClose();
}

void CNyWin32CaptureTestDlg::OnDestroy()
{
	delete appctrl;
	CDialog::OnDestroy();
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CNyWin32CaptureTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	appctrl->DrawBitmap();
	CDialog::OnTimer(nIDEvent);
}
//SendMessage�Ŕ�΂������b�Z�[�W�̎󂯌�
LRESULT CNyWin32CaptureTestDlg::OnASyncCapture(WPARAM wParam, LPARAM lParam )
{
	//���[����64bit�|�C���^���Ƃ܂����Ȃ��H
	appctrl->DrawBitmap((void*)lParam);
	return 0;
}
