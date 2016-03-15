
#include <ImagePre/ImagePre.h>
#include <ListArrayLib/ListArrayLib.h>

DLLAPI void STDCALL MarkEdgeIplImageForRectBorder(IplImage* srcImage, CvRect rect, int ObjectGray, IplImage* EdgeImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		Position = hIndex*srcImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, Position++)
		{
			//四周的边缘像素---轮廓边缘像素判断
			if(hIndex == rect.y || hIndex == rect.y + rect.height - 1
				|| wIndex == rect.x || wIndex == rect.x + rect.width - 1)
			{
				if(srcImage->imageData[Position] == ObjectGray)
					EdgeImage->imageData[Position] = ObjectGray;
				else
				{
					if ((wIndex == rect.x && srcImage->imageData[Position + 1] == ObjectGray)
						|| (wIndex == rect.x + rect.width - 1 && srcImage->imageData[Position - 1] == ObjectGray)
						|| (hIndex == rect.y && srcImage->imageData[Position + srcImage->widthStep] == ObjectGray)
						|| (hIndex == rect.y + rect.height - 1 && srcImage->imageData[Position - srcImage->widthStep] == ObjectGray))
						EdgeImage->imageData[Position] = ObjectGray;
					else
						EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
			}
			else
			{
				if(srcImage->imageData[Position] == ObjectGray)
				{
					if (srcImage->imageData[Position + 1] == 255 - ObjectGray
						|| srcImage->imageData[Position - 1] == 255 - ObjectGray
						|| srcImage->imageData[Position + srcImage->widthStep] == 255 - ObjectGray
						|| srcImage->imageData[Position - srcImage->widthStep] == 255 - ObjectGray) 
						EdgeImage->imageData[Position] = ObjectGray;
					else
						EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
				else
				{
					EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
			}//单个像素判断结束			
		}
	}
}

DLLAPI void STDCALL TraceEdgeForIplImageModel(IplImage* srcImage, IplImage* MarkImage, int MarkValue, 
											  int hIndex, int wIndex, ListPoint* EdgePoint)
{	
	int dIndex = 0, Position = hIndex*srcImage->widthStep + wIndex;

	int PointX = 0, PointY = 0, NumIndex = 0;

	int DirectIndex = 0, IsFindStartPoint = 0, IsFindIndex = 3;

	static int Direction[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

	//向动态数组中添加第一个像素点坐标
	AddArrayPoint(EdgePoint->Item, cvPoint(wIndex, hIndex));

	MarkImage->imageData[Position] = 128;   //标识当前像素点已经被跟踪

	//表示为目标色的起始像素，开始进行跟踪操作---当前像素向后遍历
	while(NumIndex < EdgePoint->Item->size())
	{
		NumIndex++;

		for (DirectIndex = 0; DirectIndex < 8; DirectIndex++)
		{
			PointX = (*EdgePoint->Item)[NumIndex - 1].x + Direction[IsFindIndex][0];
			PointY = (*EdgePoint->Item)[NumIndex - 1].y + Direction[IsFindIndex][1];

			if(PointX >= srcImage->width || PointX < 0 || PointY >= srcImage->height || PointY < 0)
				continue;

			Position = PointY*srcImage->widthStep + PointX;

			//表示当前像素为边缘点
			if(srcImage->imageData[Position] == MarkValue && MarkImage->imageData[Position] != 128)
			{
				//将领域坐标加入到动态数组中
				AddArrayPoint(EdgePoint->Item, cvPoint(PointX, PointY));

				MarkImage->imageData[Position] = 128;     //标识当前像素点已经被跟踪

				if(DirectIndex - 1 < 0)
					IsFindIndex = 0;
				else
					IsFindIndex = DirectIndex - 1;

				break;
			}
			else
			{
				IsFindIndex++;

				if(IsFindIndex == 8)
					IsFindIndex = 0;
			}
		}

	}//while循环结束，即连续区域结束
}

/****************************************************************
	提取边缘图像中所有边缘的边缘长度最大的边缘
*****************************************************************/
DLLAPI int STDCALL ExtractMaxEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect, 
												  int ObjectGray, ListPoint* PointList)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, Position = 0, EdgeNum = 0;

	//基于二值图像定位到杆号对应的位置
	ListPoint EdgePoint;
	EdgePoint.Item = (ArrayPoint *)MallocArrayPoint();

	//基于点集合计算最小外接矩形的四个顶点
	cvRectObj TempRect; CvPoint TempPoint;

	memset(MarkImage->imageData, 255 - ObjectGray, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

	//确定区域范围内边缘数目最多的像素
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		Position = hIndex*EdgeImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, Position++)
		{
			if(EdgeImage->imageData[Position] == 255 - ObjectGray
				|| MarkImage->imageData[Position] == 128)
				continue;

			TraceEdgeForIplImageModel(EdgeImage, MarkImage, ObjectGray, hIndex, wIndex, &EdgePoint);

			if(EdgePoint.Item->size() > 30)
				EdgeNum++;

			if(PointList->Item->size() < EdgePoint.Item->size())
				CopyArrayPoint(PointList->Item, EdgePoint.Item);

			ClearArrayPoint(EdgePoint.Item);
		}
	}//整个区域遍历结束

	FreeArrayPoint(EdgePoint.Item);

	return EdgeNum;
}

/****************************************************************
	提取边缘图像中所有边缘像素点
*****************************************************************/
DLLAPI int STDCALL ExtractAllEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect,
												 int ObjectGray, ListPoint* PointList)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, Position = 0, EdgeNum = 0;

	//基于二值图像定位到杆号对应的位置
	ListPoint EdgePoint;
	EdgePoint.Item = (ArrayPoint *)MallocArrayPoint();

	//基于点集合计算最小外接矩形的四个顶点
	cvRectObj TempRect; CvPoint TempPoint;

	memset(MarkImage->imageData, 255 - ObjectGray, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

	/*cvShowImage("edgeImg", EdgeImage);
	cvWaitKey(0);*/

	//确定区域范围内边缘数目最多的像素
	for (hIndex = rect.y; hIndex < VL_MIN(MarkImage->height,(rect.y + rect.height)); hIndex++)
	{
		Position = hIndex*EdgeImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < VL_MIN(MarkImage->width,(rect.x + rect.width)); wIndex++, Position++)
		{
			if(EdgeImage->imageData[Position] == 255 - ObjectGray
				|| MarkImage->imageData[Position] == 128)
				continue;

			TraceEdgeForIplImageModel(EdgeImage, MarkImage, ObjectGray, hIndex, wIndex, &EdgePoint);

			if(EdgePoint.Item->size() > 30)
			{
				EdgeNum++;

				if(PointList->Item->size() == 0)
					CopyArrayPoint(PointList->Item, EdgePoint.Item);
				else
				{
					for(dIndex = 0; dIndex < EdgePoint.Item->size(); dIndex++)
					{
						TempPoint = (*EdgePoint.Item)[dIndex];

						AddArrayPoint(PointList->Item, TempPoint);
					}
				}
			}			

			ClearArrayPoint(EdgePoint.Item);
		}
	}//整个区域遍历结束

	FreeArrayPoint(EdgePoint.Item);

	return EdgeNum;
}

DLLAPI void STDCALL FilterRegionListForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
											int MinMarkNum, int MaxMarkNum, IplImage* dstImage, ListRect* rectList, ListInt* intList, ArrayPoint* PointArray)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//使用C++标准库中的矢量创建动态数组
	//ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0; 

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				Position = YValue*srcImage->widthStep + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			if(PointArray->size() > MaxMarkNum || PointArray->size() < MinMarkNum)
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}	
			else
			{
				//将相关区域添加到对应的矩形框架列表中
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
			}

			//if(PointArray->size() > 0)
				//ClearArrayPoint(PointArray);				
		}
	}//整个区域遍历结束

	//FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterBlobNoiseForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
										   int MinMarkNum, int MaxMarkNum, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));
			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				//访问矢量动态数组中的元素,确定迭代的当前像素的位置
				if (XValue <= rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			if (PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum) //不可以被滤除的斑点
			{
				//如果不需要删除的斑点，则先标示为gray
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}//不可以被滤除的斑点结束

			//对矢量动态数组清除
			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
												   int MinMarkNum, int MaxMarkNum, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, TempGray = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//访问矢量动态数组中的元素,确定迭代的当前像素的位置
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			if(PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum)
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}
				
			if(MaxXValue - MinXValue > 0.90*rect.width || MaxYValue - MinYValue > 0.90*rect.height)
			{
				
			}
			else
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}		

			ClearArrayPoint(PointArray);
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterMaxHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
													  int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//访问矢量动态数组中的元素,确定迭代的当前像素的位置
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			//若矩形的高度或者宽度满足要求，直接滤除
			if(MaxXValue - MinXValue > 0.90*rect.width || MaxYValue - MinYValue > 0.90*rect.height)
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
				continue;
			}

			if(PointArray->size() > BestArray->size())
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				if(PointArray->size() > 0)
					ClearArrayPoint(PointArray);
			}				
		}
	}//整个区域遍历结束

	FreeArrayPoint(BestArray);
	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterAreaEdgeRatioForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, 
											   int MinAreaNum, int MaxAreaNum, IplImage* dstImage, 
											   ListRect* rectList, ListInt* intList, ListFloat* floatList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//访问矢量动态数组中的元素,确定迭代的当前像素的位置
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//基于四领域判断当前像素是否为边缘像素
				if((XValue - 1 >= rect.x && srcImage->imageData[Position - 1] == 255 - Objectgray)
					|| (XValue + 1 < rect.x + rect.width && srcImage->imageData[Position + 1] == 255 - Objectgray)
					|| (YValue - 1 >= rect.y && srcImage->imageData[Position - srcImage->widthStep] == 255 - Objectgray)
					|| (YValue + 1 < rect.y + rect.height && srcImage->imageData[Position + srcImage->widthStep] == 255 - Objectgray))
					EdgeNum++;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			//若矩形的高度或者宽度满足要求，直接滤除
			//if(PointArray->size()/EdgeNum > MinRatoValue)
			//{
			//	//将当前目标设置为背景色
			//	for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
			//	{
			//		Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

			//		dstImage->imageData[Position] = 255 - Objectgray;
			//	}

			//	ClearArrayPoint(PointArray);
			//	continue;
			//}

			if(PointArray->size() > MaxAreaNum || PointArray->size() < MinAreaNum)
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}	
			else
			{
				float TempValue = (float)PointArray->size()/EdgeNum;
				//将相关区域添加到对应的矩形框架列表中
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
				AddArrayFloat(floatList->Item, TempValue);
			}
			
			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);				
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);
}


DLLAPI void STDCALL FilterMaxRegionForRectImage(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	//对整个区域的像素进行遍历和搜索
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0; 

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				Position = YValue*srcImage->widthStep + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			if(PointArray->size() > BestArray->size())
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				//将当前目标设置为背景色
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				if(PointArray->size() > 0)
					ClearArrayPoint(PointArray);
			}				
		}
	}//整个区域遍历结束

	FreeArrayPoint(BestArray);	FreeArrayPoint(PointArray);
}

DLLAPI bool STDCALL FilterMaxRegionForImage(IplImage* srcImage, IplImage* MarkImage, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, TempGray = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0, IsExistOthers = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//使用C++标准库中的矢量创建动态数组
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	//对整个区域的像素进行遍历和搜索
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//判断当前像素是否为背景色
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//判断当前像素是否被标记过
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//当前坐标数据加入到矢量动态数组中
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = 1<<30; MinYValue = 1<<30; MaxXValue = 0; MaxYValue = 0;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				//访问矢量动态数组中的元素,确定迭代的当前像素的位置
				/*if (XValue < 0 || XValue >= srcImage->width
					|| YValue < 0 || YValue >= srcImage->height)
					continue;*/

				Position = YValue*srcImage->width + XValue;

				//八领域区域的左上角坐标
				if (XValue - 1 >= 0 && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//八领域区域的上坐标
				if (XValue >= 0 && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//八领域区域的右上角坐标
				if (XValue + 1 < srcImage->width && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//八领域区域的右边坐标
				if (XValue + 1 < srcImage->width && YValue >= 0
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//八领域区域的右下角坐标
				if (XValue + 1 < srcImage->width && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= 0 && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= 0 && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= 0 && YValue >= 0
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			//表示两个数组都大于0，表示存在两个目标
			if(BestArray->size() > 20 && PointArray->size() > 20)
				IsExistOthers = 1;

			if (PointArray->size() > BestArray->size())    //满足矩形区域的要求
			{
				//将BestArray对应位置清零
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
			}
		}
	}//整个区域遍历结束

	FreeArrayPoint(BestArray);
	FreeArrayPoint(PointArray);

	if(IsExistOthers == 1)
		return true;
	else
		return false;
}

DLLAPI void STDCALL FilterEdgeRegionRectList(IplImage* srcImage, IplImage* MarkImage, int Objectgray, CvRect rect, int MinMarkNum,
											 int MaxMarkNum, IplImage* dstImage, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, IsEdgeRegion = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0; IsEdgeRegion = 0; 

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				if (XValue == rect.x || XValue == rect.x + rect.width - 1
					|| YValue == rect.y || YValue == rect.y + rect.height - 1)
				{
					IsEdgeRegion = 1;
				}

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			if (IsEdgeRegion == 1)
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else
			{
				if(PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum || MaxXValue - MinXValue > 2*(MaxYValue - MinYValue)) //不可以被滤除的斑点
				{
					for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
					{
						Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
						dstImage->imageData[Position] = 255 - Objectgray;					
					}
				}//不可以被滤除的斑点结束
				else
				{
					//将相关区域添加到对应的矩形框架列表中
					AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
					AddArrayInt(intList->Item, PointArray->size());
				}
			}

			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//整个区域遍历结束
	
	FreeArrayPoint(PointArray);
}


DLLAPI void STDCALL FilterAllRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
											int MaxMarkNum, float HeightWidthRatio, bool IsSide, IplImage* dstImage, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;  float RatioValue = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			RatioValue = (float)VL_MAX(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1)/VL_MIN(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

			if (PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum) //不可以被滤除的斑点
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}//不可以被滤除的斑点结束
			else if(MaxXValue - MinXValue > 0.75*srcImage->width ||
				MaxYValue - MinYValue > 0.75*srcImage->height || RatioValue > HeightWidthRatio)
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinXValue <= rect.x + 1 || MaxXValue + 1 >=rect.x + rect.width))//基于X边缘滤波
			{
				//针对轴区域进行滤波
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinYValue <= rect.y + 1 || MaxYValue + 1 >=rect.y + rect.height))//基于Y边缘滤波
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else
			{
				//将相关区域添加到对应的矩形框架列表中
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
			}

			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterMaxRectRegion(IplImage* srcImage, IplImage* MarkImage,int Objectgray, IplImage* dstImage, 
										CvRect* rectRegion, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, IsEdgeRegion = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	CvRect rect = cvRect(0, 0, srcImage->width, srcImage->height), TempRect;

	(*rectRegion) = cvRect(0, 0, 0, 0);

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0, IsEdgeRegion = 0;

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//while循环结束,即八领域标记结束

			TempRect = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

			if(PointArray->size() > 30)
			{
				AddArrayRect(rectList->Item, TempRect);
				AddArrayInt(intList->Item, PointArray->size());
			}

			if (TempRect.height*TempRect.width > rectRegion->height*rectRegion->width)    //满足矩形区域的要求
			{
				//将BestArray对应位置清零
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);

				(*rectRegion) = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);
			}
			else
			{
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
			}
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);
	FreeArrayPoint(BestArray);
}