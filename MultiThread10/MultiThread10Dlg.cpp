
// MultiThread10Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread10.h"
#include "MultiThread10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSemaphore semaphoreWrite(2, 2); //��Դ�������߳�2������ǰ�ɷ����߳���2��   
char g_Array[10];
// CMultiThread10Dlg �Ի���



CMultiThread10Dlg::CMultiThread10Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread10Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_A, m_ctrlA);
	DDX_Control(pDX, IDC_B, m_ctrlB);
	DDX_Control(pDX, IDC_C, m_ctrlC);
}

BEGIN_MESSAGE_MAP(CMultiThread10Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CMultiThread10Dlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CMultiThread10Dlg ��Ϣ�������

BOOL CMultiThread10Dlg::OnInitDialog()
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

void CMultiThread10Dlg::OnPaint()
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
HCURSOR CMultiThread10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT WriteA(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);
	CString str;
	for (int i = 0; i<10; i++)
	{
		pEdit->GetWindowText(str);
		g_Array[i] = 'A';
		str = str + g_Array[i];
		pEdit->SetWindowText(str);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;
}

UINT WriteB(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);
	CString str;

	for (int i = 0; i<10; i++)
	{
		pEdit->GetWindowText(str);
		g_Array[i] = 'B';
		str = str + g_Array[i];
		pEdit->SetWindowText(str);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;

}

UINT WriteC(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText("");
	WaitForSingleObject(semaphoreWrite.m_hObject, INFINITE);

	for (int i = 0; i<10; i++)
	{
		g_Array[i] = 'C';
		pEdit->SetWindowText(g_Array);
		//Sleep(1);
	}
	
	ReleaseSemaphore(semaphoreWrite.m_hObject, 1, NULL);
	return 0;
}

void CMultiThread10Dlg::OnBnClickedStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWinThread *pWriteA = AfxBeginThread(WriteA,
		&m_ctrlA,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteA->ResumeThread();

	CWinThread *pWriteB = AfxBeginThread(WriteB,
		&m_ctrlB,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteB->ResumeThread();

	CWinThread *pWriteC = AfxBeginThread(WriteC,
		&m_ctrlC,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteC->ResumeThread();
}
