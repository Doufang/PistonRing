#ifndef _OBJECTLIB_H_
#define _OBJECTLIB_H_

#include "Public.h"

/*********************************************************
              所有对象类的接口函数
**********************************************************/
DLLAPI int STDCALL FitCircleObj(ListPoint PointSet, cvCircleObj* OutCircle);
DLLAPI void STDCALL RansacCirclePoint(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);
DLLAPI void STDCALL RansacCirclePointForLess(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);

DLLAPI void STDCALL FitLineObj(ListPoint PointSet, cvLineObj* outLine);
DLLAPI void STDCALL RansacLinePoint(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);
DLLAPI void STDCALL RansacLinePointForLess(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);
DLLAPI void STDCALL RansacLinePointForEdge(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);

DLLAPI void STDCALL GetConvexPointByListPoint(ListPoint* PointList, ListPoint* ConvexList);
DLLAPI void STDCALL GetMinimumEnclosedRectangle(ListPoint* PointList, cvRectObj* outRect);

DLLAPI void STDCALL QuickHullAlgorithmForListPoint(ListPoint* PointList, ListPoint* ConvexList);

DLLAPI void STDCALL RansacLinePointPair(ListPointPair PointPairSet, float errorThreshold, float errorAngle,
										ListPointPair* BestPointPair, ListPointPair* TempPointPair);

DLLAPI void STDCALL RansacLineTwoDimPointPair(ListPointPair PointPairSet, float errorThreshold, float errorAngle, 
											  ListPointPair* BestPointPair, ListPointPair* TempPointPair);

DLLAPI void STDCALL RansacPointPairClustering(ListTwoPointPair TwoPointPairList, float errorThreshold,
											  int& bestYIndex, int& bestXIndex, ListPointPair* BestPointPair);

DLLAPI void STDCALL RansacEllipsePoints(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint);
DLLAPI bool STDCALL FitEllipseObj(ListPoint PointSet, cvEllipseObj* outEllipse);

DLLAPI void STDCALL MergeRectListForInt(ListRect* RectList, ListInt* IntList, int deltaValue);
DLLAPI void STDCALL MergeRectListForHeight(ListRect* RectList, ListInt* IntList, int deltaValue);
DLLAPI void STDCALL MergeRectMsgList(ListRectMsg* RectList, int deltaValue);

DLLAPI bool STDCALL MergeRectListForThree(ListRect* RectList, CvRect& mergeRect, int MaxImageHeight, int MaxImageWidth, float deltaPercent);

DLLAPI void STDCALL SortPointsListByXValue(ListPoint* PointList);

#ifdef UPPROCESS
   #ifndef OBJECTLIB_IMPLEMENT
      #pragma comment(lib, "ObjectLib.lib")
   #endif
#endif

#endif