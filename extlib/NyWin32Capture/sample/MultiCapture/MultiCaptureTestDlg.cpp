// MultiCaptureTestDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include <assert.h>
#include "NyWin32Capture.h"
#include "MultiCaptureTest.h"
#include "MultiCaptureTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace NyWin32Capture;

class CaptureItem
{
public:
	int tag;
	CaptureDevice* dev;
	BITMAPINFOHEADER dibheader;
	HWND hwnd;

	CaptureItem(int i_tag,CaptureDevice* i_dev,const BITMAPINFOHEADER& bmi,HWND i_hwnd)
	{
		this->tag=i_tag;
		this->dev=i_dev;
		this->dibheader=bmi;
		this->hwnd=i_hwnd;
	}
};
/*	�A�v���P�[�V�����R���g���[��
*/
class AppCtrl
{
public:
	CWnd* _wnd;
	CaptureDeviceList* devlist;
	std::vector<CaptureItem*> _item;
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
		//�S���̃J������������
		for(int i=0;i<nod;i++){
			CaptureDevice* d=devlist->getDevice(i);
			d->openDevice();
			//�L���v�`���t�H�[�}�b�g��ݒ�
			BITMAPINFOHEADER b;
			SetupCaptureFormat(d,b);
			//�R�[���o�b�N���[�h�̎��Ɏg�����[�U��`�l��ݒ�
			CaptureItem* item=new CaptureItem(i,d,b,i_app_window->m_hWnd);
			this->_item.push_back(item);
			d->setUserValue(item);
		}
	}
	virtual ~AppCtrl()
	{
		for(int i=0;i<this->_item.size();i++){
			delete _item[i];
		}
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
		//DIB��邽�߂Ƀw�b�_��ۑ����Ă���
		return true;
	}
	//data�̓��e���E�C���h�E�ɕ`��
	void DrawBitmap(void* data,int i)
	{
		//DIB��BITMAP�ɕϊ����ĕ`��
		CDC* dc=this->_wnd->GetDC();
		CBitmap bmp;
		BITMAPINFO bmi;
		bmi.bmiHeader=this->_item[i]->dibheader;
		int x,y;
		x=(i%3)*320;
		y=(i/3)*240+32;
		int l=SetDIBitsToDevice(
			dc->m_hDC,x,y,320,240,
			0,0,0,240,data,&bmi,DIB_RGB_COLORS);
		this->_wnd->ReleaseDC(dc);
	}
	void Start(bool is_async)
	{
		for(int i=0;i<this->_item.size();i++){
			if(!is_async){
				this->_item[i]->dev->startCapture();	//�����擾�̎����Ȃ炱����
			}else{
				this->_item[i]->dev->startCaptureCallback(AppCtrl::OnCaptureImage);		//�񓯊��擾�̎����Ȃ炱����
			}
		}
	}
	void Stop()
	{
		for(int i=0;i<this->_item.size();i++){
			this->_item[i]->dev->stopCapture();
		}
	}
	static void OnCaptureImage(const CaptureDevice* i_sender,BYTE *pBuffer, long BufferLen)
	{
		//SendMessage�Ŕ�΂�
		CaptureItem* inst=(CaptureItem*)(i_sender->getUserValue());
		DWORD ret;
		//GDI�Ɠ������Ƃ�Ƃ��Ƀf�b�g���b�N����\�������邩��B
		if(SendMessageTimeout(inst->hwnd,WM_USER+39,inst->tag,(LPARAM)pBuffer,SMTO_ABORTIFHUNG,1000,&ret)==0){
			OutputDebugString(_T("Error on SendMessageTimeout"));
		}
	}
};


//�N���X�����J�������Ȃ��̂ŃO���[�o���ϐ��ɂ��Ă��邯�ǁA���ۂ�
//�e�N���X�̃����o�ɂ��������悢�ł��B
AppCtrl* appctrl;




// CMultiCaptureTestDlg �_�C�A���O




CMultiCaptureTestDlg::CMultiCaptureTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiCaptureTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiCaptureTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiCaptureTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_SWITCH, &CMultiCaptureTestDlg::OnBnClickedSwitch)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
    ON_MESSAGE((WM_USER+39),CMultiCaptureTestDlg::OnASyncCapture)//�L���v�`��
END_MESSAGE_MAP()


// CMultiCaptureTestDlg ���b�Z�[�W �n���h��

BOOL CMultiCaptureTestDlg::OnInitDialog()
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

void CMultiCaptureTestDlg::OnPaint()
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
HCURSOR CMultiCaptureTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//�{�^���������ꂽ�Ƃ�
void CMultiCaptureTestDlg::OnBnClickedSwitch()
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

void CMultiCaptureTestDlg::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDialog::OnClose();
}

void CMultiCaptureTestDlg::OnDestroy()
{
	delete appctrl;
	CDialog::OnDestroy();
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CMultiCaptureTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}
//SendMessage�Ŕ�΂������b�Z�[�W�̎󂯌�
LRESULT CMultiCaptureTestDlg::OnASyncCapture(WPARAM wParam, LPARAM lParam )
{
	//���[����64bit�|�C���^���Ƃ܂����Ȃ��H
	appctrl->DrawBitmap((void*)lParam,wParam);
	return 0;
}
