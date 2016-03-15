
// MultiThread3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiThread3.h"
#include "MultiThread3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


threadInfo Info;


// CMultiThread3Dlg �Ի���



CMultiThread3Dlg::CMultiThread3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiThread3Dlg::IDD, pParent)
	, m_nMilliSecond(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MILLISECOND, m_nMilliSecond);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CMultiThread3Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CMultiThread3Dlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CMultiThread3Dlg ��Ϣ�������

BOOL CMultiThread3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_ctrlProgress.SetRange(0, 99);
	m_nMilliSecond = 10;
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultiThread3Dlg::OnPaint()
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
HCURSOR CMultiThread3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiThread3Dlg::OnBnClickedStart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	Info.nMilliSecond = m_nMilliSecond;
	Info.pctrlProgress = &m_ctrlProgress;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, &Info, 0, &ThreadID);

	
	//GetDlgItem(IDC_START)->EnableWindow(FALSE);
	//WaitForSingleObject(hThread,INFINITE);
	//GetDlgItem(IDC_START)->EnableWindow(TRUE);
	
}

UINT ThreadFunc(LPVOID lpParam)
{
	threadInfo* pInfo = (threadInfo*)lpParam;
	for (int i = 0; i<100; i++)
	{
		int nTemp = pInfo->nMilliSecond;
		pInfo->pctrlProgress->SetPos(i);
		Sleep(nTemp);
	}
	return 0;

}
