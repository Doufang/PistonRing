#pragma once
#include "UIThreadDlg.h"


// CUIThread

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

protected:
	CUIThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUIThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
	CUIThreadDlg m_dlg;
};


