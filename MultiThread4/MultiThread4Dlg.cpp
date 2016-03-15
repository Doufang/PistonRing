
// MultiThread4Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread4.h"
#include "MultiThread4Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


volatile BOOL m_bRunFlag = TRUE;


// CMultiThread4Dlg �Ի���



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


// CMultiThread4Dlg ��Ϣ�������

BOOL CMultiThread4Dlg::OnInitDialog()
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

void CMultiThread4Dlg::OnPaint()
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// ���ϴ����̣߳�ֱ���ٲ��ܴ���Ϊֹ  
	m_nCount = nCount;
	UpdateData(FALSE);
	Sleep(5000);
	// ��ʱ5�룬�ȴ����д������߳̽���  
	GetDlgItem(IDC_TEST)->EnableWindow(TRUE);
	m_bRunFlag = TRUE;
}
