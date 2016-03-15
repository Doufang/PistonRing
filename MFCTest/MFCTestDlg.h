
// MFCTestDlg.h : ͷ�ļ�
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

// CMFCTestDlg �Ի���
class CMFCTestDlg : public CDialogEx
{
// ����
public:
	CMFCTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCTEST_DIALOG };

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

