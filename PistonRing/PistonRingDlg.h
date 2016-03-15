
// PistonRingDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "SerialComm.h"
#include <MMSystem.h>
#include <atlimage.h>
#include <Windows.h>
#pragma comment( lib,"winmm.lib" )

#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon/Image.h>
#include <pylon/TlFactory.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/InstantCamera.h>

#include "CvvImage.h"
#include <opencv2/opencv.hpp>

#include "PLCSetting.h"	

#include "..\MachineVisionLib\CameraBasler.h"
#pragma comment(lib, "MachineVisionLib")


using namespace Pylon;
using namespace GenApi;
using namespace std;
using namespace cv;

#define VL_MIN(x,y) (((x)<(y))?(x):(y))
#define VL_MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_GRAY_VALUE 256
#define MIN_GRAY_VALUE 0

#define WM_USER_GRAB_FINISHED WM_USER + 0x100





//漏光相机
extern CameraBasler g_CameraLeakey;
extern CCriticalSection g_CsLeakey;
//定位相机
extern CameraBasler g_CameraLocation;
extern CCriticalSection g_CsLocation;
//间隙检测相机
extern CameraBasler g_CameraClearance;
extern CCriticalSection g_CsClearance;
//PLC通讯口
extern SerialComm g_CommXDM;
extern CCriticalSection g_CsCommXDM;

extern bool& g_AutoRun;
extern bool& g_Running;

// CPistonRingDlg 对话框
class CPistonRingDlg : public CDialogEx
{
// 构造
public:
	CPistonRingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PISTONRING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	//窗口初始化相关操作
	afx_msg void OnClose();			//窗口关闭确认提示
	virtual BOOL PreTranslateMessage(MSG* pMsg);	//截获键盘消息
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//窗体标签颜色设置
	afx_msg void OnSize(UINT nType, int cx, int cy);		//窗体尺寸改变时重新设置控件的大小

	void DialogFontSetting(CDC* pDC, int nPointSet);	//用于设置标签的字体
	void MyEditFontSet();	//
	float oldFSP[2];		//用于记录窗体的原始大小
	CFont mLableStateFont, mMyEditFont;	

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//拍照相关定义
public:
	void DrawPicToHDC(IplImage* scr, UINT ID);

	//线程创建函数
	CWinThread* ThreadCreate(
		AFX_THREADPROC pfnThreadProc,
		SerialComm* pComm,
		CCriticalSection* pCsComm,
		CameraBasler* pCamera,
		CCriticalSection* pCsCamera,
		bool* pRun = NULL,
		DWORD time = 100);

	//相机初始化线程创建
	void CameraInitCreate(
		CameraBasler* pCamera,
		String_t cameraString,
		double exposureTime,
		double frameRate,
		int64_t width,
		int64_t height);

public:
	PLCSetting* myPlC;
	afx_msg void OnPlcSetting();	
	CStatic m_CheckLeakeyStateShow;
	CStatic m_CheckLocationStateShow;
	CStatic m_CheckClearanceStateShow;

	UINT m_LeakeyCheckNumber;
	UINT m_LeakeyRunTime;
	UINT m_PositionRunTime;
	UINT m_ClearanceRunTime;
	UINT m_TotalCount;
	UINT m_OKCount;
	UINT m_LeakeyCount;
	UINT m_ClearanceCount;
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
};

//多线程传递参数
typedef struct ThreadParms
{
	HWND hWnd;
	SerialComm* pComm;
	CCriticalSection* pCsComm;
	CameraBasler* pCamera;
	CCriticalSection* pCsCamera;
	bool* pRun;
	DWORD time;
};

//相机初始化线程参数
typedef struct CameraInitParams
{
	HWND hWnd;
	CameraBasler* pCamera;
	String_t cameraString;
	double exposureTime;
	double frameRate;
	int64_t width;
	int64_t height;
};

//相机初始化
UINT ThreadCameraInit(LPVOID pParam);
//图片采集线程
UINT ThreadCameraGrab(LPVOID pParam);
//漏光检测线程
UINT ThreadCheckLeakey(LPVOID pParam);
//定位线程
UINT ThreadCheakLocation(LPVOID pParam);
//间隙检测线程
UINT ThreadCheakClearance(LPVOID pParam);

//下位机状态采集线程
UINT ThreadRunState(LPVOID pParam);

