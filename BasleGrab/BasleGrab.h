#ifndef _BASLEGRAB_H_
#define _BASLEGRAB_H_

#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon/Image.h>

#include <pylon/InstantCamera.h>

#include <iostream>
#include <opencv2/opencv.hpp>

#include <ListArrayLib/ListArrayLib.h>
#pragma comment(lib, "ListArrayLib.lib")

#ifdef UPPROCESS
	#ifndef BASLEGRAB_IMPLEMENT
		#pragma comment(lib, "BasleGrab.lib")
	#endif
#endif


//#define DLLAPI extern "C" 
//#define STDCALL _stdcall

using namespace Pylon;

using namespace GenApi;

using namespace std;

using namespace cv;


DLLAPI int STDCALL GrabImage(int a);
DLLAPI void STDCALL cvDilationImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage);
DLLAPI int STDCALL FilterAllRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
										   int MaxMarkNum, float HeightWidthRatio, bool IsSide, IplImage* dstImage, ListRect* rectList, ListInt* intList);


int otsu(IplImage* dst);
void ShowImage( Pylon::IImage& image, const char* message);//为什么第二个参数后面不能添加=NULL

#endif  //_BASLEGRAB_H_