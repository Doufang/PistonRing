#pragma once


// CUIThreadDlg 对话框

class CUIThreadDlg : public CDialog
{
	DECLARE_DYNAMIC(CUIThreadDlg)

public:
	CUIThreadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUIThreadDlg();

// 对话框数据
	enum { IDD = IDD_UITHREADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
