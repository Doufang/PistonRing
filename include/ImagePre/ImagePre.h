
#ifndef _IMAGEPRE_FILE_H_
#define _IMAGEPRE_FILE_H_

#include "Public.h"

DLLAPI void STDCALL AdaptiveStretchImage(IplImage* srcImage, CvRect rect, float lowValue,
										 float highValue, IplImage* dstImage);

DLLAPI void STDCALL MarkEdgeIplImageForRectBorder(IplImage* srcImage, CvRect rect, int ObjectGray, IplImage* EdgeImage);


DLLAPI void STDCALL TraceEdgeForIplImageModel(IplImage* srcImage, IplImage* MarkImage, int MarkValue, 
											  int hIndex, int wIndex, ListPoint* EdgePoint);

//��ȡ���б�Ե�����ص�����Ŀ���Ե����ӵ����ص㼯����
DLLAPI int STDCALL ExtractMaxEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect,
												 int ObjectGray, ListPoint* PointList);

//��ȡ���б�Ե�㣬��ӵ����ص㼯����
DLLAPI int STDCALL ExtractAllEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect,
												 int ObjectGray, ListPoint* PointList);

DLLAPI void STDCALL cvAdaptiveDevision(IplImage* srcImage, int hModel, int wModel, int DiffValue, IplImage* dstImage);

DLLAPI void STDCALL cvFixThreshold(IplImage* srcImage, int threshold, IplImage* dstImage);
DLLAPI void STDCALL cvFixThreshold_Inverse(IplImage* srcImage, int threshold, IplImage* dstImage);

DLLAPI void STDCALL cvFixThreshold_Three(IplImage* currImage, IplImage* currThreeImage,
										 int currThreshold, int currThreeThreshold, IplImage* dstImage);

DLLAPI bool STDCALL cvFixBottomThreshold_Three(IplImage* currImage, IplImage* currThreeImage,
											   int currThreshold, int currThreeThreshold, IplImage* dstImage);

DLLAPI void STDCALL cvAdaptiveDevisionImageByProb(IplImage* srcImage, float MinValue, float MaxValue, IplImage* dstImage);

DLLAPI void STDCALL OstuThreshold(IplImage* srcImage, IplImage* dstImage);
DLLAPI void STDCALL PathContrastForRectImage(IplImage* srcImage, CvRect rect, int PathValue, IplImage* dstImage);

DLLAPI void STDCALL PathContrastForMarkImage(IplImage* srcImage, IplImage* markImage, int ObjectGray, int PathValue, IplImage* dstImage);

DLLAPI bool STDCALL ProbDevisionImageByLow(IplImage* srcImage, float MinValue, float MaxValue, int deltaThreshold, int deltaValue, IplImage* dstImage);
DLLAPI void STDCALL ProbDevisionImageByHigh(IplImage* srcImage, float MaxValue, int deltaValue, IplImage* dstImage);


DLLAPI bool STDCALL FixThresholdForBottomImage(IplImage* srcImage, float MaxValue, int channelID, int deltaThreshold, int deltaValue, IplImage* dstImage);

/*********************************************************
              �����˲���Ľӿں���
**********************************************************/
//�˳�������ֵ�������Ŀ����
DLLAPI void STDCALL FilterHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
												   int MinMarkNum, int MaxMarkNum, int Objectgray, IplImage* dstImage);

//�˳������������Χ֮�������Ŀ����
DLLAPI void STDCALL FilterBlobNoiseForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
										   int MinMarkNum, int MaxMarkNum, IplImage* dstImage);

//�˳������÷�Χ֮�������Ŀ�����ʣ�ಿ����ӵ�ListRect��
DLLAPI void STDCALL FilterRegionListForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
											int MinMarkNum, int MaxMarkNum, IplImage* dstImage, ListRect* rectList, 
											ListInt* intList, ArrayPoint* PointArray);

DLLAPI void STDCALL FilterMaxHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
													  int Objectgray, IplImage* dstImage);

DLLAPI bool STDCALL FilterMaxRegionForImage(IplImage* srcImage, IplImage* MarkImage, int Objectgray, IplImage* dstImage);

DLLAPI void STDCALL FilterMaxRegionForRectImage(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, IplImage* dstImage);

DLLAPI void STDCALL FilterEdgeRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
											 int MaxMarkNum, IplImage* dstImage, ListRect* rectList, ListInt* intList);

DLLAPI void STDCALL FilterAllRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
											int MaxMarkNum, float HeightWidthRatio, bool IsSide,IplImage* dstImage, ListRect* rectList, ListInt* intList);

DLLAPI void STDCALL FilterAreaEdgeRatioForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, 
											   int MinAreaNum, int MaxAreaNum, IplImage* dstImage,
											   ListRect* rectList, ListInt* intList, ListFloat* floatList);

DLLAPI void STDCALL FilterMaxRectRegion(IplImage* srcImage, IplImage* MarkImage,int Objectgray, IplImage* dstImage, 
										CvRect* rectRegion, ListRect* rectList, ListInt* intList);

DLLAPI void STDCALL cvDilationImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage);
DLLAPI void STDCALL cvErosionImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage);


DLLAPI void STDCALL cvDrawSectorM(IplImage * image, cvDrawSector * DrawSector, CvPoint centerOut, CvPoint centerIn, CvSize axesOut, CvSize axesIn, double RotationAngleOut, double RotationAngleIn, double start_angleOut, double end_angleOut, double start_angleIn, double end_angleIn);


#ifdef UPPROCESS
#ifndef IMAGEPRE_IMPLEMENT
#pragma comment(lib, "ImagePre.lib")
#endif
#endif

#endif  //_IMAGEPRE_FILE_H_
