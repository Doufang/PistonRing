
// MultiThread2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread2.h"
#include "MultiThread2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiThread2Dlg �Ի���



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


// CMultiThread2Dlg ��Ϣ�������

BOOL CMultiThread2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiThread2Dlg::OnPaint()
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int integer = m_nCount;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, (VOID*)integer, 0, &ThreadID);

	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	DWORD d = WaitForSingleObject(hThread, INFINITE);
	
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}
