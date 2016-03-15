
// MultiThread1Dlg.h : 头文件
//

#pragma once


// CMultiThread1Dlg 对话框
class CMultiThread1Dlg : public CDialogEx
{
// 构造
public:
	CMultiThread1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MULTITHREAD1_DIALOG };

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

protected:
	HANDLE hThread;			//线程句柄
	DWORD ThreadID;			//线程ID
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};

void ThreadFunc(HWND hWnd);