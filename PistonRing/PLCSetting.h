#pragma once
#include "PLCXDM60.h"
#include "SerialComm.h"


//PLC通讯口
extern SerialComm g_CommXDM;
extern CCriticalSection g_CsCommXDM;
extern bool& g_AutoRun;
extern bool& g_Running;

#define WM_USER_START_CHECK_LEAKEY WM_USER + 0x101
#define WM_USER_LEAKEY_FINISHED WM_USER + 0x102

#define WM_USER_START_LOCATION WM_USER + 0x103
#define WM_USER_LOCATION_FINISHED WM_USER + 0x104

#define WM_USER_START_CLEARANCE WM_USER + 0x105
#define WM_USER_CLEARANCE_FINISHED WM_USER + 0x106

// PLCSetting 对话框

class PLCSetting : public CDialogEx
{
	DECLARE_DYNAMIC(PLCSetting)

public:
	PLCSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PLCSetting();

// 对话框数据
	enum { IDD = IDD_PLC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_plcHD1252;
	CString m_plcHD1262;
	CString m_plcHD1272;
	CWnd *m_pParentWnd;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();

	int GetStringNum(CString szVal);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton20();
};
