
// MultiThread8Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread8.h"
#include "MultiThread8Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection critical_section;
CString str;
// CMultiThread8Dlg 对话框



CMultiThread8Dlg::CMultiThread8Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread8Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_W, m_ctrlW);
	DDX_Control(pDX, IDC_D, m_ctrlD);
}

BEGIN_MESSAGE_MAP(CMultiThread8Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WRITEW, &CMultiThread8Dlg::OnBnClickedWritew)
	ON_BN_CLICKED(IDC_WRITED, &CMultiThread8Dlg::OnBnClickedWrited)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMultiThread8Dlg 消息处理程序

BOOL CMultiThread8Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetTimer(1, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiThread8Dlg::OnPaint()
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
HCURSOR CMultiThread8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT WriteW(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	critical_section.Lock();
	// 锁定临界区，其它线程遇到critical_section.Lock();语句时要等待  
	//直至执行 critical_section.Unlock();语句  

	for (int i = 0; i<10; i++)
	{
		str.Append(L"W");
		pEdit->SetWindowText(str);
		Sleep(200);
	}
	str = "";
	critical_section.Unlock();
	return 0;
}


UINT WriteD(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	critical_section.Lock();
	// 锁定临界区，其它线程遇到critical_section.Lock();语句时要等待  
	//直至执行 critical_section.Unlock();语句  
	
	for (int i = 0; i<10; i++)
	{
		str.Append(L"D");
		pEdit->SetWindowText(str);
		Sleep(200);
	}
	str = "";
	critical_section.Unlock();
	return 0;
}


void CMultiThread8Dlg::OnBnClickedWritew()
{
	// TODO:  在此添加控件通知处理程序代码
	CWinThread *pWriteW = AfxBeginThread(WriteW, &m_ctrlW, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteW->ResumeThread();
}


void CMultiThread8Dlg::OnBnClickedWrited()
{
	// TODO:  在此添加控件通知处理程序代码
	CWinThread *pWriteD = AfxBeginThread(WriteD, &m_ctrlD, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteD->ResumeThread();
}


void CMultiThread8Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{

	}
	CDialogEx::OnTimer(nIDEvent);
}
