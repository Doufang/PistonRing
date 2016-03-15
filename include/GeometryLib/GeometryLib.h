#ifndef _GEOMETRYLIB_H_
#define _GEOMETRYLIB_H_

#include "Public.h"

/*********************************************************
              所有几何类的接口函数
**********************************************************/
DLLAPI float     STDCALL DotDotDis(CvPoint Point1, CvPoint Point2);
DLLAPI cvLineObj STDCALL DotDotLine(CvPoint Point1, CvPoint Point2);
DLLAPI float     STDCALL DotDotAngle(CvPoint Point1, CvPoint Point2, int AngleRange);
DLLAPI float     STDCALL DotLineDis(CvPoint Point, cvLineObj InLine);
DLLAPI float     STDCALL LineLineDis(cvLineObj InLine1, cvLineObj InLine2);
DLLAPI float     STDCALL DotLineDisAbs(CvPoint Point, cvLineObj InLine);
DLLAPI float     STDCALL LineLineDotDis(cvLineObj InLine1, cvLineObj InLine2);
DLLAPI CvPoint   STDCALL LineLineIntersection(cvLineObj InLine1, cvLineObj InLine2);
DLLAPI float     STDCALL LineLineIncludeAngle(cvLineObj InLine1, cvLineObj InLine2, int AngleRangle);

#ifdef UPPROCESS
   #ifndef GEOMETRYLIB_IMPLEMENT
      #pragma comment(lib, "GeometryLib.lib")
   #endif
#endif

#endif  //_GEOMETRYLIB_H_