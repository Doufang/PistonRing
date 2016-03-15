
// MultiThread5Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CMultiThread5Dlg 对话框
class CMultiThread5Dlg : public CDialogEx
{
// 构造
public:
	CMultiThread5Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MULTITHREAD5_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
