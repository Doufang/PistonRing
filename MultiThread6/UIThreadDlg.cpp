// UIThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiThread6.h"
#include "UIThreadDlg.h"
#include "afxdialogex.h"


// CUIThreadDlg 对话框

IMPLEMENT_DYNAMIC(CUIThreadDlg, CDialog)

CUIThreadDlg::CUIThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUIThreadDlg::IDD, pParent)
{

}

CUIThreadDlg::~CUIThreadDlg()
{
}

void CUIThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUIThreadDlg, CDialog)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CUIThreadDlg 消息处理程序


void CUIThreadDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	MessageBox(L"You Clicked The Left Button!");
	CDialog::OnLButtonDown(nFlags, point);
}
