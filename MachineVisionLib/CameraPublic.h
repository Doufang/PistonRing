#pragma once

#include <opencv.hpp>
#include <Public.h>

#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon/Image.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/InstantCamera.h>

#include <ListArrayLib/ListArrayLib.h>
#include <ImagePre/ImagePre.h>
#include <ObjectLib/ObjectLib.h>

#pragma comment(lib, "ListArrayLib.lib")
#pragma comment(lib, "ImagePre.lib")
#pragma comment(lib, "ObjectLib.lib")


#define VALUE_MIN(x,y) (((x)<(y))?(x):(y))
#define VALUE_MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_GRAY_VALUE 256
#define MIN_GRAY_VALUE 0

using namespace Pylon;
using namespace GenApi;
using namespace std;
using namespace cv;


#ifdef DLL_Camera
#define CameraAPI _declspec(dllexport) //导出类point
#else
#define CameraAPI _declspec(dllimport) //导入类point
#endif

struct MEASURERESULT
{
	float ippixel;
	float icircle;
	int group[3][3];
	int positon[3];
	float lastResult;
};

typedef MEASURERESULT CameraAPI MeasureResult;

int CameraAPI otsu(Mat dst);

//void CameraAPI MeasureMethod(Mat& img, MEASURERESULT *mearsureGroup, int funcFlag);

void CameraAPI JDDetect(IplImage* sourceImage);

bool CameraAPI checkLightNum(IplImage* ZoneImg, int LeakLightNum);

void CameraAPI cvRefineForegroundMaskBySegm(CvSeq* segments, IplImage*  pFrImg);

CvSeq CameraAPI *cvSegmentFGMask(CvArr* _mask, int poly1Hull0, float perimScale, CvMemStorage* storage, CvPoint offset);

bool CameraAPI CheckZoneLeak(IplImage* currImageBefore, CvRect Zone, int LeakLightNum);

void CameraAPI getMaxDistance(IplImage* img, ListPoint* line, int* a);

bool CameraAPI getMinYPositon(ListPoint* line, int firstPosition, int lastPosition, CvRect* zone, int* a, CvSize imgSize, int direction);

int CameraAPI OnNewvision(IplImage* currImageBefore, IplImage* maskImage);
