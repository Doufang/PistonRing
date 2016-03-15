
// MFCTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTest.h"
#include "MFCTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct tagTHREADPARMS
{
	HWND hWnd;					//���ڶ��߳���Ϣ����
	bool* running;
	CCriticalSection* pCriticalSection;
}CommParms;

//©��������߳���ض���
UINT ThreadComm(LPVOID pParam);
bool readBool[40];
WORD readWord[20];
DWORD readDword[10];
SerialComm mySerialPort;

// CMFCTestDlg �Ի���



CMFCTestDlg::CMFCTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, mySerialPort.myComm);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_USER_COMM_FINISHED, &CMFCTestDlg::CommFished)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCTestDlg ��Ϣ�������

BOOL CMFCTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	mySerialPort.OpenMyComm(2);

	Running = true;

	ThreadCommRelay* commRelay = new ThreadCommRelay;
	commRelay->hWnd = m_hWnd;
	commRelay->running = &Running;
	commRelay->pCS = &m_csComm;
	commRelay->comm = &mySerialPort;
	commRelay->address = 0;
	commRelay->number = 1;
	commRelay->back = &Running;
	commRelay->sleepTime = 20;
	m_threadComm = AfxBeginThread(ThreadCommReadRelay, commRelay, 0, 0, CREATE_SUSPENDED);
	if (mySerialPort.IsOpen())
	{
		m_threadComm->ResumeThread();
	}
	m_threadComm->ResumeThread();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCTestDlg::OnPaint()
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
HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCTestDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//char FilePath[1024] = { '\0' }, FolderPath[1024] = "E:\\wuxi\\leak";
	int hIndex = 0, wIndex = 0, ImagePosition = 0, TempPosition = 0, colorValue = 0;
	bool leakLight = false;	int LeakLightNum = 5;

	//sprintf_s(FilePath, "%s\\7.bmp", FolderPath);

	IplImage* currImageBefore = cvLoadImage("00.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	if (currImageBefore == NULL)
		return ;

	//cvShowImage("currImageBefore",currImageBefore);
	//cvWaitKey(0);

	//���ͼ���г���̫������İ�ͼ��������û�й���
	//����ͼ��75%���ǰ�ͼ�򷵻�

	//sprintf_s(FilePath, "%s\\NewOne.bmp", FolderPath);
	IplImage* maskImage = cvLoadImage("NewOne.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	//����mask��ʾ����
	//int bwPosition = 0;
	//for (int hIndex = 0; hIndex < maskImage->height; hIndex++)
	//{
	//	for (int wIndex = 0; wIndex < maskImage->width; wIndex++)
	//	{
	//		bwPosition = hIndex*maskImage->widthStep + wIndex;
	//		if( maskImage->imageData[bwPosition] != 255)
	//			maskImage->imageData[bwPosition] = 255;
	//		else
	//			maskImage->imageData[bwPosition] = 0;
	//	}
	//}


	if (maskImage == NULL)
		return ;

	cvShowImage("maskImage", maskImage);
	cvWaitKey(0);

	IplImage* currImage = cvCreateImage(cvSize(currImageBefore->width, currImageBefore->height), IPL_DEPTH_8U, 1);
	memset(currImage->imageData, 0, currImage->height*currImage->widthStep*sizeof(unsigned char));

	//ʹ��Mask
	cvCopy(currImageBefore, currImage, maskImage);

	//cvShowImage("currImage",currImage);
	//cvWaitKey(0);

	IplImage* EdgeImage = cvCreateImage(cvSize(currImageBefore->width, currImageBefore->height), IPL_DEPTH_8U, 1);
	memset(EdgeImage->imageData, 0, EdgeImage->height*EdgeImage->widthStep*sizeof(unsigned char));

	cvCanny(currImage, EdgeImage, 50, 180, 3);

	cvShowImage("EdgeImage", EdgeImage);
	cvWaitKey(0);


	int edgeTempPosition = 0;
	for (int hIndex = 1; hIndex < EdgeImage->height - 1; hIndex++)
	{
		for (int wIndex = 1; wIndex < EdgeImage->width - 1; wIndex++)
		{
			edgeTempPosition = hIndex*EdgeImage->widthStep + wIndex;
			if (EdgeImage->imageData[edgeTempPosition] == 255)
			if (maskImage->imageData[edgeTempPosition + 1] == 0
				|| maskImage->imageData[edgeTempPosition - 1] == 0
				|| maskImage->imageData[edgeTempPosition + maskImage->widthStep] == 0
				|| maskImage->imageData[edgeTempPosition - maskImage->widthStep] == 0)
				EdgeImage->imageData[edgeTempPosition] = 0;
		}
	}

	cvShowImage("EdgeImage2", EdgeImage);
	cvWaitKey(0);

	ListPoint pointSet; ListPoint bestPoint; ListPoint tempPoint;
	pointSet.Item = (ArrayPoint*)MallocArrayPoint();
	bestPoint.Item = (ArrayPoint*)MallocArrayPoint();
	tempPoint.Item = (ArrayPoint*)MallocArrayPoint();

	IplImage* markImage = cvCreateImage(cvGetSize(currImage), IPL_DEPTH_8U, 1);
	memset(markImage->imageData, 0, markImage->height*markImage->widthStep*sizeof(unsigned char));

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	ListRect rectList; ListInt intAreaList;
	rectList.Item = (ArrayRect*)MallocArrayRect();
	intAreaList.Item = (ArrayInt *)MallocArrayInt();

	ExtractAllEdgePointNumForItem(EdgeImage, markImage, cvRect(0, 0, currImageBefore->width, currImageBefore->height), 255, &pointSet);


	cvCircleObj TempCircle;

	memset(markImage->imageData, 0, markImage->height*markImage->widthStep*sizeof(unsigned char));

	RansacCirclePoint(pointSet, &bestPoint, &tempPoint);


	if (bestPoint.Item->size() == 0)
	{
		cvReleaseImage(&maskImage);
		cvReleaseImage(&markImage);
		cvReleaseImage(&currImage);
		cvReleaseImage(&EdgeImage);
		return ;
	}


	SortPointsListByXValue(&bestPoint);

	cvCircleObj outCircle;
	FitCircleObj(bestPoint, &outCircle);

	ListPoint pointOutCircleSet;
	pointOutCircleSet.Item = (ArrayPoint*)MallocArrayPoint();

	int radiusAdd = 0;
	int radiusMove = 35;

	for (int dIndex = 0; dIndex < VL_MAX(bestPoint.Item->size(), 0); dIndex++)
	{
		CvPoint TempPoint;
		TempPoint.x = ((*bestPoint.Item)[dIndex]).x;
		TempPoint.y = ((*bestPoint.Item)[dIndex]).y + radiusMove;
		AddArrayPoint(pointOutCircleSet.Item, TempPoint);

	}


	ListPoint pointMoreCircleSet;
	pointMoreCircleSet.Item = (ArrayPoint*)MallocArrayPoint();

	for (int wIndex = 0; wIndex < currImageBefore->width; wIndex++)
	{
		CvPoint TempPoint;
		CvPoint TempOutPoint;
		float x = 0, y = 0;
		x = wIndex - outCircle.CirclePoint.x;
		//y = dIndex - outCircle.CirclePoint.y;

		y = sqrt((outCircle.Radius + radiusMove) * (outCircle.Radius + radiusMove) - x * x);

		TempPoint.x = wIndex;
		if (outCircle.CirclePoint.y < 0)
			y = outCircle.CirclePoint.y + y;
		else
			y = outCircle.CirclePoint.y - y;

		TempPoint.y = y;
		AddArrayPoint(pointMoreCircleSet.Item, TempPoint);
	}

	SortPointsListByXValue(&pointMoreCircleSet);



	int maskCircleTemp = 0;

	for (int wIndex = 0; wIndex < markImage->width; wIndex++)
	{
		for (int hIndex = 0; hIndex < markImage->height; hIndex++)
		{
			maskCircleTemp = hIndex*markImage->widthStep + wIndex;

			if (hIndex <= ((*pointMoreCircleSet.Item)[wIndex]).y)
			{
				markImage->imageData[maskCircleTemp] = 255;
			}

		}
	}

	int a[4] = { 0 };

	//currImageBefore = cvLoadImage("E:\\wuxi\\leak\\1532.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	getMaxDistance(currImageBefore, &pointMoreCircleSet, a);

	int subPotion = 0;
	for (int hIndex = 0; hIndex < markImage->height; hIndex++)
	{
		for (int wIndex = 0; wIndex < markImage->width; wIndex++)
		{
			subPotion = hIndex*markImage->widthStep + wIndex;

			//currImageBefore->imageData[subPotion] -= maskCircle->imageData[subPotion];
			//if (maskCircle->imageData[subPotion] == 255)
			//	currImageBefore->imageData[subPotion] = 0;
			if (currImageBefore->imageData[subPotion] - markImage->imageData[subPotion] <= 0)
				currImageBefore->imageData[subPotion] = 0;

			if (currImageBefore->imageData[subPotion] - markImage->imageData[subPotion] >= 255)
				currImageBefore->imageData[subPotion] = 255;

		}
	}

	cvThreshold(currImageBefore, markImage, 110, 255, CV_THRESH_BINARY);

	cvShowImage("currbf", currImageBefore);
	cvWaitKey(0);


	CvPoint LineCenter;
	CvPoint LineCenter2;
	LineCenter2.x = (int)outCircle.CirclePoint.x;
	LineCenter2.y = (int)outCircle.CirclePoint.y;

	//LineCenter.x = (a[1]+a[0]/2);
	LineCenter.x = a[1] + a[0] / 2;
	LineCenter.y = a[3];


	CvSize imgSize = cvSize(currImageBefore->width, currImageBefore->height);

	CvRect Zone;
	Zone.x = 0;
	Zone.y = 0;
	Zone.width = 0;
	Zone.height = 0;

	bool Zone0 = false, Zone1 = false, Zone2 = false, Zone3 = false, Zone4 = false;
	//û�л������
	if (a[0] == 0)
	{
		if (getMinYPositon(&pointMoreCircleSet, 0, currImage->width, &Zone, a, imgSize, 0))
		{
			if (getMinYPositon(&pointMoreCircleSet, 0, currImageBefore->width, &Zone, a, imgSize, 0))
				Zone0 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum);
			//IplImage* ZoneImg = cvCreateImage(cvSize(Zone.width, Zone.height), IPL_DEPTH_8U, 1);
			//memset(ZoneImg->imageData, 0, ZoneImg->height*ZoneImg->widthStep*sizeof(unsigned char));

			//IplImage* ZoneImg2 = cvCreateImage(cvSize(Zone.width, Zone.height), IPL_DEPTH_8U, 1);
			//memset(ZoneImg2->imageData, 0, ZoneImg2->height*ZoneImg2->widthStep*sizeof(unsigned char));

			//cvSetImageROI(currImageBefore,Zone);

			//cvCopy(currImageBefore,ZoneImg);  

			//cvResetImageROI(currImageBefore); 

			//cvThreshold(ZoneImg, ZoneImg2, 110, 255, CV_THRESH_BINARY);	

			//int leakNum = 0;
			//int bwPosition = 0;
			//for (int hIndex = 0; hIndex < ZoneImg2->height; hIndex++)
			//{
			//	for (int wIndex = 0; wIndex < ZoneImg2->width; wIndex++)
			//	{
			//		bwPosition = hIndex*ZoneImg2->widthStep + wIndex;
			//		if( ZoneImg2->imageData[bwPosition] == 255)
			//			leakNum++;					
			//	}
			//}

			//if (leakNum > LeakLightNum)
			//{
			//	leakLight = true;
			//	//cvShowImage("ZoneImage2", ZoneImg2);
			//	//cvWaitKey(0);
			//}

			//cvReleaseImage(&ZoneImg);
		}
		//getMinYPositon(ListPoint* line, int firstPosition, int lastPosition, CvRect* zone, int* a, int direction)

	}
	else//�л������
	{
		//getMinYPositon
		if (a[1] > currImageBefore->width / 2)
		{
			if (getMinYPositon(&pointMoreCircleSet, a[2], currImageBefore->width, &Zone, a, imgSize, 4))
				Zone4 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum);

			if (getMinYPositon(&pointMoreCircleSet, 0, a[1], &Zone, a, imgSize, 3))
				Zone3 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum);
		}

		if (a[1] <= currImageBefore->width / 2)
		{
			if (getMinYPositon(&pointMoreCircleSet, 0, a[1], &Zone, a, imgSize, 1))
				Zone1 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum);

			if (getMinYPositon(&pointMoreCircleSet, a[2], currImageBefore->width, &Zone, a, imgSize, 2))
				Zone2 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum);
		}
	}

	//cvReleaseImage(&currImageBefore);
	cvReleaseImage(&maskImage);
	cvReleaseImage(&markImage);
	cvReleaseImage(&currImage);
	cvReleaseImage(&EdgeImage);

	bool lastResult = false;
	if (Zone0 || Zone1 || Zone2 || Zone3 || Zone4)
	{
		lastResult = true;
		return ;
	}

	lastResult = false;
	return ;
}

void JDDetect(IplImage* sourceImage)
{
	const int max_corners = 3;
	const char* filename = "JINGLONGLONG.jpg";
	int cornerCount = max_corners;//�ǵ�������Ŀ

	//���ڱ���ǵ������
	CvPoint2D32f corners[max_corners];

	IplImage*grayImage = 0, *corners1 = 0, *corners2 = 0;

	cvNamedWindow("IMAGE", CV_WINDOW_AUTOSIZE);
	//����ͼ�񣬼��
	//sourceImage=cvLoadImage(filename,1);
	grayImage = cvCreateImage(cvGetSize(sourceImage), IPL_DEPTH_8U, 1);

	//cvCvtColor(sourceImage,grayImage,CV_RGB2GRAY);
	grayImage = sourceImage;
	corners1 = cvCreateImage(cvGetSize(sourceImage), IPL_DEPTH_32F, 1);
	corners2 = cvCreateImage(cvGetSize(sourceImage), IPL_DEPTH_32F, 1);

	//void cvGoodFeaturesToTrack( 
	//	const CvArr* image,
	//	CvArr* eig_image, 
	//	CvArr* temp_image,
	//	CvPoint2D32f* corners,
	//	int* corner_count,
	//	double quality_level,
	//	double min_distance,
	//	const CvArr* mask=NULL
	//	int block_size   =3,
	//	int use_harris =0,
	//	double k = 0.4)
	cvGoodFeaturesToTrack(
		grayImage,          //	const CvArr* image,           
		corners1,           //	CvArr* eig_image,           
		corners2,			//	CvArr* temp_image,
		corners,            //	CvPoint2D32f* corners,        
		&cornerCount,       //	int* corner_count,          
		0.05,               //	double quality_level,    
		10,                  //	double min_distance,   
		0,                  //	const CvArr* mask=NULL
		3,					//	int block_size   =3,
		0,                  //	int use_harris =0,                                              
		0.4					//	double k = 0.4)
		);
	if (cornerCount)
	{
		int i = 0;
		CvScalar color = CV_RGB(0, 0, 255);
		for (i; i<cornerCount; i++)
			cvCircle(
			sourceImage,     //Ҫ��Բ��ͼ��
			cvPoint((int)(corners[i].x), (int)(corners[i].y)),           //Բ��
			6,   //�뾶
			color,//��ɫ
			2,    //ò�����߿�
			CV_AA,//����
			0    //Բ������ͺͰ뾶��С�������Ŀ
			);
	}
	cvShowImage("IMAGE", sourceImage);

	cvWaitKey(0);
	//�ͷ���Դ	
	cvReleaseImage(&corners1);
	cvReleaseImage(&corners2);
	cvReleaseImage(&grayImage);
	cvDestroyWindow("IMAGE");
}


void cvRefineForegroundMaskBySegm(CvSeq* segments, IplImage*  pFrImg)
{
	IplImage* tmp_image = cvCreateImage(cvSize(pFrImg->width, pFrImg->height), IPL_DEPTH_8U, 1);
	for (; segments; segments = ((CvSeq*)segments)->h_next)
	{
		CvSeq seq = *segments;
		seq.v_next = seq.h_next = NULL;
		cvZero(tmp_image);
		cvDrawContours(tmp_image, &seq, CV_RGB(0, 0, 255), CV_RGB(0, 0, 255), 10, -1);
		int num1 = cvCountNonZero(tmp_image);
		cvAnd(tmp_image, pFrImg, tmp_image);
		int num2 = cvCountNonZero(tmp_image);
		if (num2 > num1*0.5)
			cvDrawContours(pFrImg, &seq, CV_RGB(0, 0, 255), CV_RGB(0, 0, 255), 10, -1);
		else
			cvDrawContours(pFrImg, &seq, CV_RGB(0, 0, 0), CV_RGB(0, 0, 0), 10, -1);
	}
	cvReleaseImage(&tmp_image);
}
//���У�segments������ͼ��ָ��㷨�õ����������У���pFrImg����Ŀ�����㷨��⵽��ǰ��Ŀ�꣬�����Ϳ��Էָ�ͼ��Ľ������Ŀ��������������䡣���⣬������εõ��ָ�ͼ��Ҳ���Բ������º�����ʵ�֣�


CvSeq*cvSegmentFGMask(CvArr* _mask, int poly1Hull0, float perimScale, CvMemStorage* storage, CvPoint offset)
{
	CvMat mstub, *mask = cvGetMat(_mask, &mstub);
	CvMemStorage* tempStorage = storage ? storage : cvCreateMemStorage();
	CvSeq *contours, *c;
	int nContours = 0;
	CvContourScanner scanner;

	// clean up raw mask
	cvMorphologyEx(mask, mask, 0, 0, CV_MOP_OPEN, 1);
	cvMorphologyEx(mask, mask, 0, 0, CV_MOP_CLOSE, 1);
	// find contours around only bigger regions
	scanner = cvStartFindContours(mask, tempStorage,
		sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, offset);

	while ((c = cvFindNextContour(scanner)) != 0)
	{
		double len = cvContourPerimeter(c);
		double q = (mask->rows + mask->cols) / perimScale; // calculate perimeter len threshold
		if (len < q) //Get rid of blob if it's perimeter is too small
			cvSubstituteContour(scanner, 0);
		else //Smooth it's edges if it's large enough
		{
			CvSeq* newC;
			if (poly1Hull0) //Polygonal approximation of the segmentation 
				newC = cvApproxPoly(c, sizeof(CvContour), tempStorage, CV_POLY_APPROX_DP, 2, 0);
			else //Convex Hull of the segmentation
				newC = cvConvexHull2(c, tempStorage, CV_CLOCKWISE, 1);
			cvSubstituteContour(scanner, newC);
			nContours++;
		}
	}
	contours = cvEndFindContours(&scanner);
	// paint the found regions back into the image
	cvZero(mask);
	for (c = contours; c != 0; c = c->h_next)
		cvDrawContours(mask, c, cvScalarAll(255), cvScalarAll(0), -1, CV_FILLED, 8,
		cvPoint(-offset.x, -offset.y));
	if (tempStorage != storage)
	{
		cvReleaseMemStorage(&tempStorage);
		contours = 0;
	}
	return contours;
}
// _mask��һ����ֵͼ�񣬿�����Canny��Ե����cvThreshold�Ⱥ����õ��Ķ�ֵͼ��


void getMaxDistance(IplImage* img, ListPoint* line, int* a)
{
	int tempStart = 0, tempEnd = 0, tempDistance = 0;
	bool tempFlag = false;
	int wIndex = 0, hIndex = 0;

	for (wIndex = 30; wIndex < line->Item->size() - 1; wIndex++)
	{
		int i = (*line->Item)[wIndex].x;
		int j = (*line->Item)[wIndex].y;

		uchar* ptr = (uchar*)(img->imageData + j * img->widthStep + i);

		if (*ptr < 100 && tempFlag == false)
			continue;

		if (tempFlag == false)
		{
			tempStart = wIndex;
			tempFlag = true;
		}
		//(*PointList->Item)[hIndex]
		//currImageBefore


		if (*ptr > 190 && tempFlag == true)
			continue;
		if (*ptr < 100)
		{
			tempEnd = wIndex;
			tempFlag = false;
		}
		if (tempDistance < tempEnd - tempStart)
		{
			a[0] = tempDistance = tempEnd - tempStart;
			a[1] = tempStart;
			a[2] = tempEnd;
			a[3] = (*line->Item)[tempStart + tempDistance / 2].y;
		}

	}
	if (a[0] == 0)
	{
		a[1] = 0;
	}
	return;
}





//int getRectZone(ListPoint* line, int* a, CvRect* zone1, CvRect* zone2, int dist)
//{
//	//û�л������
//	if (a[0] == 0)
//	{
//		//getMinYPositon		
//		//getMinYPositon(ListPoint* line, int firstPosition, int lastPosition, CvRect* zone, int* a, int direction)
//
//	}
//	else//�л������
//	{
//		//getMinYPositon
//		//if()
//	}
//	
//}

//direction:
//0: ��Ӧ���޷�϶���
//1����Ӧ�ڷ�϶����������������
//2: ��Ӧ�ڷ�϶�������Ҳ��������
//3����Ӧ�ڷ�϶���Ҳ������������
//4: ��Ӧ�ڷ�϶���Ҳ���Ҳ��������
//
bool getMinYPositon(ListPoint* line, int firstPosition, int lastPosition, CvRect* zone, int* a, CvSize imgSize, int direction)
{
	int tempX = 0, tempY = 0;
	int tempWidth = imgSize.width;
	int tempHeight = imgSize.height;

	int wIndex = 0;

	if (firstPosition < 0)
		firstPosition = 0;


	if (lastPosition > line->Item->size())
		lastPosition = line->Item->size();

	if (direction == 0)
	{
		for (wIndex = firstPosition; wIndex < lastPosition; wIndex++)
		{
			if ((*line->Item)[wIndex].y > tempY)
				tempY = (*line->Item)[wIndex].y;
		}

		zone->x = 0;
		zone->y = tempY + 15;
		zone->width = tempWidth;
		zone->height = tempHeight - (tempY + 15) >= 0 ? (tempHeight - tempY - 15) : (tempHeight - tempY);
		return true;
	}

	//���
	if (direction == 1)//a[1] >= tempWidth/2
	{
		//ǰ����
		for (wIndex = firstPosition; wIndex < lastPosition; wIndex++)
		{
			if ((*line->Item)[wIndex].y > tempY)
				tempY = (*line->Item)[wIndex].y;
		}

		if (lastPosition < 18)
		{
			zone->x = 0;
			zone->y = 0;
			zone->width = 0;
			zone->height = 0;
			return false;
		}

		if (a[1] <= tempWidth / 2)
		{
			zone->x = 0;
			zone->y = tempY + 15;
			zone->height = tempHeight - (tempY + 15);
			zone->width = lastPosition - 15;
			return true;
		}

		return false;

	}

	if (direction == 2)
	{
		//�����
		for (wIndex = firstPosition; wIndex < lastPosition; wIndex++)
		{
			if ((*line->Item)[wIndex].y > tempY)
				tempY = (*line->Item)[wIndex].y;
		}

		zone->x = firstPosition + 15;
		zone->y = tempY + 15;
		zone->width = tempWidth - (firstPosition + 15);
		zone->height = tempHeight - (tempY + 15);
		return true;
	}

	//�Ҳ�
	if (direction == 3)
	{
		//ǰ����
		for (wIndex = firstPosition; wIndex < lastPosition; wIndex++)
		{
			if ((*line->Item)[wIndex].y > tempY)
				tempY = (*line->Item)[wIndex].y;
		}

		zone->x = 0;
		zone->y = tempY + 15;
		zone->width = lastPosition - 15;
		zone->height = tempHeight - (tempY + 15);
		return true;

	}

	if (direction == 4)
	{
		//�����
		for (wIndex = firstPosition; wIndex < lastPosition; wIndex++)
		{
			if ((*line->Item)[wIndex].y > tempY)
				tempY = (*line->Item)[wIndex].y;
		}

		if (firstPosition + 18 > tempWidth)
		{
			zone->x = 0;
			zone->y = 0;
			zone->width = 0;
			zone->height = 0;
			return false;
		}

		if (a[2] >= tempWidth / 2)
		{
			zone->x = firstPosition + 15;
			zone->y = tempY;
			zone->height = tempHeight - (tempY + 15);
			zone->width = lastPosition - (firstPosition + 15);
			return true;
		}

		return false;
	}

}



bool checkLightNum(IplImage* ZoneImg, int LeakLightNum)
{
	int leakNum = 0;
	int bwPosition = 0;
	for (int hIndex = 0; hIndex < ZoneImg->height; hIndex++)
	{
		for (int wIndex = 0; wIndex < ZoneImg->width; wIndex++)
		{
			bwPosition = hIndex*ZoneImg->widthStep + wIndex;
			if (ZoneImg->imageData[bwPosition] == 255)
				leakNum++;
		}
	}

	if (leakNum > LeakLightNum)
		return true;

	return false;
}





bool CheckZoneLeak(IplImage* currImageBefore, CvRect Zone, int LeakLightNum)
{
	IplImage* ZoneImg = cvCreateImage(cvSize(Zone.width, Zone.height), IPL_DEPTH_8U, 1);
	memset(ZoneImg->imageData, 0, ZoneImg->height*ZoneImg->widthStep*sizeof(unsigned char));

	cvSetImageROI(currImageBefore, Zone);

	cvCopy(currImageBefore, ZoneImg);

	cvResetImageROI(currImageBefore);

	cvShowImage("ZoneImage", ZoneImg);
	cvWaitKey(0);

	if (checkLightNum(ZoneImg, LeakLightNum))
	{
		cvReleaseImage(&ZoneImg);
		return true;
	}

	cvReleaseImage(&ZoneImg);
	return false;
}


void ByteToBool(byte* sourceByte, int number, bool* result)
{
	for (size_t i = 0; i < number; i++)
	{
		*(result + i) = (*(sourceByte + i / 8) >> i % 8) & 1;
	}
}

//������ת�����ֽ�
void INTToByte(UINT16 intSource, byte* byHigh, byte* byLow)
{
	WORD Address = intSource;
	*byLow = 0;
	*byHigh = 0;
	*byLow |= Address;
	Address >>= 8;
	*byHigh |= Address;
}

void DwordToWord(DWORD dwordSource, WORD* wordHigh, WORD* wordLow)
{
	DWORD Address = dwordSource;
	*wordLow = 0;
	*wordHigh = 0;
	*wordLow |= Address;
	Address >>= 16;
	*wordHigh |= Address;
}

UINT ThreadComm(LPVOID pParam)
{
	CommParms* ptp = (CommParms*)pParam;
	HWND m_hWnd = ptp->hWnd;
	bool* Running = ptp->running;
	CCriticalSection* pCriticalSection = ptp->pCriticalSection;
	delete ptp;

	bool grab = false;
	while (*Running)
	{
		pCriticalSection->Lock();
		grab = mySerialPort.ReadCommRelay(M(0), 20, readBool);
		pCriticalSection->Unlock();
		if (grab)
		{
			PostMessage(m_hWnd, WM_USER_COMM_FINISHED, 0, 0);
		}

		Sleep(100);
	}
	return 0;
}

void CMFCTestDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Running = false;
	Sleep(1000);
	WaitForSingleObject(m_threadComm->m_hThread, 1000);
	mySerialPort.CloseMyComm();
	CDialogEx::OnClose();
}

LRESULT CMFCTestDlg::CommFished(WPARAM wParam, LPARAM lParam)
{
	TRACE("%d\n", readBool[0]);
	return 0;
}

void CMFCTestDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	m_csComm.Lock();
	bool b;
	mySerialPort.ReadCommRelay(M(0), 1, &b);
	if (b)
	{
		mySerialPort.WriteCommRelay(M(0), 0);
	}
	else
	{
		mySerialPort.WriteCommRelay(M(0), 1);
	}	
	m_csComm.Unlock();
}
