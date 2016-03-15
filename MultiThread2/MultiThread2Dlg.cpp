
// MultiThread2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread2.h"
#include "MultiThread2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiThread2Dlg 对话框



CMultiThread2Dlg::CMultiThread2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread2Dlg::IDD, pParent)
	, m_nCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT, m_nCount);
}

BEGIN_MESSAGE_MAP(CMultiThread2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CMultiThread2Dlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CMultiThread2Dlg 消息处理程序

BOOL CMultiThread2Dlg::OnInitDialog()
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

void CMultiThread2Dlg::OnPaint()
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
HCURSOR CMultiThread2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ThreadFunc(int integer)

{
	int i;
	for (i = 0; i<integer; i++)
	{
		Beep(200, 50);
		Sleep(1000);
	}
}

void CMultiThread2Dlg::OnBnClickedStart()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int integer = m_nCount;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (VOID*)integer, 0, &ThreadID);

	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	DWORD d = WaitForSingleObject(hThread, INFINITE);
	
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}
