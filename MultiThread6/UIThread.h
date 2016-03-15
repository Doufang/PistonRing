#pragma once
#include "UIThreadDlg.h"


// CUIThread

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

protected:
	CUIThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CUIThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
	CUIThreadDlg m_dlg;
};


