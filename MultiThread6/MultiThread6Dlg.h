
// MultiThread6Dlg.h : ͷ�ļ�
//

#pragma once
#include "UIThread.h"

// CMultiThread6Dlg �Ի���
class CMultiThread6Dlg : public CDialogEx
{
// ����
public:
	CMultiThread6Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTITHREAD6_DIALOG };

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
	afx_msg void OnBnClickedUiThread();
};
