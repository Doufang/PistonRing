// UIThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread6.h"
#include "UIThread.h"


// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
{
}

CUIThread::~CUIThread()
{
}

BOOL CUIThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	m_dlg.Create(IDD_UITHREADDLG);
	m_dlg.ShowWindow(SW_SHOW);
	m_pMainWnd = &m_dlg;
	return TRUE;
}

int CUIThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	m_dlg.DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
END_MESSAGE_MAP()

