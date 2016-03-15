
// MultiThread5Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CMultiThread5Dlg �Ի���
class CMultiThread5Dlg : public CDialogEx
{
// ����
public:
	CMultiThread5Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTITHREAD5_DIALOG };

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
	CWinThread* pThread;
	afx_msg void OnBnClickedStart();
};

struct threadInfo
{
	UINT nMilliSecond;
	CProgressCtrl* pctrlProgress;
};

UINT ThreadFunc(LPVOID lpParam);
