#ifndef DLL_Camera
#define DLL_Camera
#endif

#include "CameraPublic.h"

int otsu(cv::Mat dst)
{

	int i, j;
	int tmp;

	double u0, u1, w0, w1, u, uk;

	double cov;
	double maxcov = 0.0;
	int maxthread = 0;

	int hst[MAX_GRAY_VALUE] = { 0 };
	double pro_hst[MAX_GRAY_VALUE] = { 0.0 };

	int height = dst.cols;
	int width = dst.rows;


	//统计每个灰度的数量
	for (i = 0; i<width; i++)
	{
		for (j = 0; j<height; j++)
		{
			tmp = dst.at<uchar>(i, j);
			hst[tmp]++;
		}
	}

	//计算每个灰度级占图像中的概率
	for (i = MIN_GRAY_VALUE; i<MAX_GRAY_VALUE; i++)
		pro_hst[i] = (double)hst[i] / (double)(width*height);


	//计算平均灰度值
	u = 0.0;
	for (i = MIN_GRAY_VALUE; i<MAX_GRAY_VALUE; i++)
		u += i*pro_hst[i];

	double det = 0.0;
	for (i = MIN_GRAY_VALUE; i< MAX_GRAY_VALUE; i++)
		det += (i - u)*(i - u)*pro_hst[i];

	//统计前景和背景的平均灰度值，并计算类间方差
	for (i = MIN_GRAY_VALUE; i<MAX_GRAY_VALUE; i++)
	{
		w0 = 0.0; w1 = 0.0; u0 = 0.0; u1 = 0.0; uk = 0.0;

		for (j = MIN_GRAY_VALUE; j < i; j++)
		{
			uk += j*pro_hst[j];
			w0 += pro_hst[j];

		}
		u0 = uk / w0;
		w1 = 1 - w0;
		u1 = (u - uk) / (1 - w0);

		//计算类间方差
		cov = w0*w1*(u1 - u0)*(u1 - u0);

		if (cov > maxcov)
		{
			maxcov = cov;
			maxthread = i;
		}
	}

	return maxthread;

}

void JDDetect(IplImage* sourceImage)
{
	const int max_corners = 3;
	const char* filename = "JINGLONGLONG.jpg";
	int cornerCount = max_corners;//角点的最大数目

	//用于保存角点的坐标
	CvPoint2D32f corners[max_corners];

	IplImage*grayImage = 0, *corners1 = 0, *corners2 = 0;

	cvNamedWindow("IMAGE", CV_WINDOW_AUTOSIZE);
	//读入图像，检测
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
			sourceImage,     //要画圆的图像
			cvPoint((int)(corners[i].x), (int)(corners[i].y)),           //圆心
			6,   //半径
			color,//颜色
			2,    //貌似是线宽
			CV_AA,//线型
			0    //圆心坐标和和半径的小数点的数目
			);
	}
	//cvShowImage("IMAGE", sourceImage);

	//cvWaitKey(0);
	//释放资源	
	cvReleaseImage(&corners1);
	cvReleaseImage(&corners2);
	cvReleaseImage(&grayImage);
	cvDestroyWindow("IMAGE");
}

bool checkLightNum(IplImage* ZoneImg, int LeakLightNum, int checkLightValue)
{
	int leakNum = 0;
	int bwPosition = 0;
	//如果checkLightValue==-1 使用自动阈值，否则使用固定阈值checkLightValue
	for (int hIndex = 0; hIndex < ZoneImg->height; hIndex++)
	{
		for (int wIndex = 0; wIndex < ZoneImg->width; wIndex++)
		{
			bwPosition = hIndex*ZoneImg->widthStep + wIndex;
			if (ZoneImg->imageData[bwPosition] >= checkLightValue)
				leakNum++;
		}
	}

	if (leakNum > LeakLightNum)
		return true;

	return false;
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
//其中，segments是利用图像分割算法得到的轮廓序列，而pFrImg则是目标检测算法检测到的前景目标，这样就可以分割图像的结果来对目标检测的区域进行填充。另外，对于如何得到分割图像，也可以采用以下函数来实现：

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
// _mask是一个二值图像，可以是Canny边缘检测或cvThreshold等函数得到的二值图像。

bool CheckZoneLeak(IplImage* currImageBefore, CvRect Zone, int LeakLightNum, int checkLightValue)
{
	IplImage* ZoneImg = cvCreateImage(cvSize(Zone.width, Zone.height), IPL_DEPTH_8U, 1);
	memset(ZoneImg->imageData, 0, ZoneImg->height*ZoneImg->widthStep*sizeof(unsigned char));

	cvSetImageROI(currImageBefore, Zone);

	cvCopy(currImageBefore, ZoneImg);

	cvResetImageROI(currImageBefore);

	//cvShowImage("ZoneImage", ZoneImg);
	//cvWaitKey(0);

	if (checkLightNum(ZoneImg, LeakLightNum, checkLightValue))
	{
		cvReleaseImage(&ZoneImg);
		return true;
	}

	cvReleaseImage(&ZoneImg);
	return false;
}

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

	//左侧
	if (direction == 1)//a[1] >= tempWidth/2
	{
		//前矩形
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
		//后矩形
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

	//右侧
	if (direction == 3)
	{
		//前矩形
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
		//后矩形
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

int OnNewvision(IplImage* currImageBefore, IplImage* maskImage)
{
	//int pAPosition[6][2]={0};//pointArrayPosition
	//pAPosition[0][0]=0; pAPosition[0][1]=6;
	//pAPosition[1][0]=6; pAPosition[1][1]=0;
	//pAPosition[2][0]=12;pAPosition[2][1]=6;

	//pAPosition[3][0]=0; pAPosition[3][1]=9;
	//pAPosition[4][0]=6; pAPosition[4][1]=12;
	//pAPosition[5][0]=12;pAPosition[5][1]=9;

	//ListPoint pointSet1;
	//pointSet1.Item = (ArrayPoint*)MallocArrayPoint();
	//for (int dIndex = 0; dIndex <3; dIndex++)
	//{//cvPoint(wIndex, hIndex)
	//	pointSet1.Item->push_back(cvPoint(pAPosition[dIndex][0], pAPosition[dIndex][1]));
	//}	
	//
	//cvCircleObj outCircle1;
	//if(pointSet1.Item->size() == 0)
	//	return;
	//FitCircleObj(pointSet1, &outCircle1);

	//ListPoint pointSet2;
	//pointSet2.Item = (ArrayPoint*)MallocArrayPoint();
	//for (int dIndex = 3; dIndex <6; dIndex++)
	//{//cvPoint(wIndex, hIndex)
	//	pointSet2.Item->push_back(cvPoint(pAPosition[dIndex][0], pAPosition[dIndex][1]));
	//}
	//cvCircleObj outCircle2;
	//if(pointSet2.Item->size() == 0)
	//	return;
	//FitCircleObj(pointSet2, &outCircle2);

	//IplImage* currImage1 = cvCreateImage(cvSize(2000, 3000), IPL_DEPTH_8U, 1);
	//memset(currImage1->imageData, 0, currImage1->height*currImage1->widthStep*sizeof(unsigned char));

	//int bwPosition = 0;
	//for (int hIndex = 0; hIndex < currImage1->width; hIndex++)
	//{
	//	int y1 = 0, y2 = 0;
	//	y1 = sqrt( outCircle1.Radius * outCircle1.Radius - hIndex * hIndex);
	//	y2 = sqrt( outCircle2.Radius * outCircle2.Radius - hIndex * hIndex);
	//	for (int wIndex = 0; wIndex < currImage1->height; wIndex++)
	//	{
	//		bwPosition = hIndex*currImage1->widthStep + wIndex;
	//		if( wIndex < y1)
	//			currImage1->imageData[bwPosition] = 0;
	//		else if (wIndex > y1 && wIndex < y2)
	//			currImage1->imageData[bwPosition] = 255;
	//		else
	//			currImage1->imageData[bwPosition] = 0;
	//	}
	//}
	//cvShowImage("currImage1",currImage1);
	//cvWaitKey(0);

	//cvReleaseImage(&currImage1);
	//return;


	int hIndex = 0, wIndex = 0, ImagePosition = 0, TempPosition = 0, colorValue = 0;
	bool leakLight = false;	int LeakLightNum = 5;

	//IplImage* currImageBefore = cvLoadImage("00.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	if (currImageBefore == NULL)
		return 1;

	if (maskImage == NULL)
	{
		return 2;
	}

	IplImage* currImage = cvCreateImage(cvSize(currImageBefore->width, currImageBefore->height), IPL_DEPTH_8U, 1);
	memset(currImage->imageData, 0, currImage->height*currImage->widthStep*sizeof(unsigned char));

	//使用Mask
	cvCopy(currImageBefore, currImage, maskImage);

	//cvShowImage("currImage",currImage);
	//cvWaitKey(0);

	IplImage* EdgeImage = cvCreateImage(cvSize(currImageBefore->width, currImageBefore->height), IPL_DEPTH_8U, 1);
	memset(EdgeImage->imageData, 0, EdgeImage->height*EdgeImage->widthStep*sizeof(unsigned char));

	cvCanny(currImage, EdgeImage, 50, 180, 3);

	//cvShowImage("EdgeImage", EdgeImage);
	//cvWaitKey(0);


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

	//cvShowImage("EdgeImage2", EdgeImage);
	////cvSaveImage("E:\\wuxi\\EdgeImage.jpg",EdgeImage);
	//cvWaitKey(0);

	ListPoint pointSet; ListPoint bestPoint; ListPoint tempPoint;
	pointSet.Item = (ArrayPoint*)MallocArrayPoint();
	bestPoint.Item = (ArrayPoint*)MallocArrayPoint();
	tempPoint.Item = (ArrayPoint*)MallocArrayPoint();


	ListPoint pointSet13, pointSet23, pointSet33;
	ListPoint bestPoint13, bestPoint23, bestPoint33;
	pointSet13.Item = (ArrayPoint*)MallocArrayPoint();
	pointSet23.Item = (ArrayPoint*)MallocArrayPoint();
	pointSet33.Item = (ArrayPoint*)MallocArrayPoint();

	bestPoint13.Item = (ArrayPoint*)MallocArrayPoint();
	bestPoint23.Item = (ArrayPoint*)MallocArrayPoint();
	bestPoint33.Item = (ArrayPoint*)MallocArrayPoint();

	IplImage* markImage = cvCreateImage(cvGetSize(currImage), IPL_DEPTH_8U, 1);
	memset(markImage->imageData, 0, markImage->height*markImage->widthStep*sizeof(unsigned char));

	//ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	ListRect rectList; ListInt intAreaList;
	rectList.Item = (ArrayRect*)MallocArrayRect();
	intAreaList.Item = (ArrayInt *)MallocArrayInt();

	ExtractAllEdgePointNumForItem(EdgeImage, markImage, cvRect(0, 0, currImageBefore->width, currImageBefore->height), 255, &pointSet);

	//未搜寻到边缘点，可能是已经边缘效果不好或者是无料
	if (pointSet.Item->size() == 0 || pointSet.Item->size() < 10)
	{
		cvReleaseImage(&currImageBefore);
		//cvReleaseImage(&maskImage);
		cvReleaseImage(&markImage);
		cvReleaseImage(&currImage);
		cvReleaseImage(&EdgeImage);
		return 3;
	}

	CvPoint PartTempPoint;

	for (int dIndex = 0; dIndex < pointSet.Item->size() / 3; dIndex++)
	{
		PartTempPoint = (*pointSet.Item)[dIndex];

		AddArrayPoint(pointSet13.Item, PartTempPoint);
	}

	cvCircleObj TempCircle;

	memset(markImage->imageData, 0, markImage->height*markImage->widthStep*sizeof(unsigned char));

	//之前使用方法为全部进行ransac滤除，之后为缩短时间修改为1/3、1/3、1/3进行滤除
	//RansacCirclePoint(pointSet, &bestPoint, &tempPoint);
	//if(bestPoint.Item->size() == 0)
	//{
	//	cvReleaseImage(&currImageBefore);
	//	cvReleaseImage(&maskImage);
	//	cvReleaseImage(&markImage);
	//	cvReleaseImage(&currImage);
	//	cvReleaseImage(&EdgeImage);
	//	return;
	//}
	//SortPointsListByXValue(&bestPoint);

	RansacCirclePoint(pointSet13, &bestPoint13, &tempPoint);

	if (bestPoint13.Item->size() == 0)
	{
		cvReleaseImage(&currImageBefore);
		//cvReleaseImage(&maskImage);
		cvReleaseImage(&markImage);
		cvReleaseImage(&currImage);
		cvReleaseImage(&EdgeImage);
		return 4;
	}

	cvCircleObj outCircle;
	cvCircleObj outCircle13, outCircle23, outCircle33;
	//ListPoint bestPoint13, bestPoint23, bestPoint33;
	//bestPoint13.Item = (ArrayPoint*)MallocArrayPoint();	
	//bestPoint23.Item = (ArrayPoint*)MallocArrayPoint();	
	//bestPoint33.Item = (ArrayPoint*)MallocArrayPoint();	

	//for (int dIndex = 0; dIndex< bestPoint.Item->size()/3; dIndex++)
	//{
	//	AddArrayPoint(bestPoint13.Item, (*bestPoint.Item)[dIndex]);
	//}
	FitCircleObj(bestPoint13, &outCircle13);

	if (outCircle13.CirclePoint.x < 0 || outCircle13.CirclePoint.y >0)
	{
		for (int dIndex = pointSet.Item->size() / 3; dIndex < 2 * pointSet.Item->size() / 3; dIndex++)
		{
			PartTempPoint = (*pointSet.Item)[dIndex];

			AddArrayPoint(pointSet23.Item, PartTempPoint);
		}

		RansacCirclePoint(pointSet23, &bestPoint23, &tempPoint);

		if (bestPoint23.Item->size() == 0)
		{
			cvReleaseImage(&currImageBefore);
			//cvReleaseImage(&maskImage);
			cvReleaseImage(&markImage);
			cvReleaseImage(&currImage);
			cvReleaseImage(&EdgeImage);
			return 5;
		}

		FitCircleObj(bestPoint23, &outCircle23);

		if (outCircle23.CirclePoint.x < 0 || outCircle23.CirclePoint.y >0)
		{
			for (int dIndex = 2 * pointSet.Item->size() / 3; dIndex < pointSet.Item->size(); dIndex++)
			{
				PartTempPoint = (*pointSet.Item)[dIndex];

				AddArrayPoint(pointSet33.Item, PartTempPoint);
			}

			RansacCirclePoint(pointSet33, &bestPoint33, &tempPoint);

			if (bestPoint33.Item->size() == 0)
			{
				cvReleaseImage(&currImageBefore);
				//cvReleaseImage(&maskImage);
				cvReleaseImage(&markImage);
				cvReleaseImage(&currImage);
				cvReleaseImage(&EdgeImage);
				return 6;
			}

			FitCircleObj(bestPoint33, &outCircle33);

			if (outCircle33.CirclePoint.x < 0 || outCircle33.CirclePoint.y >0)
			{
				outCircle.CirclePoint.x = 0;
				outCircle.CirclePoint.y = 1;
				outCircle.Radius = 1;
			}
			else
				outCircle = outCircle33;;
		}
		else
			outCircle = outCircle23;

	}
	else
		outCircle = outCircle13;

	//FitCircleObj(bestPoint, &outCircle);

	if (outCircle.CirclePoint.y == 1 && outCircle.Radius == 1)
	{
		cvReleaseImage(&currImageBefore);
		//cvReleaseImage(&maskImage);
		cvReleaseImage(&markImage);
		cvReleaseImage(&currImage);
		cvReleaseImage(&EdgeImage);
		return 7;
	}

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
			y = VL_MAX(0, outCircle.CirclePoint.y + y);
		else
			y = VL_MAX(0, outCircle.CirclePoint.y - y);

		TempPoint.y = y;
		if (TempPoint.x >= 0 && TempPoint.y >= 0)
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

	//cvShowImage("currbf", currImageBefore);
	//cvWaitKey(0);


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

	int checkLightValue = 55;
	bool Zone0 = false, Zone1 = false, Zone2 = false, Zone3 = false, Zone4 = false;
	//没有豁口区域
	if (a[0] == 0)
	{
		if (getMinYPositon(&pointMoreCircleSet, 0, currImage->width, &Zone, a, imgSize, 0))
		{
			if (getMinYPositon(&pointMoreCircleSet, 0, currImageBefore->width, &Zone, a, imgSize, 0))
				Zone0 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum, checkLightValue);
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
	else//有豁口区域
	{
		//getMinYPositon
		if (a[1] > currImageBefore->width / 2)
		{
			if (getMinYPositon(&pointMoreCircleSet, a[2], currImageBefore->width, &Zone, a, imgSize, 4))
				Zone4 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum, checkLightValue);

			if (getMinYPositon(&pointMoreCircleSet, 0, a[1], &Zone, a, imgSize, 3))
				Zone3 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum, checkLightValue);
		}

		if (a[1] <= currImageBefore->width / 2)
		{
			if (getMinYPositon(&pointMoreCircleSet, 0, a[1], &Zone, a, imgSize, 1))
				Zone1 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum, checkLightValue);

			if (getMinYPositon(&pointMoreCircleSet, a[2], currImageBefore->width, &Zone, a, imgSize, 2))
				Zone2 = CheckZoneLeak(currImageBefore, Zone, LeakLightNum, checkLightValue);
		}
	}

	cvReleaseImage(&currImageBefore);
	//cvReleaseImage(&maskImage);
	cvReleaseImage(&markImage);
	cvReleaseImage(&currImage);
	cvReleaseImage(&EdgeImage);

	bool lastResult = false;
	if (Zone0 || Zone1 || Zone2 || Zone3 || Zone4)
	{
		lastResult = true;
		return -1;
	}

	lastResult = false;
	return 0;

}


