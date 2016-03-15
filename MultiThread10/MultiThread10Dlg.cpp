
// MultiThread10Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread10.h"
#include "MultiThread10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSemaphore semaphoreWrite(2, 2); //资源最多访问线程2个，当前可访问线程数2个   
char g_Array[10];
// CMultiThread10Dlg 对话框



CMultiThread10Dlg::CMultiThread10Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread10Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_A, m_ctrlA);
	DDX_Control(pDX, IDC_B, m_ctrlB);
	DDX_Control(pDX, IDC_C, m_ctrlC);
}

BEGIN_MESSAGE_MAP(CMultiThread10Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CMultiThread10Dlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CMultiThread10Dlg 消息处理程序

BOOL CMultiThread10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiThread10Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMultiThread10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT WriteA(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);
	CString str;
	for (int i = 0; i<10; i++)
	{
		pEdit->GetWindowText(str);
		g_Array[i] = 'A';
		str = str + g_Array[i];
		pEdit->SetWindowText(str);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;
}

UINT WriteB(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);
	CString str;

	for (int i = 0; i<10; i++)
	{
		pEdit->GetWindowText(str);
		g_Array[i] = 'B';
		str = str + g_Array[i];
		pEdit->SetWindowText(str);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;

}

UINT WriteC(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);

	for (int i = 0; i<10; i++)
	{
		g_Array[i] = 'C';
		pEdit->SetWindowText(g_Array);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;
}

void CMultiThread10Dlg::OnBnClickedStart()
{
	// TODO:  在此添加控件通知处理程序代码
	CWinThread *pWriteA = AfxBeginThread(WriteA,
		&m_ctrlA,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteA->ResumeThread();

	CWinThread *pWriteB = AfxBeginThread(WriteB,
		&m_ctrlB,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteB->ResumeThread();

	CWinThread *pWriteC = AfxBeginThread(WriteC,
		&m_ctrlC,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteC->ResumeThread();
}
