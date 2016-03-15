
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
, mValueFeedingOKNumber(0)
, mValueFeedingNGNumber(0)
, mValueClearanceOKNumber(0)
, mValueClearanceNGNumber(0)
, m_LeakyCheckResult(_T("����"))
, m_PositionResult(_T("����"))
, m_ClearanceCheckResult(_T("����"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPistonRingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, mValueFeedingSystemSwitch);
	DDV_MinMaxInt(pDX, mValueFeedingSystemSwitch, 0, 1);
	DDX_Radio(pDX, IDC_RADIO3, mValueLeakySystemSwitch);
	DDV_MinMaxInt(pDX, mValueLeakySystemSwitch, 0, 1);
	DDX_Radio(pDX, IDC_RADIO5, mValueClearanceSystemSwitch);
	DDV_MinMaxInt(pDX, mValueClearanceSystemSwitch, 0, 1);
	DDX_Radio(pDX, IDC_RADIO7, mValueUnloadingSystemSwith);
	DDV_MinMaxInt(pDX, mValueUnloadingSystemSwith, 0, 1);
	DDX_Radio(pDX, IDC_RADIO9, mValueRotarySystemSwitch);
	DDV_MinMaxInt(pDX, mValueRotarySystemSwitch, 0, 1);
	DDX_Text(pDX, IDC_EDIT1, mValueFeedingOKNumber);
	DDX_Text(pDX, IDC_EDIT2, mValueFeedingNGNumber);
	DDX_Text(pDX, IDC_EDIT3, mValueClearanceOKNumber);
	DDX_Text(pDX, IDC_EDIT4, mValueClearanceNGNumber);
	DDX_Control(pDX, IDC_EDIT1, mControlFeedingOKNumber);
	DDX_Control(pDX, IDC_EDIT2, mControlFeedingNGNumber);
	DDX_Control(pDX, IDC_EDIT3, mControlClearanceOKNumber);
	DDX_Control(pDX, IDC_EDIT4, mControlClearanceNGNumber);
	DDX_Control(pDX, IDC_MSCOMM1, g_CommXDM.myComm);
	DDX_Text(pDX, IDC_STATIC025, m_LeakyCheckResult);
	DDX_Text(pDX, IDC_STATIC026, m_PositionResult);
	DDX_Text(pDX, IDC_STATIC027, m_ClearanceCheckResult);
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
	ON_BN_CLICKED(IDC_RADIO2, &CPistonRingDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CPistonRingDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CPistonRingDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO3, &CPistonRingDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &CPistonRingDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO5, &CPistonRingDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO8, &CPistonRingDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO7, &CPistonRingDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO10, &CPistonRingDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO9, &CPistonRingDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_BUTTON1, &CPistonRingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPistonRingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPistonRingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPistonRingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON9, &CPistonRingDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CPistonRingDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CPistonRingDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CPistonRingDlg::OnBnClickedButton12)
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


// CPistonRingDlg ��Ϣ��������

//���ڳ�ʼ��
BOOL CPistonRingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

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

// �����Ի���������С����ť������Ҫ����Ĵ���
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
	// TODO:  �ڴ�����ר�ô����/����û���

	CDialogEx::OnOK();
}

void CPistonRingDlg::OnCancel()
{
	// TODO:  �ڴ�����ר�ô����/����û���

	CDialogEx::OnCancel();
}

//���ڹر���Ϣ
void CPistonRingDlg::OnClose()
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
	switch (MessageBox(L"Ӧ�ó��򼴽��رգ���ȷ�ϣ�", L"�˳�", MB_OKCANCEL))
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

//��Ϣ���ͺ���
BOOL CPistonRingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ�����ר�ô����/����û���
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
	case IDC_STATIC020:
	case IDC_STATIC021:
	case IDC_STATIC022:
	case IDC_STATIC023:
	case IDC_STATIC024:
		pDC->SetTextColor(RGB(0, 0, 255));		//��RGB��ı���ɫ
		DialogFontSetting(pDC, 250);
		break;
	case IDC_STATIC025:
	case IDC_STATIC026:
	case IDC_STATIC027:		
		pDC->SetTextColor(RGB(255, 0, 0));		//��RGB��ı���ɫ
		DialogFontSetting(pDC, 400);
		break;
	case IDC_BUTTON1:
		
		break;
	case IDC_BUTTON2:

		break;
	case IDC_BUTTON3:

		break;
	case IDC_BUTTON4:

		break;
	case IDC_BUTTON9:

		break;
	case IDC_BUTTON10:

		break;
	case IDC_BUTTON11:

		break;
	case IDC_BUTTON12:

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

//��������
void CPistonRingDlg::DialogFontSetting(CDC* pDC, int nPointSet)
{
	mLableStateFont = new CFont;
	mLableStateFont->CreatePointFont(nPointSet, L"����");
	pDC->SelectObject(mLableStateFont);
	mLableStateFont->DeleteObject();
}

void CPistonRingDlg::MyEditFontSet()
{
	mMyEditFont.CreatePointFont(250, L"����");
	mControlFeedingOKNumber.SetFont(&mMyEditFont);
	mControlFeedingNGNumber.SetFont(&mMyEditFont);
	mControlClearanceOKNumber.SetFont(&mMyEditFont);
	mControlClearanceNGNumber.SetFont(&mMyEditFont);
}

//��ʱ����Ӧ
void CPistonRingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ	
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
	// TODO:  �ڴ�����������������
	myPlC = new PLCSetting(this);
	myPlC->Create(IDD_PLC, this);
	myPlC->ShowWindow(SW_SHOW);
}

//����ϵͳ��
void CPistonRingDlg::OnBnClickedRadio2(){}

//����ϵͳ�ر�
void CPistonRingDlg::OnBnClickedRadio1(){}


//©�����
void CPistonRingDlg::OnBnClickedRadio4(){}

//©����ر�
void CPistonRingDlg::OnBnClickedRadio3(){}

//��϶����
void CPistonRingDlg::OnBnClickedRadio6(){}

//��϶���ر�
void CPistonRingDlg::OnBnClickedRadio5(){}

//����ϵͳ��
void CPistonRingDlg::OnBnClickedRadio8(){}

//����ϵͳ�ر�
void CPistonRingDlg::OnBnClickedRadio7(){}

//��ת̨��
void CPistonRingDlg::OnBnClickedRadio10(){}

//��ת̨�ر�
void CPistonRingDlg::OnBnClickedRadio9(){}

//�ֶ���ť
void CPistonRingDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	g_AutoRun = false;
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(300), 0);
	g_CsCommXDM.Unlock();
}

//�Զ���ť
void CPistonRingDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	g_AutoRun = true;
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(300), 1);
	g_CsCommXDM.Unlock();
}

//���а�ť
void CPistonRingDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);

	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(301), 1);
	g_CsCommXDM.Unlock();
	SetTimer(4, 100, NULL);
	//if (g_Running)
	//{
	//	PostMessage(WM_USER_START_CHECK_LEAKEY, 0, 0);
	//	PostMessage(WM_USER_START_LOCATION, 0, 0);
	//}
}

//ֹͣ��ť
void CPistonRingDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(302), 1);
	g_CsCommXDM.Unlock();
	g_Running = false;
	
	SetTimer(1, 50, NULL);
	SetTimer(2, 50, NULL);
	SetTimer(3, 100, NULL);
}

//��ͣ��ť
void CPistonRingDlg::OnBnClickedButton9(){}

//������ť
void CPistonRingDlg::OnBnClickedButton10(){}

//��ͣ��ť
void CPistonRingDlg::OnBnClickedButton11()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	g_Running = false;
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(305), 1);
	g_CsCommXDM.Unlock();
}

//��λ��ť
void CPistonRingDlg::OnBnClickedButton12()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(306), 1);
	g_CommXDM.WriteCommRelay(PLCM(306), 0);
	g_CsCommXDM.Unlock();
}

//UINT ThreadComm(LPVOID pParam)
//{
//	ThreadParmsComm* ptp = (ThreadParmsComm*)pParam;
//	HWND m_hWnd = ptp->hWnd;
//	SerialComm* comm = ptp->comm;
//	bool& Running = *(ptp->running);
//	CCriticalSection* pCriticalSection = ptp->pCS;
//	delete ptp;
//	
//	bool read1 = false;
//	bool read2 = false;
//	bool read3 = false;
//	while (Running)
//	{
//		pCriticalSection->Lock();
//		read1 = comm->ReadCommRelay(PLCM(1000), 40, g_ReadBool);
//		read2 = comm->ReadCommWord(PLCD(0), 20, g_ReadWord);
//		read3 = comm->ReadCommDword(PLCD(100), 20, g_ReadDword);
//		pCriticalSection->Unlock();
//		if (read1 || read2 || read3)
//		{
//			PostMessage(m_hWnd, WM_USER_COMM_FINISHED, 0, 0);
//		}
//		if (!(read1 && read2 && read3))
//		{
//			PostMessage(m_hWnd, WM_USER_COMM_ERROR, 0, 0);
//		}
//		read1 = read2 = read3 = false;
//		Sleep(1000);
//	}
//	return 0;
//}

//LRESULT CPistonRingDlg::CommFished(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
//
//LRESULT CPistonRingDlg::CommError(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


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

//ͼƬ�ɼ������Ϣ
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

//��ʼ��©����Ϣ
LRESULT CPistonRingDlg::OnUserStratCheckLeakey(WPARAM wParam, LPARAM lParam)
{

	ThreadCreate(ThreadCheckLeakey, &g_CommXDM, &g_CsCommXDM,
		&g_CameraLeakey, &g_CsLeakey, &g_Running, 100);

	return 0;
}

//©���������Ϣ
LRESULT CPistonRingDlg::OnUserLeakeyFished(WPARAM wParam, LPARAM lParam)
{
	int n = (int)wParam;

	return 0;
}

//��ʼ��λ��Ϣ
LRESULT CPistonRingDlg::OnUserStartLocation(WPARAM wParam, LPARAM lParam)
{
	
	ThreadCreate(ThreadCheakLocation, &g_CommXDM, &g_CsCommXDM,
		&g_CameraLocation, &g_CsLocation, &g_Running);
	
	return 0;
}

//��λ�����Ϣ
LRESULT CPistonRingDlg::OnUserLocationFished(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

//��ʼ����϶��Ϣ
LRESULT CPistonRingDlg::OnUserStartClearance(WPARAM wParam, LPARAM lParam)
{
	
	ThreadCreate(ThreadCheakClearance, &g_CommXDM, &g_CsCommXDM,
		&g_CameraClearance, &g_CsClearance, &g_Running);
	
	return 0;
}

//��϶��������Ϣ
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
		PostMessage(hWnd, WM_USER_LOCATION_FINISHED, 0, 0);
		//���Ϳ�ʼ����϶��Ϣ��
		PostMessage(hWnd, WM_USER_START_CLEARANCE, 0, 0);
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