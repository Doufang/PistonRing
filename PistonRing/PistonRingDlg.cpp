
// PistonRingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PistonRing.h"
#include "PistonRingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//窗口初始化标志，初始化完成后设置为false。
BOOL ResizeControl = true;

//漏光相机
CameraBasler g_CameraLeakey;
CCriticalSection g_CsLeakey;
//定位相机
CameraBasler g_CameraLocation;
CCriticalSection g_CsLocation;
//间隙检测相机
CameraBasler g_CameraClearance;
CCriticalSection g_CsClearance;
//PLC通讯口
SerialComm g_CommXDM;
CCriticalSection g_CsCommXDM;
//

//下位机通讯状态定义
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


//掩码图片
IplImage* m_maskImage = NULL;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPistonRingDlg 对话框



CPistonRingDlg::CPistonRingDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CPistonRingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPistonRingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, g_CommXDM.myComm);

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

	ON_MESSAGE(WM_USER_GRAB_FINISHED, &CPistonRingDlg::OnUserGrabFished)
	ON_MESSAGE(WM_USER_START_CHECK_LEAKEY, &CPistonRingDlg::OnUserStratCheckLeakey)
	ON_MESSAGE(WM_USER_LEAKEY_FINISHED, &CPistonRingDlg::OnUserLeakeyFished)
	ON_MESSAGE(WM_USER_START_LOCATION, &CPistonRingDlg::OnUserStartLocation)
	ON_MESSAGE(WM_USER_LOCATION_FINISHED, &CPistonRingDlg::OnUserLocationFished)
	ON_MESSAGE(WM_USER_START_CLEARANCE, &CPistonRingDlg::OnUserStartClearance)
	ON_MESSAGE(WM_USER_CLEARANCE_FINISHED, &CPistonRingDlg::OnUserClearanceFished)	
	//ON_MESSAGE(WM_USER_COMM_FINISHED, &CPistonRingDlg::CommFished)
	//ON_MESSAGE(WM_USER_COMM_ERROR, &CPistonRingDlg::CommError)
END_MESSAGE_MAP()


// CPistonRingDlg 消息处理程序

//窗口初始化
BOOL CPistonRingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//初始化窗口的原始数据，然后最大化显示
	oldFSP[0] = 1350;
	oldFSP[1] = 768;
	ShowWindow(SW_MAXIMIZE);
	MyEditFontSet();

	g_CommXDM.OpenMyComm(1);
	m_maskImage = cvLoadImage("00.bmp", 0);
	
	g_CameraLeakey.CameraInit("192.168.3.100", 2000, 20, 1600, 1200);
	SetTimer(1, 50, NULL);
	g_CameraLocation.CameraInit("192.168.2.100", 2000, 60, 640, 480);
	SetTimer(2, 50, NULL);
	g_CameraClearance.CameraInit("192.168.4.100", 3000, 10, 2500, 1900);
	SetTimer(3, 100, NULL);

	m_commRunning = true;
	m_tRunState = AfxBeginThread(ThreadRunState, (LPVOID)(&m_commRunning));


	ResizeControl = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPistonRingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPistonRingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPistonRingDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}

void CPistonRingDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

//窗口关闭消息
void CPistonRingDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (MessageBox(L"应用程序即将关闭，请确认！", L"退出", MB_OKCANCEL))
	{
	case 1:
		KillTimer(1);
		g_Running = false;
		m_commRunning = false;
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

//消息传送函数
BOOL CPistonRingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
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


//颜色设置
HBRUSH CPistonRingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC020:
	case IDC_STATIC021:
	case IDC_STATIC022:
	case IDC_STATIC023:
	case IDC_STATIC024:
		pDC->SetTextColor(RGB(0, 0, 255));		//用RGB宏改变颜色
		DialogFontSetting(pDC, 250);
		break;
	case IDC_STATIC025:
	case IDC_STATIC026:
	case IDC_STATIC027:		
		pDC->SetTextColor(RGB(255, 0, 0));		//用RGB宏改变颜色
		DialogFontSetting(pDC, 400);
		break;
	
	default:
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//尺寸设置
void CPistonRingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  获取窗口变化的比例，然后重新设置控件的尺寸，并且最小化时不设置
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
			HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
			while (hwndChild)
			{
				woc = ::GetDlgCtrlID(hwndChild);//取得ID  
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

//字体设置
void CPistonRingDlg::DialogFontSetting(CDC* pDC, int nPointSet)
{
	mLableStateFont = new CFont;
	mLableStateFont->CreatePointFont(nPointSet, L"宋体");
	pDC->SelectObject(mLableStateFont);
	mLableStateFont->DeleteObject();
}

void CPistonRingDlg::MyEditFontSet()
{
	mMyEditFont.CreatePointFont(250, L"宋体");
}

//定时器响应
void CPistonRingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
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
			//向下位机发送停止命令
			g_CommXDM.WriteCommRelay(PLCM(302), 1);
			break;
		}
		if ((!g_PositionOne) && (!g_PositionTwo) && (!g_PositionThree) && (!g_PositionFour))
		{
			g_CommXDM.WriteCommRelay(PLCM(302), 1);
			//向下位机发送停止命令
			break;
		}
		if (g_Running)
		{
			if (g_PositionOne)
			{
				switch (g_ReadWord[4])
				{
				case 1:
					//旋转到下料合格位
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//旋转到下料漏光位
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//旋转到下料间隙位
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
					//旋转到下料合格位
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//旋转到下料漏光位
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//旋转到下料间隙位
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
					//旋转到下料合格位
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//旋转到下料漏光位
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//旋转到下料间隙位
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
					//旋转到下料合格位
					g_CommXDM.WriteCommRelay(PLCM(1203), 1);
					break;
				case 2:
					//旋转到下料漏光位
					g_CommXDM.WriteCommRelay(PLCM(1206), 1);
					break;
				case 3:
					//旋转到下料间隙位
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

//PLC设置及手动控制界面
void CPistonRingDlg::OnPlcSetting()
{
	// TODO:  在此添加命令处理程序代码
	myPlC = new PLCSetting(this);
	myPlC->Create(IDD_PLC, this);
	myPlC->ShowWindow(SW_SHOW);
}


//创建通用线程
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

//创建相机初始化线程
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

//图片采集完成消息
LRESULT CPistonRingDlg::OnUserGrabFished(WPARAM wParam, LPARAM lParam)
{
	CameraBasler& camera = *((CameraBasler*)lParam);
	switch (camera.cameraWidth)
	{
	case 640:
		g_CsLocation.Lock();
		DrawPicToHDC(g_CameraLocation.myImage, IDC_STATIC_CAMERA30);
		g_CsLocation.Unlock();
		break;
	case 1600:
		g_CsLeakey.Lock();
		DrawPicToHDC(g_CameraLeakey.myImage, IDC_STATIC_CAMERA200);
		g_CsLeakey.Unlock();
		break;
	case 2500:
		g_CsClearance.Lock();
		DrawPicToHDC(g_CameraClearance.myImage, IDC_STATIC_CAMERA500);
		g_CsClearance.Unlock();
		break;
	default:
		break;
	}
	return 0;
}

//开始检漏光消息
LRESULT CPistonRingDlg::OnUserStratCheckLeakey(WPARAM wParam, LPARAM lParam)
{

	ThreadCreate(ThreadCheckLeakey, &g_CommXDM, &g_CsCommXDM,
		&g_CameraLeakey, &g_CsLeakey, &g_Running, 100);

	return 0;
}

//漏光检测完成消息
LRESULT CPistonRingDlg::OnUserLeakeyFished(WPARAM wParam, LPARAM lParam)
{
	int n = (int)wParam;

	return 0;
}

//开始定位消息
LRESULT CPistonRingDlg::OnUserStartLocation(WPARAM wParam, LPARAM lParam)
{
	
	ThreadCreate(ThreadCheakLocation, &g_CommXDM, &g_CsCommXDM,
		&g_CameraLocation, &g_CsLocation, &g_Running);
	
	return 0;
}

//定位完成消息
LRESULT CPistonRingDlg::OnUserLocationFished(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

//开始检测间隙消息
LRESULT CPistonRingDlg::OnUserStartClearance(WPARAM wParam, LPARAM lParam)
{
	
	ThreadCreate(ThreadCheakClearance, &g_CommXDM, &g_CsCommXDM,
		&g_CameraClearance, &g_CsClearance, &g_Running);
	
	return 0;
}

//间隙检测完成消息
LRESULT CPistonRingDlg::OnUserClearanceFished(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}


//UINT ThreadCheckLeakey(LPVOID pParam)
//{
//	ThreadParms* ptp = (ThreadParms*)pParam;
//	HWND m_hWnd = ptp->hWnd;
//	delete ptp;
//
//	uint16_t n = 0;
//	int result = 0;
//	while (g_Running)
//	{
//		while (g_Running)
//		{
//			g_CsCommXDM.Lock();
//			g_CommXDM.ReadCommWord(PLCHD(1420), 1, &n);
//			g_CsCommXDM.Unlock();
//			if (n == 1)
//			{
//				g_CsCommXDM.Lock();
//				g_CommXDM.WriteCommRelay(PLCM(1520), 1);
//				g_CsCommXDM.Unlock();
//				break;
//			}
//		}
//
//		bool b = false;
//		do
//		{
//			g_CsLeakey.Lock();
//			g_CameraLeakey.GrabImage();
//			IplImage* img = cvCreateImage(cvSize(g_CameraLeakey.myImage->width, g_CameraLeakey.myImage->height), IPL_DEPTH_8U, 1);
//			memset(img->imageData, 0, img->height*img->widthStep*sizeof(unsigned char));
//			cvCopy(g_CameraLeakey.myImage, img);
//			result = OnNewvision(img, m_maskImage);
//			PostMessage(m_hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&g_CameraLeakey));
//			g_CsLeakey.Unlock();
//			if (result == -1)
//			{
//				g_CsCommXDM.Lock();
//				g_CommXDM.WriteCommWord(PLCHD(1421), 2);
//				g_CsCommXDM.Unlock();
//				break;
//			}
//
//			g_CsCommXDM.Lock();
//			g_CommXDM.ReadCommRelay(PLCM(1425), 1, &b);
//			g_CsCommXDM.Unlock();
//		} while (!b);
//
//		g_CsCommXDM.Lock();
//		g_CommXDM.WriteCommRelay(PLCM(1426), 1);
//		g_CsCommXDM.Unlock();
//
//		if (!g_AutoRun)
//		{
//			break;
//		}
//	}
//
//	return 0;
//}

//图片采集线程
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

//漏光检测线程
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
		PostMessage(hWnd, WM_USER_LEAKEY_FINISHED, (WPARAM)result, 0);
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

//定位线程
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
	
	//读取下位机开始定位信号PLCM(1536)
	bool flag = false;
	//拍照判断是否有间隙位置，若有间隙位置，则设定间隙位置，开启定位，发送开始检间隙消息；
	//如果没有间隙位置，设定三分之一位移，等待定位完成，再次检查拍照检测间隙位置；
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
		//拍照检测间隙定位信息，并给出检测结果。
		csCamera.Lock();
		camera.GrabImage();
		PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
		csCamera.Unlock();
		//给下位机发送检测结果，并设置是否需要再次检测标志。
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
			//再次检测间隙位置
			csCamera.Lock();
			camera.GrabImage();
			PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
			csCamera.Unlock();
			//设置间隙定位结果
			csComm.Lock();
			comm.WriteCommRelay(PLCM(1537), 0);
			comm.WriteCommDword(PLCHD(1434), 100000);
			comm.WriteCommRelay(PLCM(1530), 1);
			csComm.Unlock();
		}
		//等待下位机定位完成，
		do
		{
			csComm.Lock();
			comm.ReadCommRelay(PLCM(1537), 1, &position);
			csComm.Unlock();
			if (!run) return 1;
			Sleep(10);
		} while (!position);
		PostMessage(hWnd, WM_USER_LOCATION_FINISHED, 0, 0);
		//发送开始检测间隙消息。
		PostMessage(hWnd, WM_USER_START_CLEARANCE, 0, 0);
		//判断是否需要继续运行
		csComm.Lock();
		comm.ReadCommRelay(PLCM(351), 1, &run);
		csComm.Unlock();
		Sleep(800);
	}

	return 0;
}

//间隙检测线程
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
	
	//拍照测量闭口间隙
	csCamera.Lock();
	camera.GrabImage();
	PostMessage(hWnd, WM_USER_GRAB_FINISHED, 0, (LPARAM)(&camera));
	csCamera.Unlock();
	//设置检测结果
	csComm.Lock();
	comm.WriteCommWord(PLCHD(1431), 1);
	comm.WriteCommRelay(PLCM(1436), 1);
	comm.ReadCommRelay(PLCM(351), 1, &run);
	csComm.Unlock();
	PostMessage(hWnd, WM_USER_CLEARANCE_FINISHED, 0, 0);

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
