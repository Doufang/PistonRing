
// MultiThread3Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CMultiThread3Dlg �Ի���
class CMultiThread3Dlg : public CDialogEx
{
// ����
public:
	CMultiThread3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTITHREAD3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nMilliSecond;
	CProgressCtrl m_ctrlProgress;
protected:
	HANDLE hThread;
	DWORD ThreadID;
public:
	afx_msg void OnBnClickedStart();
};

struct threadInfo

{
	UINT nMilliSecond;
	CProgressCtrl* pctrlProgress;
};

UINT ThreadFunc(LPVOID lpParam);
