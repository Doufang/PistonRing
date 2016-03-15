/*********************************************
基于传入的边缘图像、灰度图像提取矩形对象
***********************************************/
#include <ListArrayLib/ListArrayLib.h>
#include <ObjectLib/ObjectLib.h>
#include <GeometryLib/GeometryLib.h>

int CalculateDistrictIndex(ArrayPoint PointArray, CvPoint StartPoint, CvPoint EndPoint, int StartIndex, int EndIndex)
{
	static float ConverDis = 3.2;

	CvPoint TempPoint;

	float TempDis = 0, MaxTempDis = 0;
	int PointIndex = 0; 

	cvLineObj TempLine = DotDotLine(StartPoint, EndPoint);

	//计算其中偏移距离最远的像素点
	for (int dIndex = StartIndex + 1; dIndex <= EndIndex - 1; dIndex++)
	{
		TempPoint = PointArray[dIndex];

		//当前点到直线的距离
		TempDis = TempLine.sinAValue*TempPoint.x - TempLine.cosBValue*TempPoint.y + TempLine.InterceptValue;

		if (abs(TempDis) >= MaxTempDis)
		{
			MaxTempDis = abs(TempDis);

			PointIndex = dIndex;
		}
	}

	if(MaxTempDis < ConverDis)
		return 0;
	else
		return PointIndex;
}

//基于循环迭代的方式计算凹凸点的数组
void  CalculateDistrictPoint(ArrayPoint PointArray, CvPoint StartPoint, CvPoint EndPoint, int StartIndex, int EndIndex, ListPoint* ConverPoint)
{
	CvPoint TempPoint;

	int PointIndex = CalculateDistrictIndex(PointArray, StartPoint, EndPoint, StartIndex, EndIndex);

	if(PointIndex == 0)
		return;

	TempPoint = PointArray[PointIndex];

	AddArrayPoint(ConverPoint->Item, TempPoint);

	CalculateDistrictPoint(PointArray, PointArray[StartIndex], PointArray[PointIndex], StartIndex, PointIndex, ConverPoint);
	CalculateDistrictPoint(PointArray, PointArray[PointIndex], PointArray[EndIndex], PointIndex, EndIndex, ConverPoint);
}

DLLAPI void STDCALL GetConvexPointByListPoint(ListPoint* PointList, ListPoint* ConvexList)
{
	//八个区域坐标极值点
	CvPoint ExtrePoint1, ExtrePoint2, ExtrePoint3, ExtrePoint4, ExtrePoint5, ExtrePoint6, ExtrePoint7, ExtrePoint8;

	//X轴和Y轴的最大值和最小值
	int MinXValue = 1<<30, MaxXValue = 0, MinYValue = 1<<30, MaxYValue = 0;   

	int IsRegionExist1 = 1, IsRegionExist2 = 1, IsRegionExist3 = 1, IsRegionExist4 = 1;

	ArrayPoint RegionPoint1, RegionPoint2, RegionPoint3, RegionPoint4;

	CvPoint TempPoint, StartPoint, EndPoint;		//存储计算分区凸包点的起始点

	//在四轴方向上的极值点边缘
	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		if (MaxYValue < TempPoint.y)
		{
			MaxYValue = TempPoint.y;
			ExtrePoint1 = TempPoint;
			ExtrePoint2 = TempPoint;
		}
		else if (MaxYValue == TempPoint.y)
		{
			//1取X值较小的一边
			ExtrePoint1.x = TempPoint.x < ExtrePoint1.x ? TempPoint.x : ExtrePoint1.x;

			//2取X值较大的一边
			ExtrePoint2.x = TempPoint.x > ExtrePoint2.x ? TempPoint.x : ExtrePoint2.x;
		}

		if (MaxXValue < TempPoint.x)
		{
			MaxXValue = TempPoint.x;
			ExtrePoint3 = TempPoint;
			ExtrePoint4 = TempPoint;
		}
		else if (MaxXValue == TempPoint.x)
		{
			//3取Y值较大的一边
			ExtrePoint3.y = TempPoint.y > ExtrePoint3.y ? TempPoint.y : ExtrePoint3.y;

			//4取Y值较小的一边
			ExtrePoint4.y = TempPoint.y < ExtrePoint4.y ? TempPoint.y : ExtrePoint4.y;
		}	

		if (MinYValue > TempPoint.y)
		{
			MinYValue = TempPoint.y;
			ExtrePoint5 = TempPoint;
			ExtrePoint6 = TempPoint;
		}
		else if (MinYValue == TempPoint.y)
		{
			//5取X值较大的一边
			ExtrePoint5.x = TempPoint.x > ExtrePoint5.x ? TempPoint.x : ExtrePoint5.x;

			//6取X值较小的一边
			ExtrePoint6.x = TempPoint.x < ExtrePoint6.x ? TempPoint.x : ExtrePoint6.x;
		}			

		if (MinXValue > TempPoint.x)
		{
			MinXValue = TempPoint.x;

			ExtrePoint7 = TempPoint; 
			ExtrePoint8 = TempPoint;
		}
		else if (MinXValue == TempPoint.x)
		{
			//7取Y值较小的一边
			ExtrePoint7.y = TempPoint.y < ExtrePoint7.y ? TempPoint.y : ExtrePoint7.y;

			//8取Y值较大的一边
			ExtrePoint8.y = TempPoint.y > ExtrePoint8.y ? TempPoint.y : ExtrePoint8.y;
		}
	}

	//判断基于边界点的四个分区是否存在
	if (ExtrePoint8.x == ExtrePoint1.x && ExtrePoint8.y == ExtrePoint1.y)
		IsRegionExist1 = 0;

	if (ExtrePoint2.x == ExtrePoint3.x && ExtrePoint2.y == ExtrePoint3.y)
		IsRegionExist2 = 0;

	if (ExtrePoint4.x == ExtrePoint5.x && ExtrePoint4.y == ExtrePoint5.y)
		IsRegionExist3 = 0;

	if (ExtrePoint6.x == ExtrePoint7.x && ExtrePoint6.y == ExtrePoint7.y)
		IsRegionExist4 = 0;

	//对点集进行分区操作
	int MarkDis = 0;
	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		//并且当前点处于1,8两点之间的矩形中
		if(IsRegionExist1 == 1 
			&& TempPoint.x >= VL_MIN(ExtrePoint1.x, ExtrePoint8.x) && TempPoint.x <= VL_MAX(ExtrePoint1.x, ExtrePoint8.x)
			&& TempPoint.y >= VL_MIN(ExtrePoint1.y, ExtrePoint8.y) && TempPoint.y <= VL_MAX(ExtrePoint1.y, ExtrePoint8.y))
		{
			//点到直线的距离
			MarkDis = (ExtrePoint1.x - ExtrePoint8.x)*(TempPoint.y - ExtrePoint8.y) 
				- (ExtrePoint1.y - ExtrePoint8.y)*(TempPoint.x - ExtrePoint8.x);

			if(MarkDis >= 1 || MarkDis <= -1)
				RegionPoint1.push_back(TempPoint);
		}

		if(IsRegionExist2 == 1
			&& TempPoint.x >= VL_MIN(ExtrePoint2.x, ExtrePoint3.x) && TempPoint.x <= VL_MAX(ExtrePoint2.x, ExtrePoint3.x)
			&& TempPoint.y >= VL_MIN(ExtrePoint2.y, ExtrePoint3.y) && TempPoint.y <= VL_MAX(ExtrePoint2.y, ExtrePoint3.y))
		{
			MarkDis = (ExtrePoint3.x - ExtrePoint2.x)*(TempPoint.y - ExtrePoint2.y) 
				- (ExtrePoint3.y - ExtrePoint2.y)*(TempPoint.x - ExtrePoint2.x);

			if(MarkDis >= 1 || MarkDis <= -1)
				RegionPoint2.push_back(TempPoint);
		}

		if (IsRegionExist3 == 1
			&& TempPoint.x >= VL_MIN(ExtrePoint4.x, ExtrePoint5.x) && TempPoint.x <= VL_MAX(ExtrePoint4.x, ExtrePoint5.x)
			&& TempPoint.y >= VL_MIN(ExtrePoint4.y, ExtrePoint5.y) && TempPoint.y <= VL_MAX(ExtrePoint4.y, ExtrePoint5.y))
		{
			MarkDis = (ExtrePoint5.x - ExtrePoint4.x)*(TempPoint.y - ExtrePoint4.y) 
				- (ExtrePoint5.y - ExtrePoint4.y)*(TempPoint.x - ExtrePoint4.x);

			if(MarkDis >= 1 || MarkDis <= -1)
				RegionPoint3.push_back(TempPoint);
		}

		if(IsRegionExist4 == 1
			&& TempPoint.x >= VL_MIN(ExtrePoint6.x, ExtrePoint7.x) && TempPoint.x <= VL_MAX(ExtrePoint6.x, ExtrePoint7.x)
			&& TempPoint.y >= VL_MIN(ExtrePoint6.y, ExtrePoint7.y) && TempPoint.y <= VL_MAX(ExtrePoint6.y, ExtrePoint7.y))
		{
			MarkDis = (ExtrePoint7.x - ExtrePoint6.x)*(TempPoint.y - ExtrePoint6.y) 
				- (ExtrePoint7.y - ExtrePoint6.y)*(TempPoint.x - ExtrePoint6.x);

			if(MarkDis >= 1 || MarkDis <= -1)
				RegionPoint4.push_back(TempPoint);
		}
	}

	//第一个区域若存在
	StartPoint = ExtrePoint8;

	AddArrayPoint(ConvexList->Item, StartPoint);//此点一定是凸包点

	if (IsRegionExist1 == 1)  //对第一个区域内进行凹凸点集提取
	{
		EndPoint = ExtrePoint1;

		if(RegionPoint1.size() > 0)
			CalculateDistrictPoint(RegionPoint1, StartPoint, EndPoint, 0, RegionPoint1.size() - 1, ConvexList);

		AddArrayPoint(ConvexList->Item, EndPoint);
	}

	if (ExtrePoint1.x != ExtrePoint2.x || ExtrePoint1.y != ExtrePoint2.y)
		AddArrayPoint(ConvexList->Item, ExtrePoint2);

	if (IsRegionExist2 == 1)
	{
		StartPoint = ExtrePoint2; EndPoint = ExtrePoint3;

		if(RegionPoint2.size() > 0)
			CalculateDistrictPoint(RegionPoint2,StartPoint,EndPoint,0, RegionPoint2.size() - 1, ConvexList);

		AddArrayPoint(ConvexList->Item, EndPoint);
	}

	if (ExtrePoint3.x != ExtrePoint4.x || ExtrePoint3.y != ExtrePoint4.y)
		AddArrayPoint(ConvexList->Item, ExtrePoint4);

	if (IsRegionExist3 == 1)
	{
		StartPoint = ExtrePoint4; EndPoint = ExtrePoint5;

		if(RegionPoint3.size() > 0)
			CalculateDistrictPoint(RegionPoint3,StartPoint,EndPoint,0, RegionPoint3.size() - 1, ConvexList);

		AddArrayPoint(ConvexList->Item, EndPoint);
	}

	if (ExtrePoint5.x != ExtrePoint6.x || ExtrePoint5.y != ExtrePoint6.y)
		AddArrayPoint(ConvexList->Item, ExtrePoint6);

	if (IsRegionExist3 == 1)
	{
		StartPoint = ExtrePoint6; EndPoint = ExtrePoint7;

		if(RegionPoint4.size() > 0)
			CalculateDistrictPoint(RegionPoint4,StartPoint,EndPoint,0, RegionPoint4.size() - 1, ConvexList);

		AddArrayPoint(ConvexList->Item, EndPoint);
	}

	if (ExtrePoint7.x != ExtrePoint8.x || ExtrePoint7.y != ExtrePoint8.y)
		AddArrayPoint(ConvexList->Item, ExtrePoint8);

	RegionPoint1.clear();
	RegionPoint2.clear();
	RegionPoint3.clear();
	RegionPoint4.clear();	
}

float CalculateAngleRectArea(ListPoint* PointList, int delta, cvLineObj* TempLine1, cvLineObj* TempLine2, cvLineObj* TempLine3, cvLineObj* TempLine4)
{	
	CvPoint TempPoint;

	float TempIntercept = 0, Area = 0;

	TempLine1->sinAValue = sinf(delta*VL_PI/180);
	TempLine1->cosBValue = cosf(delta*VL_PI/180);
	TempLine1->InterceptValue = -1 << 30;						

	TempLine2->sinAValue = sinf(delta*VL_PI/180);
	TempLine2->cosBValue = cosf(delta*VL_PI/180);
	TempLine2->InterceptValue = 1 << 30;

	TempLine3->sinAValue = sinf(delta*VL_PI/180 + VL_PI/2);
	TempLine3->cosBValue = cosf(delta*VL_PI/180 + VL_PI/2);
	TempLine3->InterceptValue = -1 << 30;					

	TempLine4->sinAValue = sinf(delta*VL_PI/180 + VL_PI/2);
	TempLine4->cosBValue = cosf(delta*VL_PI/180 + VL_PI/2);
	TempLine4->InterceptValue = 1 << 30;

	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		TempIntercept = -TempLine1->sinAValue * TempPoint.x + TempLine1->cosBValue * TempPoint.y;

		//等到Intercept的最大值
		TempLine1->InterceptValue = TempIntercept > TempLine1->InterceptValue ? TempIntercept : TempLine1->InterceptValue;

		//得到Intercept的最小值
		TempLine2->InterceptValue = TempIntercept < TempLine2->InterceptValue ? TempIntercept : TempLine2->InterceptValue;


		TempIntercept = -TempLine3->sinAValue * TempPoint.x + TempLine3->cosBValue * TempPoint.y;

		TempLine3->InterceptValue = TempIntercept > TempLine3->InterceptValue ? TempIntercept : TempLine3->InterceptValue;

		TempLine4->InterceptValue = TempIntercept < TempLine4->InterceptValue ? TempIntercept : TempLine4->InterceptValue;
	}

	Area = abs(TempLine1->InterceptValue - TempLine2->InterceptValue) * abs(TempLine3->InterceptValue - TempLine4->InterceptValue);

	return Area;
}

void CalculateMinRectPara(ListPoint* PointList, cvRectObj* rect)
{
	cvLineObj BestLine1,BestLine2,BestLine3,BestLine4;  //面积最小的四条直线
	cvLineObj TempLine1,TempLine2,TempLine3,TempLine4;	//四条直线的临时变量

	float TempArea = 0, MinArea = 0;									
	int delta = 0, BestDelta = 0;

	int MinXValue = 1 << 30, MinYValue = 1 << 30, MaxXValue = 0, MaxYValue = 0;

	CvPoint TempPoint;

	//计算整个区域内的正的最小外接矩形
	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		MinXValue = MinXValue > TempPoint.x ? TempPoint.x : MinXValue;
		MaxXValue = MaxXValue < TempPoint.x ? TempPoint.x : MaxXValue;

		MinYValue = MinYValue > TempPoint.y ? TempPoint.y : MinYValue;
		MaxYValue = MaxYValue < TempPoint.y ? TempPoint.y : MaxYValue;
	}

	//初始化正矩形的参数
	MinArea = (MaxXValue - MinXValue)*(MaxYValue - MinYValue);

	BestLine1 = DotDotLine(cvPoint(MinXValue, MinYValue), cvPoint(MaxXValue, MinYValue));
	BestLine2 = DotDotLine(cvPoint(MinXValue, MaxYValue), cvPoint(MaxXValue, MaxYValue));

	BestLine3 = DotDotLine(cvPoint(MinXValue, MinYValue), cvPoint(MinXValue, MaxYValue));
	BestLine4 = DotDotLine(cvPoint(MaxXValue, MinYValue), cvPoint(MaxXValue, MaxYValue));

	for (delta = -10; delta <= 10; delta += 2)
	{
		TempArea = CalculateAngleRectArea(PointList, delta, &TempLine1, &TempLine2, &TempLine3, &TempLine4);

		if (TempArea < MinArea)
		{
			BestLine1 = TempLine1; BestLine2 = TempLine2;
			BestLine3 = TempLine3; BestLine4 = TempLine4;

			BestDelta = delta; MinArea = TempArea;
		}
	}

	for (delta = BestDelta - 2; delta <= BestDelta + 2; delta++)
	{
		TempArea = CalculateAngleRectArea(PointList, delta, &TempLine1, &TempLine2, &TempLine3, &TempLine4);

		if (TempArea < MinArea)
		{
			BestLine1 = TempLine1; BestLine2 = TempLine2;
			BestLine3 = TempLine3; BestLine4 = TempLine4;

			BestDelta = delta; MinArea = TempArea;
		}
	}

	//基于线段计算四个交点
	rect->FirstPoint = LineLineIntersection(BestLine1, BestLine3);
	rect->SecondPoint = LineLineIntersection(BestLine1, BestLine4);
	rect->ThirdPoint = LineLineIntersection(BestLine2, BestLine4);
	rect->FourthPoint = LineLineIntersection(BestLine2, BestLine3);
}

/*******************************************
基于点集合的动态数组计算当前的最小外接矩形
*********************************************/
DLLAPI void STDCALL GetMinimumEnclosedRectangle(ListPoint* PointList, cvRectObj* outRect)
{
	ListPoint ConvexPoint;
	ConvexPoint.Item = (ArrayPoint *) MallocArrayPoint();

	//提取当前点集的凹凸点
	GetConvexPointByListPoint(PointList, &ConvexPoint);

	//基于凹凸点计算最小外接矩形的角度和参数
	cvRectObj rect;

	CalculateMinRectPara(&ConvexPoint, &rect);

	memcpy(outRect, &rect, sizeof(cvRectObj));

	FreeArrayPoint(ConvexPoint.Item);
}

/**********************************************
     基于左上角和右下角确定边缘点
***********************************************/
DLLAPI void STDCALL QuickHullAlgorithmForListPoint(ListPoint* PointList, ListPoint* ConvexList)
{
	//八个区域坐标极值点
	CvPoint UpLeftPoint, DownRightPoint;

	//X轴和Y轴的最大值和最小值
	int MinXValue = 1<<30, MaxXValue = 0, MinYValue = 1<<30, MaxYValue = 0;   

	ArrayPoint RegionPoint1, RegionPoint2;

	CvPoint TempPoint;//, StartPoint, EndPoint;		//存储计算分区凸包点的起始点

	//在四轴方向上的极值点边缘
	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		//若当前像素点坐标较小，则选择该像素点
		if (MinXValue > TempPoint.x)
		{
			MinXValue = TempPoint.x;
			UpLeftPoint = TempPoint;
		}
		else if (MinXValue == TempPoint.x)  //若X相同，则选择Y轴最大值
		{
			UpLeftPoint.y = TempPoint.y > UpLeftPoint.y ? TempPoint.y : UpLeftPoint.y;
		}

		//若当前像素点坐标较大，则选择该像素点
		if (MaxXValue < TempPoint.x)
		{
			MaxXValue = TempPoint.x;
			DownRightPoint = TempPoint;
		}
		else if (MaxXValue == TempPoint.x)  //若同为最大值，则选择Y轴最小值
		{
			DownRightPoint.y = TempPoint.y < DownRightPoint.y ? TempPoint.y : DownRightPoint.y;
		}
	}

	cvLineObj DivisionLineObj;

	DivisionLineObj = DotDotLine(UpLeftPoint, DownRightPoint);

	//对点集进行分区操作
	float MarkDis = 0;

	for (int dIndex = 0; dIndex < PointList->Item->size(); dIndex++)
	{
		TempPoint = (*PointList->Item)[dIndex];

		//并且当前点处于1,8两点之间的矩形中
		//点到直线的距离
		MarkDis = (UpLeftPoint.x - DownRightPoint.x)*(TempPoint.y - DownRightPoint.y) 
			- (UpLeftPoint.y - DownRightPoint.y)*(TempPoint.x - DownRightPoint.x);

		MarkDis = DotLineDis(TempPoint, DivisionLineObj);

		if(MarkDis >= 0)
			RegionPoint1.push_back(TempPoint);
		else
			RegionPoint2.push_back(TempPoint);
	}

	//第一个区域若存在
	AddArrayPoint(ConvexList->Item, UpLeftPoint);//此点一定是凸包点

	if(RegionPoint1.size() > 0)
		CalculateDistrictPoint(RegionPoint1, UpLeftPoint, DownRightPoint, 0, RegionPoint1.size() - 1, ConvexList);

	AddArrayPoint(ConvexList->Item, DownRightPoint);

	if(RegionPoint2.size() > 0)
		CalculateDistrictPoint(RegionPoint2,DownRightPoint,UpLeftPoint,0, RegionPoint2.size() - 1, ConvexList);

	RegionPoint1.clear();
	RegionPoint2.clear();	
}

//判断两个圆周是否存在合并的现象
bool IsIncludeRectForInt(CvRect& rect1, CvRect rect2, int& IntValue1, int IntValue2, int deltaValue)
{
	int MinXValue = VL_MIN(rect1.x, rect2.x), MaxXValue = VL_MAX(rect1.x + rect1.width, rect2.x + rect2.width);
	int MinYValue = VL_MIN(rect1.y, rect2.y), MaxYValue = VL_MAX(rect1.y + rect1.height, rect2.y + rect2.height);

	//表示合并失败
	if(MaxXValue - MinXValue > rect1.width + rect2.width + deltaValue)
		return false;

	//表示合并失败
	if(MaxYValue - MinYValue > rect1.height + rect2.height + deltaValue)
		return false;	

	rect1 = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

	IntValue1 = IntValue1 + IntValue2;

	return true;
}

bool IsIncludeRect(CvRect& rect1, CvRect rect2, int deltaValue)
{
	int MinXValue = VL_MIN(rect1.x, rect2.x), MaxXValue = VL_MAX(rect1.x + rect1.width, rect2.x + rect2.width);
	int MinYValue = VL_MIN(rect1.y, rect2.y), MaxYValue = VL_MAX(rect1.y + rect1.height, rect2.y + rect2.height);

	//表示合并失败
	if(MaxXValue - MinXValue > rect1.width + rect2.width + deltaValue)
		return false;

	//表示合并失败
	if(MaxYValue - MinYValue > rect1.height + rect2.height + deltaValue)
		return false;	

	rect1 = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

	return true;
}

bool IsIncludeRectForHeight(CvRect& rect1, CvRect rect2, int& IntValue1, int IntValue2, int deltaValue)
{
	int MinXValue = VL_MIN(rect1.x, rect2.x), MaxXValue = VL_MAX(rect1.x + rect1.width, rect2.x + rect2.width);
	int MinYValue = VL_MIN(rect1.y, rect2.y), MaxYValue = VL_MAX(rect1.y + rect1.height, rect2.y + rect2.height);

	//表示合并失败
	if(MaxXValue - MinXValue > rect1.width + rect2.width)
		return false;

	//表示合并失败
	if(MaxYValue - MinYValue > rect1.height + rect2.height + deltaValue)
		return false;	

	rect1 = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

	IntValue1 = IntValue1 + IntValue2;

	return true;
}

//基于RectObj对象判断是否存在包含的关系
DLLAPI void STDCALL MergeRectListForInt(ListRect* RectList, ListInt* IntList, int deltaValue)
{
	CvRect TempRect1, TempRect2;  int IntValue1 = 0, IntValue2 = 0;

	ListRect TempRectList;  ListInt TempIntList;

	TempRectList.Item = (ArrayRect *)MallocArrayRect();
	TempIntList.Item = (ArrayInt *)MallocArrayInt();

	int startIndex = 0, endIndex = 0;

	while(RectList->Item->size() > 0)
	{
		startIndex = RectList->Item->size() - 1;

		TempRect1 = (*RectList->Item)[startIndex];
		IntValue1 = (*IntList->Item)[startIndex];

		//遍历除去第一个矩形以外的所有区域，判断是否存在可以合并的区域
		for (endIndex = startIndex - 1; endIndex >= 0; endIndex--)
		{
			TempRect2 = (*RectList->Item)[endIndex];
			IntValue2 = (*IntList->Item)[endIndex];

			//判断两个矩形是否存在包含关系，存在并且该将第一个矩形删除，则将其保留
			if(IsIncludeRectForInt(TempRect1, TempRect2, IntValue1, IntValue2, deltaValue))
			{
				EraseArrayRect(RectList->Item, endIndex);
				EraseArrayInt(IntList->Item, endIndex);
			}
		}

		AddArrayRect(TempRectList.Item, TempRect1);
		AddArrayInt(TempIntList.Item, IntValue1);

		startIndex = RectList->Item->size() - 1;  //将最后一个区域删除

		EraseArrayRect(RectList->Item, startIndex);
		EraseArrayInt(IntList->Item, startIndex);
	}

	CopyArrayRect(RectList->Item, TempRectList.Item);
	CopyArrayInt(IntList->Item, TempIntList.Item);

	FreeArrayRect(TempRectList.Item);
	FreeArrayInt(TempIntList.Item);
}

//基于RectObj对象判断是否存在包含的关系
DLLAPI void STDCALL MergeRectMsgList(ListRectMsg* RectList, int deltaValue)
{
	RectMsg TempRect1, TempRect2; 

	ListRectMsg TempRectList;  

	TempRectList.Item = (ArrayRectMsg *)MallocArrayRectMsg();
	
	int startIndex = 0, endIndex = 0;

	while(RectList->Item->size() > 0)
	{
		startIndex = RectList->Item->size() - 1;

		TempRect1 = (*RectList->Item)[startIndex];
		
		//遍历除去第一个矩形以外的所有区域，判断是否存在可以合并的区域
		for (endIndex = startIndex - 1; endIndex >= 0; endIndex--)
		{
			TempRect2 = (*RectList->Item)[endIndex];
			
			//判断两个矩形是否存在包含关系，存在并且该将第一个矩形删除，则将其保留
			if(IsIncludeRect(TempRect1.rect, TempRect2.rect, deltaValue))
			{
				EraseArrayRectMsg(RectList->Item, endIndex);
			}
		}

		AddArrayRectMsg(TempRectList.Item, TempRect1);
		
		startIndex = RectList->Item->size() - 1;  //将最后一个区域删除

		EraseArrayRectMsg(RectList->Item, startIndex);
	}

	CopyArrayRectMsg(RectList->Item, TempRectList.Item);
	
	FreeArrayRectMsg(TempRectList.Item);
}

DLLAPI void STDCALL MergeRectListForHeight(ListRect* RectList, ListInt* IntList, int deltaValue)
{
	CvRect TempRect1, TempRect2;  int IntValue1 = 0, IntValue2 = 0;

	ListRect TempRectList;  ListInt TempIntList;

	TempRectList.Item = (ArrayRect *)MallocArrayRect();
	TempIntList.Item = (ArrayInt *)MallocArrayInt();

	int startIndex = 0, endIndex = 0;

	while(RectList->Item->size() > 0)
	{
		startIndex = RectList->Item->size() - 1;

		TempRect1 = (*RectList->Item)[startIndex];
		IntValue1 = (*IntList->Item)[startIndex];

		//遍历除去第一个矩形以外的所有区域，判断是否存在可以合并的区域
		for (endIndex = startIndex - 1; endIndex >= 0; endIndex--)
		{
			TempRect2 = (*RectList->Item)[endIndex];
			IntValue2 = (*IntList->Item)[endIndex];

			//判断两个矩形是否存在包含关系，存在并且该将第一个矩形删除，则将其保留
			if(IsIncludeRectForHeight(TempRect1, TempRect2, IntValue1, IntValue2, deltaValue))
			{
				EraseArrayRect(RectList->Item, endIndex);
				EraseArrayInt(IntList->Item, endIndex);
			}
		}

		AddArrayRect(TempRectList.Item, TempRect1);
		AddArrayInt(TempIntList.Item, IntValue1);

		startIndex = RectList->Item->size() - 1;  //将最后一个区域删除
		EraseArrayRect(RectList->Item, startIndex);
		EraseArrayInt(IntList->Item, startIndex);
	}

	CopyArrayRect(RectList->Item, TempRectList.Item);
	CopyArrayInt(IntList->Item, TempIntList.Item);

	FreeArrayRect(TempRectList.Item);
	FreeArrayInt(TempIntList.Item);
}

//表示二维与三维之间存在交集---即判断可能存在缺陷
bool IsIncludeRectForThree(CvRect& rect1, CvRect rect2, int MaxImageHeight, int MaxImageWidth, float deltaPercent)
{
	int MinXValue = VL_MIN(rect1.x, rect2.x), MaxXValue = VL_MAX(rect1.x + rect1.width, rect2.x + rect2.width);
	int MinYValue = VL_MIN(rect1.y, rect2.y), MaxYValue = VL_MAX(rect1.y + rect1.height, rect2.y + rect2.height);

	int deltaWidth = VL_MAX(100, cvRound(deltaPercent*VL_MIN(rect1.width, rect2.width)));
	int deltaHeight = VL_MAX(100, cvRound(deltaPercent*VL_MIN(rect1.height, rect2.height)));

	//表示合并失败
	if(MaxXValue - MinXValue > rect1.width + rect2.width - VL_MIN(10, cvRound(deltaPercent*rect1.width)))
		return false;

	//表示合并失败
	if(MaxYValue - MinYValue > rect1.height + rect2.height - VL_MIN(10, cvRound(deltaPercent*rect1.height)))
		return false;	

	//取两个区域中面积较小部分作为参考依据
	if(rect1.height*rect1.width > 0.25*MaxImageHeight*MaxImageWidth 
		&& rect1.height*rect1.width > 10*rect2.height*rect2.width)
	{
		MinXValue = MinXValue > rect2.x - deltaWidth ? MinXValue : rect2.x - deltaWidth;
		MinYValue = MinYValue > rect2.y - deltaHeight ? MinYValue : rect2.y - deltaHeight;

		MaxXValue = MaxXValue < rect2.x + rect2.width + deltaWidth ? MaxXValue : rect2.x + rect2.width + deltaWidth;
		MaxYValue = MaxYValue < rect2.y + rect2.height + deltaHeight ? MaxYValue : rect2.y + rect2.height + deltaHeight;
	}
	else
	{
		MinXValue = MinXValue > rect1.x - deltaWidth ? MinXValue : rect1.x - deltaWidth;
		MinYValue = MinYValue > rect1.y - deltaHeight ? MinYValue : rect1.y - deltaHeight;

		MaxXValue = MaxXValue < rect1.x + rect1.width + deltaWidth ? MaxXValue : rect1.x + rect1.width + deltaWidth;
		MaxYValue = MaxYValue < rect1.y + rect1.height + deltaHeight ? MaxYValue : rect1.y + rect1.height + deltaHeight;	
	}	

	MinXValue = MinXValue > 3 ? MinXValue : 3;
	MinYValue = MinYValue > 3 ? MinYValue : 3;

	MaxXValue = MaxXValue < MaxImageWidth - 3 ? MaxXValue : MaxImageWidth - 3;
	MaxYValue = MaxYValue < MaxImageHeight - 3 ? MaxYValue : MaxImageHeight - 3;

	rect1 = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

	return true;
}

//判断当前指定的矩形在指定的列表中是否存在共同区域
DLLAPI bool STDCALL MergeRectListForThree(ListRect* RectList, CvRect& mergeRect, int MaxImageHeight, int MaxImageWidth, float deltaPercent)
{
	CvRect TempRect;  

	//遍历整个区域的RectList，判断是否存在共同区域
	for (int hIndex = 0; hIndex < RectList->Item->size(); hIndex++)
	{
		TempRect = (*RectList->Item)[hIndex];

		if(!IsIncludeRectForThree(mergeRect, TempRect, MaxImageHeight, MaxImageWidth, deltaPercent))
			continue;

		return true;
	}

	return false;
}

