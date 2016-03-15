
// PistonRingDlg.h : ͷ�ļ�
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





//©�����
extern CameraBasler g_CameraLeakey;
extern CCriticalSection g_CsLeakey;
//��λ���
extern CameraBasler g_CameraLocation;
extern CCriticalSection g_CsLocation;
//��϶������
extern CameraBasler g_CameraClearance;
extern CCriticalSection g_CsClearance;
//PLCͨѶ��
extern SerialComm g_CommXDM;
extern CCriticalSection g_CsCommXDM;

extern bool& g_AutoRun;
extern bool& g_Running;

// CPistonRingDlg �Ի���
class CPistonRingDlg : public CDialogEx
{
// ����
public:
	CPistonRingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PISTONRING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	//���ڳ�ʼ����ز���
	afx_msg void OnClose();			//���ڹر�ȷ����ʾ
	virtual BOOL PreTranslateMessage(MSG* pMsg);	//�ػ������Ϣ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//�����ǩ��ɫ����
	afx_msg void OnSize(UINT nType, int cx, int cy);		//����ߴ�ı�ʱ�������ÿؼ��Ĵ�С

	void DialogFontSetting(CDC* pDC, int nPointSet);	//�������ñ�ǩ������
	void MyEditFontSet();	//
	float oldFSP[2];		//���ڼ�¼�����ԭʼ��С
	CFont mLableStateFont, mMyEditFont;	

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//������ض���
public:
	void DrawPicToHDC(IplImage* scr, UINT ID);

	//�̴߳�������
	CWinThread* ThreadCreate(
		AFX_THREADPROC pfnThreadProc,
		SerialComm* pComm,
		CCriticalSection* pCsComm,
		CameraBasler* pCamera,
		CCriticalSection* pCsCamera,
		bool* pRun = NULL,
		DWORD time = 100);

	//�����ʼ���̴߳���
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

//���̴߳��ݲ���
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

//�����ʼ���̲߳���
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

//�����ʼ��
UINT ThreadCameraInit(LPVOID pParam);
//ͼƬ�ɼ��߳�
UINT ThreadCameraGrab(LPVOID pParam);
//©�����߳�
UINT ThreadCheckLeakey(LPVOID pParam);
//��λ�߳�
UINT ThreadCheakLocation(LPVOID pParam);
//��϶����߳�
UINT ThreadCheakClearance(LPVOID pParam);

//��λ��״̬�ɼ��߳�
UINT ThreadRunState(LPVOID pParam);

