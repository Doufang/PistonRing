
// MultiThread1Dlg.h : ͷ�ļ�
//

#pragma once


// CMultiThread1Dlg �Ի���
class CMultiThread1Dlg : public CDialogEx
{
// ����
public:
	CMultiThread1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTITHREAD1_DIALOG };

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

protected:
	HANDLE hThread;			//�߳̾��
	DWORD ThreadID;			//�߳�ID
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};

void ThreadFunc(HWND hWnd);