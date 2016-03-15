
// MultiThread8Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread8.h"
#include "MultiThread8Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection critical_section;
CString str;
// CMultiThread8Dlg �Ի���



CMultiThread8Dlg::CMultiThread8Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread8Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_W, m_ctrlW);
	DDX_Control(pDX, IDC_D, m_ctrlD);
}

BEGIN_MESSAGE_MAP(CMultiThread8Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WRITEW, &CMultiThread8Dlg::OnBnClickedWritew)
	ON_BN_CLICKED(IDC_WRITED, &CMultiThread8Dlg::OnBnClickedWrited)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMultiThread8Dlg ��Ϣ�������

BOOL CMultiThread8Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(1, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiThread8Dlg::OnPaint()
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
HCURSOR CMultiThread8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT WriteW(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	critical_section.Lock();
	// �����ٽ����������߳�����critical_section.Lock();���ʱҪ�ȴ�  
	//ֱ��ִ�� critical_section.Unlock();���  

	for (int i = 0; i<10; i++)
	{
		str.Append(L"W");
		pEdit->SetWindowText(str);
		Sleep(200);
	}
	str = "";
	critical_section.Unlock();
	return 0;
}


UINT WriteD(LPVOID pParam)
{
	CEdit *pEdit = (CEdit*)pParam;
	pEdit->SetWindowText(L"");
	critical_section.Lock();
	// �����ٽ����������߳�����critical_section.Lock();���ʱҪ�ȴ�  
	//ֱ��ִ�� critical_section.Unlock();���  
	
	for (int i = 0; i<10; i++)
	{
		str.Append(L"D");
		pEdit->SetWindowText(str);
		Sleep(200);
	}
	str = "";
	critical_section.Unlock();
	return 0;
}


void CMultiThread8Dlg::OnBnClickedWritew()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWinThread *pWriteW = AfxBeginThread(WriteW, &m_ctrlW, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteW->ResumeThread();
}


void CMultiThread8Dlg::OnBnClickedWrited()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWinThread *pWriteD = AfxBeginThread(WriteD, &m_ctrlD, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pWriteD->ResumeThread();
}


void CMultiThread8Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{

	}
	CDialogEx::OnTimer(nIDEvent);
}
