
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

#define WM_USER_COMM_FINISHED WM_USER + 0x110
#define WM_USER_COMM_ERROR WM_USER + 0x111




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
	CFont *mLableStateFont, mMyEditFont;	

	//������ؿؼ������Ķ���
public:
	//�ؼ�����
	CEdit mControlFeedingOKNumber;
	CEdit mControlFeedingNGNumber;
	CEdit mControlClearanceOKNumber;
	CEdit mControlClearanceNGNumber;

	//���ر���
	int mValueFeedingSystemSwitch;		// ����ϵͳ����
	int mValueLeakySystemSwitch;		// ©����ϵͳ����
	int mValueClearanceSystemSwitch;	// ��϶���ϵͳ����
	int mValueUnloadingSystemSwith;		// ����ϵͳ����
	int mValueRotarySystemSwitch;		// ��תϵͳ����

	//��������
	UINT mValueFeedingOKNumber;
	int mValueFeedingNGNumber;
	UINT mValueClearanceOKNumber;
	UINT mValueClearanceNGNumber;

	//©������ʾ
	CString m_LeakyCheckResult;
	//��λ��ʾ
	CString m_PositionResult;
	//��϶�����ʾ
	CString m_ClearanceCheckResult;

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
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();

	//�߳���Ϣ
protected:
	afx_msg LRESULT OnUserGrabFished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserStratCheckLeakey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserLeakeyFished(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnUserStartLocation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserLocationFished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserStartClearance(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserClearanceFished(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUserCommFished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCommError(WPARAM wParam, LPARAM lParam);

	protected:
		CWinThread* m_tRunState;
		bool m_commRunning;
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
