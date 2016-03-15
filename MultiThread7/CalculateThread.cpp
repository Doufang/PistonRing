// CalculateThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread7.h"
#include "CalculateThread.h"
#include "MultiThread7Dlg.h"


// CCalculateThread

IMPLEMENT_DYNCREATE(CCalculateThread, CWinThread)

CCalculateThread::CCalculateThread()
{
}

CCalculateThread::~CCalculateThread()
{
}

BOOL CCalculateThread::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	return TRUE;
}

int CCalculateThread::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCalculateThread, CWinThread)
	ON_THREAD_MESSAGE(WM_CALCULATE, OnCalculate)
END_MESSAGE_MAP()


// CCalculateThread 消息处理程序
void CCalculateThread::OnCalculate(UINT wParam, LONG lParam)
{
	int nTmpt = 0;
	for (int i = 0; i <= (int)wParam; i++)
	{
		nTmpt = nTmpt + i;
	}
	Sleep(500);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DISPLAY, nTmpt, NULL);

	//return 0;

}