#pragma once


#define WM_CALCULATE WM_USER+1

// CCalculateThread

class CCalculateThread : public CWinThread
{
	DECLARE_DYNCREATE(CCalculateThread)

protected:
	CCalculateThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalculateThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnCalculate(UINT wParam, LONG lParam);
};


