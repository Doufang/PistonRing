#include <ObjectLib/ObjectLib.h>
#include <ListArrayLib/ListArrayLib.h>

#define ELLIPSEDIS     0.12

//估算椭圆
DLLAPI void STDCALL EstimateEllipsePoints(ListPoint PointSet, cvEllipseObj ellipseObj, ArrayPoint* BestPoint, ArrayPoint* TempPoint)
{
	float XValue = 0, YValue = 0, XDisValue = 0, YDisValue = 0;

	CvPoint dTempPoint;

	float ctheta=cosf(ellipseObj.angle*VL_PI/180);
	float stheta=sinf(ellipseObj.angle*VL_PI/180);

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		dTempPoint = (*PointSet.Item)[hIndex];
		
		//计算各分支距离
		XValue = (dTempPoint.x - ellipseObj.center.x)*ctheta - (dTempPoint.y - ellipseObj.center.y)*stheta;
		YValue = (dTempPoint.x - ellipseObj.center.x)*stheta + (dTempPoint.y - ellipseObj.center.y)*ctheta;

		XDisValue = XValue*XValue*4/ellipseObj.size.width/ellipseObj.size.width;
		YDisValue = YValue*YValue*4/ellipseObj.size.height/ellipseObj.size.height;

		if((XDisValue + YDisValue) > 1 - ELLIPSEDIS && (XDisValue + YDisValue) < 1 + ELLIPSEDIS)//判断是否符合椭圆的方程
			AddArrayPoint(BestPoint, dTempPoint);
		else
			AddArrayPoint(TempPoint, dTempPoint);
	}
}

DLLAPI bool STDCALL FiveEllipseObj(CvPoint2D32f* PointArray, cvEllipseObj* outEllipse)
{
	CvBox2D box;

	cvFitEllipse(PointArray, 5, &box);

	outEllipse->center.x = box.center.x;
	outEllipse->center.y = box.center.y;
	outEllipse->size.height = box.size.height;
	outEllipse->size.width = box.size.width;
	outEllipse->angle = 90 - box.angle;

	return true;
}

DLLAPI void STDCALL RansacEllipsePoints(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex1 = 0, dIndex2 = 0,dIndex3 = 0,IsCircleObj = 0;

	cvEllipseObj tempEllipse;

	CvPoint2D32f FivePoints[5];    //分配5个点的空间

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	int TempValue = 0, MiddleValue = 0;

	for (hIndex = 0; hIndex < 0.4*PointSet.Item->size(); hIndex++)
	{
		wIndex = PointSet.Item->size() - 1 - hIndex;
		FivePoints[4].x = (*PointSet.Item)[wIndex].x;
		FivePoints[4].y = (*PointSet.Item)[wIndex].y;

		TempValue = wIndex - hIndex;

		FivePoints[0].x = (*PointSet.Item)[hIndex].x;
		FivePoints[0].y = (*PointSet.Item)[hIndex].y;

		dIndex1 = hIndex + TempValue*0.25;
		FivePoints[1].x = (*PointSet.Item)[dIndex1].x;
		FivePoints[1].y = (*PointSet.Item)[dIndex1].y;

		dIndex2 = hIndex + TempValue*0.5;
		FivePoints[2].x = (*PointSet.Item)[dIndex2].x;
		FivePoints[2].y = (*PointSet.Item)[dIndex2].y;

		dIndex3 = hIndex + TempValue*0.75;
		FivePoints[3].x = (*PointSet.Item)[dIndex3].x;
		FivePoints[3].y = (*PointSet.Item)[dIndex3].y;		

		//基于5点拟合椭圆
		FiveEllipseObj(FivePoints, &tempEllipse);

		//判断拟合的椭圆参数是否符合
		if (tempEllipse.center.x < 1.0 || tempEllipse.center.y < 1.0)
			continue;

		EstimateEllipsePoints(PointSet, tempEllipse, dBestPoint, dTempPoint);

		//一直寻找，直到找到的最佳点数是最多的。
		if(dBestPoint->size() > BestPoint->Item->size())
		{
			CopyArrayPoint(BestPoint->Item, dBestPoint);
			CopyArrayPoint(TempPoint->Item, dTempPoint);
		}
		else
		{
			ClearArrayPoint(dBestPoint);
			ClearArrayPoint(dTempPoint);
		}
	}

	MiddleValue = PointSet.Item->size()*0.5;

	for (hIndex = MiddleValue - 5; hIndex > 0.5*MiddleValue; hIndex--)
	{
		wIndex = MiddleValue + (MiddleValue - hIndex);

		FivePoints[4].x = (*PointSet.Item)[wIndex].x;
		FivePoints[4].y = (*PointSet.Item)[wIndex].y;

		TempValue = wIndex - hIndex;

		FivePoints[0].x = (*PointSet.Item)[hIndex].x;
		FivePoints[0].y = (*PointSet.Item)[hIndex].y;

		dIndex1 = hIndex + TempValue*0.25;
		FivePoints[1].x = (*PointSet.Item)[dIndex1].x;
		FivePoints[1].y = (*PointSet.Item)[dIndex1].y;

		dIndex2 = hIndex + TempValue*0.5;
		FivePoints[2].x = (*PointSet.Item)[dIndex2].x;
		FivePoints[2].y = (*PointSet.Item)[dIndex2].y;

		dIndex3 = hIndex + TempValue*0.75;
		FivePoints[3].x = (*PointSet.Item)[dIndex3].x;
		FivePoints[3].y = (*PointSet.Item)[dIndex3].y;		

		//基于5点拟合椭圆
		FiveEllipseObj(FivePoints, &tempEllipse);

		//判断拟合的椭圆参数是否符合
		if (tempEllipse.center.x < 1.0 || tempEllipse.center.y < 1.0)
			continue;

		EstimateEllipsePoints(PointSet, tempEllipse, dBestPoint, dTempPoint);

		//一直寻找，直到找到的最佳点数是最多的。
		if(dBestPoint->size() > BestPoint->Item->size())
		{
			CopyArrayPoint(BestPoint->Item, dBestPoint);
			CopyArrayPoint(TempPoint->Item, dTempPoint);
		}
		else
		{
			ClearArrayPoint(dBestPoint);
			ClearArrayPoint(dTempPoint);
		}
	}

	TempValue = PointSet.Item->size()*0.5;

	for (hIndex = 0; hIndex < TempValue; hIndex++)
	{
		FivePoints[0].x = (*PointSet.Item)[hIndex].x;
		FivePoints[0].y = (*PointSet.Item)[hIndex].y;

		dIndex1 = hIndex + TempValue*0.25;
		FivePoints[1].x = (*PointSet.Item)[dIndex1].x;
		FivePoints[1].y = (*PointSet.Item)[dIndex1].y;

		dIndex2 = hIndex + TempValue*0.5;
		FivePoints[2].x = (*PointSet.Item)[dIndex2].x;
		FivePoints[2].y = (*PointSet.Item)[dIndex2].y;

		dIndex3 = hIndex + TempValue*0.75;
		FivePoints[3].x = (*PointSet.Item)[dIndex3].x;
		FivePoints[3].y = (*PointSet.Item)[dIndex3].y;

		wIndex = TempValue + hIndex;
		FivePoints[4].x = (*PointSet.Item)[wIndex].x;
		FivePoints[4].y = (*PointSet.Item)[wIndex].y;

		//基于5点拟合椭圆
		FiveEllipseObj(FivePoints, &tempEllipse);
	
		//判断拟合的椭圆参数是否符合
		if (tempEllipse.center.x < 1.0 || tempEllipse.center.y < 1.0)
			continue;

		EstimateEllipsePoints(PointSet, tempEllipse, dBestPoint, dTempPoint);

		//一直寻找，直到找到的最佳点数是最多的。
		if(dBestPoint->size() > BestPoint->Item->size())
		{
			CopyArrayPoint(BestPoint->Item, dBestPoint);
			CopyArrayPoint(TempPoint->Item, dTempPoint);
		}
		else
		{
			ClearArrayPoint(dBestPoint);
			ClearArrayPoint(dTempPoint);
		}
	}

	for (hIndex = PointSet.Item->size() - 1; hIndex >= TempValue; hIndex--)
	{
		FivePoints[0].x = (*PointSet.Item)[hIndex].x;
		FivePoints[0].y = (*PointSet.Item)[hIndex].y;

		dIndex1 = hIndex - TempValue*0.25;
		FivePoints[1].x = (*PointSet.Item)[dIndex1].x;
		FivePoints[1].y = (*PointSet.Item)[dIndex1].y;

		dIndex2 = hIndex - TempValue*0.5;
		FivePoints[2].x = (*PointSet.Item)[dIndex2].x;
		FivePoints[2].y = (*PointSet.Item)[dIndex2].y;

		dIndex3 = hIndex - TempValue*0.75;
		FivePoints[3].x = (*PointSet.Item)[dIndex3].x;
		FivePoints[3].y = (*PointSet.Item)[dIndex3].y;

		wIndex = hIndex - TempValue;
		FivePoints[4].x = (*PointSet.Item)[wIndex].x;
		FivePoints[4].y = (*PointSet.Item)[wIndex].y;

		//基于5点拟合椭圆
		FiveEllipseObj(FivePoints, &tempEllipse);

		//判断拟合的椭圆参数是否符合
		if (tempEllipse.center.x < 1.0 || tempEllipse.center.y < 1.0)
			continue;

		EstimateEllipsePoints(PointSet, tempEllipse, dBestPoint, dTempPoint);

		//一直寻找，直到找到的最佳点数是最多的。
		if(dBestPoint->size() > BestPoint->Item->size())
		{
			CopyArrayPoint(BestPoint->Item, dBestPoint);
			CopyArrayPoint(TempPoint->Item, dTempPoint);
		}
		else
		{
			ClearArrayPoint(dBestPoint);
			ClearArrayPoint(dTempPoint);
		}
	}

	for (hIndex = PointSet.Item->size()*0.75 - 1; hIndex >= TempValue*0.5 + 1; hIndex--)
	{
		FivePoints[0].x = (*PointSet.Item)[hIndex].x;
		FivePoints[0].y = (*PointSet.Item)[hIndex].y;

		dIndex1 = hIndex - TempValue*0.125;
		FivePoints[1].x = (*PointSet.Item)[dIndex1].x;
		FivePoints[1].y = (*PointSet.Item)[dIndex1].y;

		dIndex2 = hIndex - TempValue*0.25;
		FivePoints[2].x = (*PointSet.Item)[dIndex2].x;
		FivePoints[2].y = (*PointSet.Item)[dIndex2].y;

		dIndex3 = hIndex - TempValue*0.375;
		FivePoints[3].x = (*PointSet.Item)[dIndex3].x;
		FivePoints[3].y = (*PointSet.Item)[dIndex3].y;

		wIndex = hIndex - TempValue*0.5;
		FivePoints[4].x = (*PointSet.Item)[wIndex].x;
		FivePoints[4].y = (*PointSet.Item)[wIndex].y;

		//基于5点拟合椭圆
		FiveEllipseObj(FivePoints, &tempEllipse);

		//判断拟合的椭圆参数是否符合
		if (tempEllipse.center.x < 1.0 || tempEllipse.center.y < 1.0)
			continue;

		EstimateEllipsePoints(PointSet, tempEllipse, dBestPoint, dTempPoint);

		//一直寻找，直到找到的最佳点数是最多的。
		if(dBestPoint->size() > BestPoint->Item->size())
		{
			CopyArrayPoint(BestPoint->Item, dBestPoint);
			CopyArrayPoint(TempPoint->Item, dTempPoint);
		}
		else
		{
			ClearArrayPoint(dBestPoint);
			ClearArrayPoint(dTempPoint);
		}
	}

	FreeArrayPoint(dBestPoint);
	FreeArrayPoint(dTempPoint);
}


DLLAPI bool STDCALL FitEllipseObj(ListPoint PointSet, cvEllipseObj* outEllipse)
{
   if(PointSet.Item->size() < 5)
	   return false;

   CvPoint2D32f* PointArray = (CvPoint2D32f*)malloc(PointSet.Item->size()*sizeof(CvPoint2D32f));

   for(int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++ )
   {
	   PointArray[hIndex].x = (*PointSet.Item)[hIndex].x;
	   PointArray[hIndex].y = (*PointSet.Item)[hIndex].y;
   }

   CvBox2D box;
   
   cvFitEllipse(PointArray, PointSet.Item->size(), &box);
   
   outEllipse->center.x = box.center.x;
   outEllipse->center.y = box.center.y;
   outEllipse->size.height = box.size.height;
   outEllipse->size.width = box.size.width;
   outEllipse->angle = 90 - box.angle;

   free(PointArray); PointArray = NULL;

   return true;
}