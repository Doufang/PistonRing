#pragma once


// CUIThreadDlg �Ի���

class CUIThreadDlg : public CDialog
{
	DECLARE_DYNAMIC(CUIThreadDlg)

public:
	CUIThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUIThreadDlg();

// �Ի�������
	enum { IDD = IDD_UITHREADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
