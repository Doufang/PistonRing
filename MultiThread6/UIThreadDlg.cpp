// UIThreadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread6.h"
#include "UIThreadDlg.h"
#include "afxdialogex.h"


// CUIThreadDlg �Ի���

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


// CUIThreadDlg ��Ϣ�������


void CUIThreadDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MessageBox(L"You Clicked The Left Button!");
	CDialog::OnLButtonDown(nFlags, point);
}
