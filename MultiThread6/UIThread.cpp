// UIThread.cpp : ʵ���ļ�
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
	// TODO:    �ڴ�ִ���������̳߳�ʼ��
	m_dlg.Create(IDD_UITHREADDLG);
	m_dlg.ShowWindow(SW_SHOW);
	m_pMainWnd = &m_dlg;
	return TRUE;
}

int CUIThread::ExitInstance()
{
	// TODO:    �ڴ�ִ���������߳�����
	m_dlg.DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
END_MESSAGE_MAP()

