
// PistonRingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PistonRing.h"
#include "PistonRingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���ڳ�ʼ����־����ʼ����ɺ�����Ϊfalse��
BOOL ResizeControl = true;

//©�����
CameraBasler g_CameraLeakey;
CCriticalSection g_CsLeakey;
//��λ���
CameraBasler g_CameraLocation;
CCriticalSection g_CsLocation;
//��϶������
CameraBasler g_CameraClearance;
CCriticalSection g_CsClearance;
//PLCͨѶ��
SerialComm g_CommXDM;
CCriticalSection g_CsCommXDM;
//

//��λ��ͨѶ״̬����
bool g_ReadBool[40] = { 0 };
bool& g_Running = g_ReadBool[0];
bool& g_EMStop = g_ReadBool[1];
bool& g_gReset = g_ReadBool[2];
bool& g_PositionOne = g_ReadBool[3];
bool& g_PositionTwo = g_ReadBool[4];
bool& g_PositionThree = g_ReadBool[5];
bool& g_PositionFour = g_ReadBool[6];
bool& g_AutoRun = g_ReadBool[7];
bool& g_FeedingFinished = g_ReadBool[10];
bool& g_LeakeyFinished = g_ReadBool[11];
bool& g_LocationFinished = g_ReadBool[12];
bool& g_UploadingFinished = g_ReadBool[13];
bool& g_NextStationFinished = g_ReadBool[14];
bool& g_OKStation = g_ReadBool[15];
bool& g_LeakeyStation = g_ReadBool[16];
bool& g_ClearanceStation = g_ReadBool[17];

WORD g_ReadWord[20] = { 0 };
//DWORD g_ReadDword[20];


//����ͼƬ
IplImage* m_maskImage = NULL;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPistonRingDlg �Ի���



CPistonRingDlg::CPistonRingDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CPistonRingDlg::IDD, pParent)
, m_LeakeyCheckNumber(0)
, m_LeakeyRunTime(0)
, m_PositionRunTime(0)
, m_ClearanceRunTime(0)
, m_TotalCount(0)
, m_OKCount(0)
, m_LeakeyCount(0)
, m_ClearanceCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPistonRingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, g_CommXDM.myComm);
	DDX_Control(pDX, IDC_STATIC025, m_CheckLeakeyStateShow);
	DDX_Control(pDX, IDC_STATIC026, m_CheckLocationStateShow);
	DDX_Control(pDX, IDC_STATIC027, m_CheckClearanceStateShow);
	DDX_Text(pDX, IDC_EDIT1, m_LeakeyCheckNumber);
	DDX_Text(pDX, IDC_EDIT2, m_LeakeyRunTime);
	DDX_Text(pDX, IDC_EDIT3, m_PositionRunTime);
	DDX_Text(pDX, IDC_EDIT4, m_ClearanceRunTime);
	DDX_Text(pDX, IDC_EDIT5, m_TotalCount);
	DDX_Text(pDX, IDC_EDIT10, m_OKCount);
	DDX_Text(pDX, IDC_EDIT11, m_LeakeyCount);
	DDX_Text(pDX, IDC_EDIT12, m_ClearanceCount);
}

BEGIN_MESSAGE_MAP(CPistonRingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PLC32772, &CPistonRingDlg::OnPlcSetting)

END_MESSAGE_MAP()


// CPistonRingDlg ��Ϣ�������

//���ڳ�ʼ��
BOOL CPistonRingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ʼ�����ڵ�ԭʼ���ݣ�Ȼ�������ʾ
	oldFSP[0] = 1350;
	oldFSP[1] = 768;
	ShowWindow(SW_MAXIMIZE);

	//���ô�������
	MyEditFontSet();

	//ͨѶ��ʼ��
	g_CommXDM.OpenMyComm(1);
	m_maskImage = cvLoadImage("00.bmp", 0);
	
	//�����ʼ��
	g_CameraLeakey.CameraInit("192.168.3.100", 2000, 20, 1600, 1200);
	SetTimer(1, 50, NULL);
	g_CameraLocation.CameraInit("192.168.2.100", 2000, 60, 640, 480);
	SetTimer(2, 50, NULL);
	g_CameraClearance.CameraInit("192.168.4.100", 3000, 10, 2500, 1900);
	SetTimer(3, 100, NULL);

	//����ͨѶ����߳�


	//�Ի���仯����
	ResizeControl = false;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPistonRingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPistonRingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPistonRingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPistonRingDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}

void CPistonRingDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}

//���ڹر���Ϣ
void CPistonRingDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (MessageBox(L"Ӧ�ó��򼴽��رգ���ȷ�ϣ�", L"�˳�", MB_OKCANCEL))
	{
	case 1:
		KillTimer(1);
		g_Running = false;
		Sleep(1000);

		g_CsCommXDM.Lock();
		g_CommXDM.WriteCommRelay(PLCM(302), 1);
		g_CsCommXDM.Unlock();

		cvReleaseImage(&m_maskImage);
		g_CommXDM.CloseMyComm();
		CDialogEx::OnClose();
	default:
		return;
	}

}

//��Ϣ���ͺ���
BOOL CPistonRingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return true;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//��ɫ����
HBRUSH CPistonRingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC1:
	case IDC_STATIC2:
	case IDC_STATIC3:
	case IDC_STATIC4:
	case IDC_STATIC5:
	case IDC_STATIC6:
	case IDC_STATIC7:
	case IDC_STATIC8:
		pDC->SetTextColor(RGB(0, 0, 255));		//��RGB��ı���ɫ
		DialogFontSetting(pDC, 140);
		break;
	case IDC_STATIC025:
	case IDC_STATIC026:
	case IDC_STATIC027:		
		pDC->SetTextColor(RGB(255, 0, 0));		//��RGB��ı���ɫ
		DialogFontSetting(pDC, 400);
		break;	
	default:
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//�ߴ�����
void CPistonRingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  ��ȡ���ڱ仯�ı�����Ȼ���������ÿؼ��ĳߴ磬������С��ʱ������
	if (ResizeControl)
	{
		SetProcessDPIAware();
		int i = GetDeviceCaps(this->GetDC()->GetSafeHdc(), LOGPIXELSY);
		float f = 96.0 / i;
		if (nType != SIZE_MINIMIZED)
		{
			float fsp[2];
			fsp[0] = (float)cx / oldFSP[0] * f;
			fsp[1] = (float)cy / oldFSP[1] * f;
			oldFSP[0] = (float)cx;
			oldFSP[1] = (float)cy;

			CRect oldRect, rect;
			int woc;
			HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
			while (hwndChild)
			{
				woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
				GetDlgItem(woc)->GetWindowRect(oldRect);
				ScreenToClient(oldRect);
				rect.top = long(oldRect.top * fsp[1]);
				rect.bottom = long(oldRect.bottom * fsp[1]);
				rect.left = long(oldRect.left * fsp[0]);
				rect.right = long(oldRect.right * fsp[0]);

				GetDlgItem(woc)->MoveWindow(rect, TRUE);
				hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			}
		}
	}
}

//��ǩ�����С����
void CPistonRingDlg::DialogFontSetting(CDC* pDC, int nPointSet)
{
	mLableStateFont.CreatePointFont(nPointSet, L"����");
	pDC->SelectObject(&mLableStateFont);
	mLableStateFont.DeleteObject();
}

//����Edit����
void CPistonRingDlg::MyEditFontSet()
{
	mMyEditFont.CreatePointFont(250, L"����");
	GetDlgItem(IDC_EDIT1)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT2)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT3)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT4)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT5)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT10)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT11)->SetFont(&mMyEditFont);
	GetDlgItem(IDC_EDIT12)->SetFont(&mMyEditFont);
}

//��ʱ����Ӧ
void CPistonRingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	switch (nIDEvent)
	{
	case 1:	
		g_CameraLeakey.GrabImage();
		DrawPicToHDC(g_CameraLeakey.myImage, IDC_STATIC_CAMERA200);
		break;
	case 2:
		g_CameraLocation.GrabImage();
		DrawPicToHDC(g_CameraLocation.myImage, IDC_STATIC_CAMERA30);
		break;
	case 3:
		g_CameraClearance.GrabImage();
		DrawPicToHDC(g_CameraClearance.myImage, IDC_STATIC_CAMERA500);
		break;
	case 4:
		KillTimer(4);
		if (g_EMStop || g_gReset)
		{
			//����λ������ֹͣ����
			g_CommXDM.WriteCommRelay(PLCM(302), 1);
			break;
		}
		if ((!g_PositionOne) && (!g_PositionTwo) && (!g_PositionThree) && (!g_PositionFour))
		{
			g_CommXDM.WriteCommRelay(PLCM(302), 1);
			//����λ������ֹͣ����
			break;
		}
		if (g_Running)
		{
			if (g_PositionOne)
			{
				switch (g_ReadWord[4])
				{
				case 1:
					//��ת�����Ϻϸ�λ
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//��ת������©��λ
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//��ת�����ϼ�϶λ
					g_CommXDM.WriteCommRelay(PLCM(1210), 1);
					break;
				default:
					break;
				}
			}
			if (g_PositionTwo)
			{
				switch (g_ReadWord[3])
				{
				case 1:
					//��ת�����Ϻϸ�λ
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//��ת������©��λ
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//��ת�����ϼ�϶λ
					g_CommXDM.WriteCommRelay(PLCM(1210), 1);
					break;
				default:
					break;
				}
			}
			if (g_PositionThree)
			{
				switch (g_ReadWord[2])
				{
				case 1:
					//��ת�����Ϻϸ�λ
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//��ת������©��λ
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//��ת�����ϼ�϶λ
					g_CommXDM.WriteCommRelay(PLCM(1210), 1);
					break;
				default:
					break;
				}
			}
			if (g_PositionFour)
			{
				switch (g_ReadWord[1])
				{
				case 1:
					//��ת�����Ϻϸ�λ
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//��ת������©��λ
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//��ת�����ϼ�϶λ
					g_CommXDM.WriteCommRelay(PLCM(1210), 1);
					break;
				default:
					break;
				}
			}

		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPistonRingDlg::DrawPicToHDC(IplImage* scr, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	CvvImage cimg;
	cimg.CopyOf(scr);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

//PLC���ü��ֶ����ƽ���
void CPistonRingDlg::OnPlcSetting()
{
	// TODO:  �ڴ���������������
	myPlC = new PLCSetting(this);
	myPlC->Create(IDD_PLC, this);
	myPlC->ShowWindow(SW_SHOW);
}


//����ͨ���߳�
CWinThread* CPistonRingDlg::ThreadCreate(
	AFX_THREADPROC pfnThreadProc,
	SerialComm* pComm,
	CCriticalSection* pCsComm,
	CameraBasler* pCamera,
	CCriticalSection* pCsCamera,
	bool* pRun,
	DWORD time)
{
	ThreadParms* parms = new ThreadParms;
	parms->hWnd = m_hWnd;
	parms->pCamera = pCamera;
	parms->pComm = pComm;
	parms->pCsCamera = pCsCamera;
	parms->pCsComm = pCsComm;
	parms->pRun = pRun;
	parms->time = time;

	return AfxBeginThread(pfnThreadProc, (LPVOID)parms);
}

//���������ʼ���߳�
void CPistonRingDlg::CameraInitCreate(
	CameraBasler* pCamera,
	String_t cameraString,
	double exposureTime,
	double frameRate,
	int64_t width,
	int64_t height)
{
	CameraInitParams* parms = new CameraInitParams;
	parms->hWnd = m_hWnd;
	parms->pCamera = pCamera;
	parms->cameraString = cameraString;
	parms->exposureTime = exposureTime;
	parms->frameRate = frameRate;
	parms->width = width;
	parms->height = height;

}


//ͼƬ�ɼ��߳�
UINT ThreadCameraGrab(LPVOID pParam)
{
	ThreadParms* ptp = (ThreadParms*)pParam;
	HWND hWnd = ptp->hWnd;
	SerialComm& comm = *(ptp->pComm);
	CCriticalSection& csComm = *(ptp->pCsComm);
	CameraBasler& camera = *(ptp->pCamera);
	CCriticalSection& csCamera = *(ptp->pCsCamera);
	bool& run = *(ptp->pRun);
	DWORD time = ptp->time;
	delete ptp;

	bool grab = false;
	while (run)
	{
		csCamera.Lock();
		grab = camera.GrabImage();
		csCamera.Unlock();
		if (grab)
		{
			PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
		}
		Sleep(time);
	}

	return 0;
}

//©�����߳�
UINT ThreadCheckLeakey(LPVOID pParam)
{
	ThreadParms* ptp = (ThreadParms*)pParam;
	HWND hWnd = ptp->hWnd;
	SerialComm& comm = *(ptp->pComm);
	CCriticalSection& csComm = *(ptp->pCsComm);
	CameraBasler& camera = *(ptp->pCamera);
	CCriticalSection& csCamera = *(ptp->pCsCamera);
	bool& run = *(ptp->pRun);
	delete ptp;
	
	bool flag = false;
	bool check = false;
	int result = 0;
	while (run)
	{
		do
		{
			csComm.Lock();
			comm.ReadCommRelay(PLCM(1526), 1, &flag);
			csComm.Unlock();
			if (!run) return 1;
			Sleep(10);
		} while (!flag);

		csComm.Lock();
		comm.WriteCommRelay(PLCM(1520), 1);
		csComm.Unlock();
		do
		{
			csCamera.Lock();
			camera.GrabImage();
			IplImage* img = cvCreateImage(cvSize(camera.myImage->width, camera.myImage->height), IPL_DEPTH_8U, 1);
			memset(img->imageData, 0, img->height*img->widthStep*sizeof(unsigned char));
			cvCopy(camera.myImage, img);
			result = OnNewvision(img, m_maskImage);
			PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
			csCamera.Unlock();
			if (result == -1)
			{
				csComm.Lock();
				comm.WriteCommWord(PLCHD(1421), 2);
				comm.WriteCommRelay(PLCM(1426), 1);
				csComm.Unlock();
				break;
			}				
			csComm.Lock();
			comm.ReadCommRelay(PLCM(1527), 1, &check);
			csComm.Unlock();		
			if (!run) return 1;
		} while (!check);

		if (result != -1)
		{
			csComm.Lock();
			comm.WriteCommWord(PLCHD(1421), 1);
			comm.WriteCommRelay(PLCM(1426), 1);
			csComm.Unlock();
		}
		csComm.Lock();
		comm.ReadCommRelay(PLCM(351), 1, &run);
		csComm.Unlock();
	}

	return 0;
}

//��λ�߳�
UINT ThreadCheakLocation(LPVOID pParam)
{
	ThreadParms* ptp = (ThreadParms*)pParam;
	HWND hWnd = ptp->hWnd;
	SerialComm& comm = *(ptp->pComm);
	CCriticalSection& csComm = *(ptp->pCsComm);
	CameraBasler& camera = *(ptp->pCamera);
	CCriticalSection& csCamera = *(ptp->pCsCamera);
	bool& run = *(ptp->pRun);
	DWORD time = ptp->time;
	delete ptp;
	
	//��ȡ��λ����ʼ��λ�ź�PLCM(1536)
	bool flag = false;
	//�����ж��Ƿ��м�϶λ�ã����м�϶λ�ã����趨��϶λ�ã�������λ�����Ϳ�ʼ���϶��Ϣ��
	//���û�м�϶λ�ã��趨����֮һλ�ƣ��ȴ���λ��ɣ��ٴμ�����ռ���϶λ�ã�
	bool check = false;
	bool position = false;
	int result = 0;
	while (run)
	{
		do
		{
			csComm.Lock();
			comm.ReadCommRelay(PLCM(1536), 1, &flag);
			csComm.Unlock();
			if (!run) return 1;
			Sleep(10);
		} while (!flag);
		//���ռ���϶��λ��Ϣ���������������
		csCamera.Lock();
		camera.GrabImage();
		PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
		csCamera.Unlock();
		//����λ�����ͼ�������������Ƿ���Ҫ�ٴμ���־��
		csComm.Lock();
		comm.WriteCommDword(PLCHD(1434), 100000);
		comm.WriteCommRelay(PLCM(1530), 1);
		csComm.Unlock();
		check = true;
		if (check)
		{
			do
			{
				csComm.Lock();
				comm.ReadCommRelay(PLCM(1537), 1, &position);
				csComm.Unlock();
				if (!run) return 1;
				Sleep(10);
			} while (!position);
			//�ٴμ���϶λ��
			csCamera.Lock();
			camera.GrabImage();
			PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
			csCamera.Unlock();
			//���ü�϶��λ���
			csComm.Lock();
			comm.WriteCommRelay(PLCM(1537), 0);
			comm.WriteCommDword(PLCHD(1434), 100000);
			comm.WriteCommRelay(PLCM(1530), 1);
			csComm.Unlock();
		}
		//�ȴ���λ����λ��ɣ�
		do
		{
			csComm.Lock();
			comm.ReadCommRelay(PLCM(1537), 1, &position);
			csComm.Unlock();
			if (!run) return 1;
			Sleep(10);
		} while (!position);

		//���Ϳ�ʼ����϶��Ϣ��

		//�ж��Ƿ���Ҫ��������
		csComm.Lock();
		comm.ReadCommRelay(PLCM(351), 1, &run);
		csComm.Unlock();
		Sleep(800);
	}

	return 0;
}

//��϶����߳�
UINT ThreadCheakClearance(LPVOID pParam)
{
	ThreadParms* ptp = (ThreadParms*)pParam;
	HWND hWnd = ptp->hWnd;
	SerialComm& comm = *(ptp->pComm);
	CCriticalSection& csComm = *(ptp->pCsComm);
	CameraBasler& camera = *(ptp->pCamera);
	CCriticalSection& csCamera = *(ptp->pCsCamera);
	bool& run = *(ptp->pRun);
	DWORD time = ptp->time;
	delete ptp;
	
	//���ղ����տڼ�϶
	csCamera.Lock();
	camera.GrabImage();
	PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
	csCamera.Unlock();
	//���ü����
	csComm.Lock();
	comm.WriteCommWord(PLCHD(1431), 1);
	comm.WriteCommRelay(PLCM(1436), 1);
	comm.ReadCommRelay(PLCM(351), 1, &run);
	csComm.Unlock();


	return 0;
}

UINT ThreadRunState(LPVOID pParam)
{
	bool& run = *((bool*)pParam);

	while (run)
	{
		g_CsCommXDM.Lock();
		g_CommXDM.ReadCommRelay(PLCM(2000), 20, g_ReadBool);
		g_CommXDM.ReadCommWord(PLCHD(1401), 4, g_ReadWord);
		g_CsCommXDM.Unlock();
		Sleep(10);
	}
	return 0;
}
