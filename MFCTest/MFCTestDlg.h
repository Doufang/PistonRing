
// MFCTestDlg.h : 头文件
//

#pragma once
#include <opencv.hpp>
#include <Public.h>

#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon/Image.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/InstantCamera.h>

#include <ListArrayLib/ListArrayLib.h>
#include <ImagePre/ImagePre.h>
#include <ObjectLib/ObjectLib.h>

#pragma comment(lib, "ListArrayLib.lib")
#pragma comment(lib, "ImagePre.lib")
#pragma comment(lib, "ObjectLib.lib")

#include "SerialComm.h"
#include "ThreadProgram.h"

#define WM_USER_COMM_FINISHED WM_USER + 0x500


extern UINT ThreadCommReadRelay(LPVOID pParam);

// CMFCTestDlg 对话框
class CMFCTestDlg : public CDialogEx
{
// 构造
public:
	CMFCTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCTEST_DIALOG };

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
	afx_msg void OnBnClickedButton1();

	CEvent m_event;
	bool Running;
	afx_msg LRESULT CommFished(WPARAM wParam, LPARAM lParam);
	CWinThread* m_threadComm;
	CCriticalSection m_csComm;
	

	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();


};

void JDDetect(IplImage* sourceImage);

void cvRefineForegroundMaskBySegm(CvSeq* segments, IplImage*  pFrImg);

CvSeq*cvSegmentFGMask(CvArr* _mask, int poly1Hull0, float perimScale, CvMemStorage* storage, CvPoint offset);

void getMaxDistance(IplImage* img, ListPoint* line, int* a);

bool getMinYPositon(ListPoint* line, int firstPosition, int lastPosition, CvRect* zone, int* a, CvSize imgSize, int direction);

bool checkLightNum(IplImage* ZoneImg, int LeakLightNum);

bool CheckZoneLeak(IplImage* currImageBefore, CvRect Zone, int LeakLightNum);

void ByteToBool(byte* sourceByte, int number, bool* boolValue);

void INTToByte(UINT16 intSource, byte* byHigh, byte* byLow);
void DwordToWord(DWORD dwordSource, WORD* wordHigh, WORD* wordLow);

