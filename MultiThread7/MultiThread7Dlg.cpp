
// MultiThread7Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread7.h"
#include "MultiThread7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiThread7Dlg 对话框



CMultiThread7Dlg::CMultiThread7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread7Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiThread7Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DISPLAY, OnDisplay)
	ON_BN_CLICKED(IDC_RADIO1, &CMultiThread7Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMultiThread7Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMultiThread7Dlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_SUM, &CMultiThread7Dlg::OnBnClickedSum)
END_MESSAGE_MAP()


// CMultiThread7Dlg 消息处理程序

BOOL CMultiThread7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	nAddend = 10;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultiThread7Dlg::OnPaint()
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
HCURSOR CMultiThread7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiThread7Dlg::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	nAddend = 10;
}


void CMultiThread7Dlg::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	nAddend = 50;
}


void CMultiThread7Dlg::OnBnClickedRadio3()
{
	// TODO:  在此添加控件通知处理程序代码
	nAddend = 100;
}

LRESULT CMultiThread7Dlg::OnDisplay(WPARAM wParam, LPARAM lParam)
{
	int nTemp = (int)wParam;
	SetDlgItemInt(IDC_STATUS1, nTemp, FALSE);
	return 0;

}

void CMultiThread7Dlg::OnBnClickedSum()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pCalculateThread =
		(CCalculateThread*)AfxBeginThread(RUNTIME_CLASS(CCalculateThread));
	//Sleep(500);
	m_pCalculateThread->PostThreadMessage(WM_CALCULATE, nAddend, NULL);
}
