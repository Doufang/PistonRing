/*********************************************
基于传入的边缘图像、灰度图像提取线段对象
***********************************************/
#include <ListArrayLib/ListArrayLib.h>
#include <ObjectLib/ObjectLib.h>
#include <GeometryLib/GeometryLib.h>

/*********************************************************
线段范围内和范围外,传入的BestPoint和TempPoint都应该为空
**********************************************************/
void EstimatePointPair(ListPointPair PointSet, float errorThreshold, float errorAngle, cvLineObj currLine, 
					   ArrayPointPair* BestPointPair, ArrayPointPair* TempPointPair)
{
	float angleValue = 0;

	PointPair dPointPair; cvLineObj TempLine;

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		dPointPair = (*PointSet.Item)[hIndex];

		TempLine = DotDotLine(dPointPair.normPoint, dPointPair.currPoint);

		angleValue = LineLineIncludeAngle(currLine, TempLine, 0);   //-90到90度范围

		if(abs(angleValue) > errorAngle)
		{
			AddArrayPointPair(TempPointPair, dPointPair);
			continue;
		}

		//两组点集之间的距离不超过指定的差值
		if(abs(currLine.LineLength - TempLine.LineLength) > errorThreshold)
		{
			AddArrayPointPair(TempPointPair, dPointPair);
			continue;
		}

		AddArrayPointPair(BestPointPair, dPointPair);
	}
}

/************************************************
基于传入的点集合进行直线评估，得到拟合线段的最优组点
*************************************************/
DLLAPI void STDCALL RansacLinePointPair(ListPointPair PointPairSet, float errorThreshold, float errorAngle, 
										ListPointPair* BestPointPair, ListPointPair* TempPointPair)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	cvLineObj TempLine;  PointPair dPointPair;

	ArrayPointPair* dBestPointPair = (ArrayPointPair*)MallocArrayPointPair();
	ArrayPointPair* dTempPointPair = (ArrayPointPair*)MallocArrayPointPair();

	for (hIndex = 0; hIndex < PointPairSet.Item->size(); hIndex++)
	{
		dPointPair = (*PointPairSet.Item)[hIndex];

		//以当期序号的点集组成线段，评估直线的比例
		TempLine = DotDotLine(dPointPair.normPoint, dPointPair.currPoint);

		EstimatePointPair(PointPairSet, errorThreshold, errorAngle, TempLine, dBestPointPair, dTempPointPair);

		if(dBestPointPair->size() > BestPointPair->Item->size())
		{
			CopyArrayPointPair(BestPointPair->Item, dBestPointPair);
			CopyArrayPointPair(TempPointPair->Item, dTempPointPair);
		}
		else
		{
			ClearArrayPointPair(dBestPointPair);
			ClearArrayPointPair(dTempPointPair);
		}
	}//0---1/3之后部分的图像构建

	FreeArrayPointPair(dBestPointPair);
	FreeArrayPointPair(dTempPointPair);
}

/*********************************************************
线段范围内和范围外,传入的BestPoint和TempPoint都应该为空
**********************************************************/
void EstimateTwoDimPointPair(ListPointPair PointSet, float errorThreshold, float errorAngle, PointPair normPointPair, 
							 ArrayPointPair* BestPointPair, ArrayPointPair* TempPointPair)
{
	float angleValue = 0;

	PointPair currPointPair; cvLineObj normLine, currLine;

	int normDeltaYValue = 0, currDeltaYValue = 0;

	normLine = DotDotLine(normPointPair.normPoint, normPointPair.currPoint);

	normDeltaYValue = normPointPair.currPoint.y - normPointPair.normPoint.y;

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		currPointPair = (*PointSet.Item)[hIndex];

		currLine = DotDotLine(currPointPair.normPoint, currPointPair.currPoint);

		angleValue = LineLineIncludeAngle(currLine, normLine, 0);   //-90到90度范围

		if(abs(angleValue) > errorAngle)
		{
			AddArrayPointPair(TempPointPair, currPointPair);
			continue;
		}

		currDeltaYValue = currPointPair.currPoint.y - currPointPair.normPoint.y;

		//两组点集之间的距离不超过指定的差值
		if(abs(currDeltaYValue - normDeltaYValue) > errorThreshold)
		{
			AddArrayPointPair(TempPointPair, currPointPair);
			continue;
		}

		if(currDeltaYValue*normDeltaYValue < 0)    //表示当前与标准符合相反
		{
			AddArrayPointPair(TempPointPair, currPointPair);
			continue;
		}

		AddArrayPointPair(BestPointPair, currPointPair);
	}
}

/************************************************
基于传入的点集合进行直线评估，得到拟合线段的最优组点
*************************************************/
DLLAPI void STDCALL RansacLineTwoDimPointPair(ListPointPair PointPairSet, float errorThreshold, float errorAngle, 
											  ListPointPair* BestPointPair, ListPointPair* TempPointPair)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	cvLineObj TempLine;  PointPair dPointPair;

	ArrayPointPair* dBestPointPair = (ArrayPointPair*)MallocArrayPointPair();
	ArrayPointPair* dTempPointPair = (ArrayPointPair*)MallocArrayPointPair();

	for (hIndex = 0; hIndex < PointPairSet.Item->size(); hIndex++)
	{
		dPointPair = (*PointPairSet.Item)[hIndex];

		//以当期序号的点集组成线段，评估直线的比例
		TempLine = DotDotLine(dPointPair.normPoint, dPointPair.currPoint);

		EstimateTwoDimPointPair(PointPairSet, errorThreshold, errorAngle, dPointPair, dBestPointPair, dTempPointPair);

		if(dBestPointPair->size() > BestPointPair->Item->size())
		{
			CopyArrayPointPair(BestPointPair->Item, dBestPointPair);
			CopyArrayPointPair(TempPointPair->Item, dTempPointPair);
		}
		else
		{
			ClearArrayPointPair(dBestPointPair);
			ClearArrayPointPair(dTempPointPair);
		}
	}//0---1/3之后部分的图像构建

	FreeArrayPointPair(dBestPointPair);
	FreeArrayPointPair(dTempPointPair);
}

/*********************************************************
线段范围内和范围外,传入的BestPoint和TempPoint都应该为空
**********************************************************/
void EstimatePointPairClustering(ListTwoPointPair TwoPointPairList, float errorThreshold, PointPair normPointPair, ArrayPointPair* BestPointPair)
{
	PointPair currPointPair; int hIndex = 0, wIndex = 0;

	int currDeltaXValue = 0, currDeltaYValue = 0;
	int normDeltaXValue = 0, normDeltaYValue = 0;
	
	for (hIndex = 0; hIndex < TwoPointPairList.Item->size(); hIndex++)
	{
		ArrayPointPair* currIter = (*TwoPointPairList.Item)[hIndex];

		for (wIndex = 0; wIndex < currIter->size(); wIndex++)
		{
			currPointPair = (*currIter)[wIndex];

			normDeltaXValue = abs(currPointPair.normPoint.x - normPointPair.normPoint.x);
			normDeltaYValue = abs(currPointPair.normPoint.y - normPointPair.normPoint.y);

			//若标准差值超过阈值
			if(normDeltaXValue > errorThreshold || normDeltaYValue > errorThreshold)
				continue;

			currDeltaXValue = abs(currPointPair.currPoint.x - normPointPair.currPoint.x);
			currDeltaYValue = abs(currPointPair.currPoint.y - normPointPair.currPoint.y);

			if(currDeltaXValue > errorThreshold || currDeltaYValue > errorThreshold)
				continue;
			
			AddArrayPointPair(BestPointPair, currPointPair);
		}
	}
}

/************************************************
       判断点集合的聚合度---获取最高聚合度
*************************************************/
DLLAPI void STDCALL RansacPointPairClustering(ListTwoPointPair TwoPointPairList, float errorThreshold,
											  int& bestYIndex, int& bestXIndex, ListPointPair* BestPointPair)
{
	int hIndex = 0, wIndex = 0;	PointPair dPointPair;

	ArrayPointPair* dBestPointPair = (ArrayPointPair*)MallocArrayPointPair();

	for (hIndex = 0; hIndex < TwoPointPairList.Item->size(); hIndex++)
	{
		ArrayPointPair* currIter = (*TwoPointPairList.Item)[hIndex];

		for (wIndex = 0; wIndex < currIter->size(); wIndex++)
		{
			dPointPair = (*currIter)[wIndex];

			EstimatePointPairClustering(TwoPointPairList, errorThreshold, dPointPair, dBestPointPair);

			if(dBestPointPair->size() > BestPointPair->Item->size())
			{
				//记录当前最有对应的信息
				CopyArrayPointPair(BestPointPair->Item, dBestPointPair);
				bestYIndex = hIndex; bestXIndex = wIndex;
			}
			else
			{
				ClearArrayPointPair(dBestPointPair);
			}
		}		
	}//0---1/3之后部分的图像构建

	FreeArrayPointPair(dBestPointPair);
}