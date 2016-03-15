
// MultiThread4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread4.h"
#include "MultiThread4Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


volatile BOOL m_bRunFlag = TRUE;


// CMultiThread4Dlg 对话框



CMultiThread4Dlg::CMultiThread4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread4Dlg::IDD, pParent)
	, m_nCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT, m_nCount);
}

BEGIN_MESSAGE_MAP(CMultiThread4Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, &CMultiThread4Dlg::OnBnClickedTest)
END_MESSAGE_MAP()


// CMultiThread4Dlg 消息处理程序

BOOL CMultiThread4Dlg::OnInitDialog()
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

void CMultiThread4Dlg::OnPaint()
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
HCURSOR CMultiThread4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI threadFunc(LPVOID threadNum)
{
	while (m_bRunFlag)
	{
		Sleep(3000);
	}
	return 0;
}

void CMultiThread4Dlg::OnBnClickedTest()
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD threadID;
	GetDlgItem(IDC_TEST)->EnableWindow(FALSE);
	long nCount = 0;
	while (m_bRunFlag)
	{
		if (CreateThread(NULL, 0, threadFunc, NULL, 0, &threadID) == NULL)
		{
			m_bRunFlag = FALSE;
			break;
		}
		else
		{
			nCount++;
		}
	}
	// 不断创建线程，直到再不能创建为止  
	m_nCount = nCount;
	UpdateData(FALSE);
	Sleep(5000);
	// 延时5秒，等待所有创建的线程结束  
	GetDlgItem(IDC_TEST)->EnableWindow(TRUE);
	m_bRunFlag = TRUE;
}
