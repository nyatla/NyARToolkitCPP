// NyWin32CaptureTestDlg.h : ヘッダー ファイル
//

#pragma once


// CNyWin32CaptureTestDlg ダイアログ
class CNyWin32CaptureTestDlg : public CDialog
{
// コンストラクション
public:
	CNyWin32CaptureTestDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_NYWIN32CAPTURETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnASyncCapture(UINT wParam,LONG lParam);
	afx_msg void OnBnClickedSwitch();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	bool is_start;
};
