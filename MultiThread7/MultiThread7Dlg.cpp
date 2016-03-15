
// MultiThread7Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread7.h"
#include "MultiThread7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiThread7Dlg �Ի���



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


// CMultiThread7Dlg ��Ϣ�������

BOOL CMultiThread7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	nAddend = 10;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiThread7Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMultiThread7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiThread7Dlg::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	nAddend = 10;
}


void CMultiThread7Dlg::OnBnClickedRadio2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	nAddend = 50;
}


void CMultiThread7Dlg::OnBnClickedRadio3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pCalculateThread =
		(CCalculateThread*)AfxBeginThread(RUNTIME_CLASS(CCalculateThread));
	//Sleep(500);
	m_pCalculateThread->PostThreadMessage(WM_CALCULATE, nAddend, NULL);
}
