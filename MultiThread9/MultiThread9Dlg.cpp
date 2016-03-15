
// MultiThread9Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread9.h"
#include "MultiThread9Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEvent eventWriteD(false, true);
CString str;
CString str1;
// CMultiThread9Dlg �Ի���



CMultiThread9Dlg::CMultiThread9Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread9Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread9Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_W, m_ctrlW);
	DDX_Control(pDX, IDC_D, m_ctrlD);
	DDX_Control(pDX, IDC_F, m_ctrlF);
}

BEGIN_MESSAGE_MAP(CMultiThread9Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WRITEW, &CMultiThread9Dlg::OnBnClickedWritew)
END_MESSAGE_MAP()


// CMultiThread9Dlg ��Ϣ�������

BOOL CMultiThread9Dlg::OnInitDialog()
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

void CMultiThread9Dlg::OnPaint()
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
HCURSOR CMultiThread9Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT WriteW(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	for (int i = 0; i<10; i++)
	{
		str.Append(L"W");
		pEdit->SetWindowText(str);
		Sleep(100);
	}
	str = "";
	eventWriteD.PulseEvent();
	return 0;

}


UINT WriteD(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	WaitForSingleObject(eventWriteD.m_hObject, INFINITE);
	for (int i = 0; i<10; i++)
	{
		str.Append(L"D");
		pEdit->SetWindowText(str);
		Sleep(100);
	}
	str = "";
	return 0;
}

UINT WriteF(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	WaitForSingleObject(eventWriteD.m_hObject, INFINITE);
	for (int i = 0; i<10; i++)
	{
		str1.Append(L"F");
		pEdit->SetWindowText(str1);
		Sleep(100);
	}
	str1 = "";
	return 0;
}

void CMultiThread9Dlg::OnBnClickedWritew()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWinThread *pWriteW = AfxBeginThread(WriteW, &m_ctrlW, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteW->ResumeThread();
	CWinThread *pWriteD = AfxBeginThread(WriteD, &m_ctrlD, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteD->ResumeThread();
	CWinThread *pWriteF = AfxBeginThread(WriteF, &m_ctrlF, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteF->ResumeThread();
}
