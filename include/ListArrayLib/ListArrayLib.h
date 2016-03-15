/*********************************************
所有的内存管理的封装函数---vector
***********************************************/

#ifndef _LISTARRAY_LIB_H_
#define _LISTARRAY_LIB_H_

#include "Public.h"

/****************Int结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayInt();

DLLAPI void STDCALL AddArrayInt(ArrayInt* IntArray, int IntValue);

DLLAPI void STDCALL InsertArrayInt(ArrayInt* IntArray, int InsertIndex, int IntValue);

DLLAPI void STDCALL EraseArrayInt(ArrayInt* IntArray, int Index);

DLLAPI void STDCALL ClearArrayInt(ArrayInt* IntArray);

DLLAPI void STDCALL FreeArrayInt(ArrayInt* IntArray);

DLLAPI void STDCALL CopyArrayInt(ArrayInt* dstArray, ArrayInt* srcArray);

/****************BlobInfo结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayBlobInfo();

DLLAPI void STDCALL AddArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, BlobInfo BlobInfoValue);

DLLAPI void STDCALL InsertArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, int InsertIndex, BlobInfo BlobInfoValue);

DLLAPI void STDCALL EraseArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, int Index);

DLLAPI void STDCALL ClearArrayBlobInfo(ArrayBlobInfo* BlobInfoArray);

DLLAPI void STDCALL FreeArrayBlobInfo(ArrayBlobInfo* BlobInfoArray);

DLLAPI void STDCALL CopyArrayBlobInfo(ArrayBlobInfo* dstArray, ArrayBlobInfo* srcArray);

/****************Char*结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayChar();

DLLAPI void STDCALL AddArrayChar(ArrayChar* charArray, char* charValue);

DLLAPI void STDCALL InsertArrayChar(ArrayChar* charArray, int InsertIndex, char* charValue);

DLLAPI void STDCALL EraseArrayChar(ArrayChar* charArray, int Index);

DLLAPI void STDCALL ClearArrayChar(ArrayChar* charArray);

DLLAPI void STDCALL FreeArrayChar(ArrayChar* charArray);

DLLAPI void STDCALL CopyArrayChar(ArrayChar* dstArray, ArrayChar* srcArray);

/****************Float结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayFloat();

DLLAPI void STDCALL AddArrayFloat(ArrayFloat* FloatArray, float FloatValue);

DLLAPI void STDCALL InsertArrayFloat(ArrayFloat* FloatArray, int InsertIndex, float FloatValue);

DLLAPI void STDCALL EraseArrayFloat(ArrayFloat* FloatArray, int Index);

DLLAPI void STDCALL ClearArrayFloat(ArrayFloat* FloatArray);

DLLAPI void STDCALL FreeArrayFloat(ArrayFloat* FloatArray);

DLLAPI void STDCALL CopyArrayFloat(ArrayFloat* dstArray, ArrayFloat* srcArray);

/****************Point结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPoint();

DLLAPI void STDCALL AddArrayPoint(ArrayPoint* PointArray, CvPoint Point);

DLLAPI void STDCALL InsertArrayPoint(ArrayPoint* PointArray, int InsertIndex, CvPoint Point);

DLLAPI void STDCALL EraseArrayPoint(ArrayPoint* PointArray, int Index);

DLLAPI void STDCALL ClearArrayPoint(ArrayPoint* PointArray);

DLLAPI void STDCALL FreeArrayPoint(ArrayPoint* PointArray);

DLLAPI void STDCALL CopyArrayPoint(ArrayPoint* dstArray, ArrayPoint* srcArray);
DLLAPI void STDCALL CopyArrayPointForNotFreeSrc(ArrayPoint* dstArray, ArrayPoint* srcArray);

/****************PointPair结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPointPair();

DLLAPI void STDCALL AddArrayPointPair(ArrayPointPair* PointArray, PointPair Point);

DLLAPI void STDCALL InsertArrayPointPair(ArrayPointPair* PointArray, int InsertIndex, PointPair Point);

DLLAPI void STDCALL EraseArrayPointPair(ArrayPointPair* PointArray, int Index);

DLLAPI void STDCALL ClearArrayPointPair(ArrayPointPair* PointArray);

DLLAPI void STDCALL FreeArrayPointPair(ArrayPointPair* PointArray);

DLLAPI void STDCALL CopyArrayPointPair(ArrayPointPair* dstArray, ArrayPointPair* srcArray);

/****************Point结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPointF();

DLLAPI void STDCALL AddArrayPointF(ArrayPointF* PointArray, CvPointF Point);

DLLAPI void STDCALL InsertArrayPointF(ArrayPointF* PointArray, int InsertIndex, CvPointF Point);

DLLAPI void STDCALL EraseArrayPointF(ArrayPointF* PointArray, int Index);

DLLAPI void STDCALL ClearArrayPointF(ArrayPointF* PointArray);

DLLAPI void STDCALL FreeArrayPointF(ArrayPointF* PointArray);

DLLAPI void STDCALL CopyArrayPointF(ArrayPointF* dstArray, ArrayPointF* srcArray);

/****************Line结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayLine();

DLLAPI void STDCALL AddArrayLine(ArrayLine* LineArray, cvLineObj Line);

DLLAPI void STDCALL InsertArrayLine(ArrayLine* LineArray, int InsertIndex, cvLineObj Line);

DLLAPI void STDCALL EraseArrayLine(ArrayLine* LineArray, int Index);

DLLAPI void STDCALL ClearArrayLine(ArrayLine* LineArray);

DLLAPI void STDCALL FreeArrayLine(ArrayLine* LineArray);

DLLAPI void STDCALL CopyArrayLine(ArrayLine* dstArray, ArrayLine* srcArray);

/****************Circle结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayCircle();

DLLAPI void STDCALL AddArrayCircle(ArrayCircle* CircleArray, cvCircleObj Circle);

DLLAPI void STDCALL InsertArrayCircle(ArrayCircle* CircleArray, int InsertIndex, cvCircleObj Circle);

DLLAPI void STDCALL EraseArrayCircle(ArrayCircle* CircleArray, int Index);

DLLAPI void STDCALL ClearArrayCircle(ArrayCircle* CircleArray);

DLLAPI void STDCALL FreeArrayCircle(ArrayCircle* CircleArray);

DLLAPI void STDCALL CopyArrayCircle(ArrayCircle* dstArray, ArrayCircle* srcArray);

/****************Rect结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayRect();

DLLAPI void STDCALL AddArrayRect(ArrayRect* RectArray, CvRect Rect);

DLLAPI void STDCALL InsertArrayRect(ArrayRect* RectArray, int InsertIndex, CvRect Rect);

DLLAPI void STDCALL EraseArrayRect(ArrayRect* RectArray, int Index);

DLLAPI void STDCALL ClearArrayRect(ArrayRect* RectArray);

DLLAPI void STDCALL FreeArrayRect(ArrayRect* RectArray);

DLLAPI void STDCALL CopyArrayRect(ArrayRect* dstArray, ArrayRect* srcArray);

/****************OCR结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayOCR();

DLLAPI void STDCALL AddArrayOCR(ArrayOCR* OCRArray, cvOCRObj OCR);

DLLAPI void STDCALL InsertArrayOCR(ArrayOCR* OCRArray, int InsertIndex, cvOCRObj OCR);

DLLAPI void STDCALL EraseArrayOCR(ArrayOCR* OCRArray, int Index);

DLLAPI void STDCALL ClearArrayOCR(ArrayOCR* OCRArray);

DLLAPI void STDCALL FreeArrayOCR(ArrayOCR* OCRArray);

DLLAPI void STDCALL CopyArrayOCR(ArrayOCR* dstArray, ArrayOCR* srcArray);

/****************RectObj结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayRectObj();

DLLAPI void STDCALL AddArrayRectObj(ArrayRectObj* RectArray, cvRectObj Rect);

DLLAPI void STDCALL InsertArrayRectObj(ArrayRectObj* RectArray, int InsertIndex, cvRectObj Rect);

DLLAPI void STDCALL EraseArrayRectObj(ArrayRectObj* RectArray, int Index);

DLLAPI void STDCALL ClearArrayRectObj(ArrayRectObj* RectArray);

DLLAPI void STDCALL FreeArrayRectObj(ArrayRectObj* RectArray);

DLLAPI void STDCALL CopyArrayRectObj(ArrayRectObj* dstArray, ArrayRectObj* srcArray);

/****************RectMsg结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayRectMsg();

DLLAPI void STDCALL AddArrayRectMsg(ArrayRectMsg* rectMsgArray, RectMsg rectMsg);

DLLAPI void STDCALL InsertArrayRectMsg(ArrayRectMsg* rectMsgArray, int InsertIndex, RectMsg rectMsg);

DLLAPI void STDCALL EraseArrayRectMsg(ArrayRectMsg* rectMsgArray, int Index);

DLLAPI void STDCALL ClearArrayRectMsg(ArrayRectMsg* rectMsgArray);

DLLAPI void STDCALL FreeArrayRectMsg(ArrayRectMsg* rectMsgArray);

DLLAPI void STDCALL CopyArrayRectMsg(ArrayRectMsg* dstArray, ArrayRectMsg* srcArray);

/****************PointPair结构的二维数组遍历函数*******************/
DLLAPI void* STDCALL MallocArrayTwoPointPair();

DLLAPI void STDCALL AddArrayTwoPointPair(ArrayTwoPointPair* PointArray, ArrayPointPair* PointPair);

DLLAPI void STDCALL InsertArrayTwoPointPair(ArrayTwoPointPair* PointArray, int InsertIndex, ArrayPointPair* PointPair);

DLLAPI void STDCALL EraseArrayTwoPointPair(ArrayTwoPointPair* PointArray, int Index);

DLLAPI void STDCALL ClearArrayTwoPointPair(ArrayTwoPointPair* PointArray);

DLLAPI void STDCALL FreeArrayTwoPointPair(ArrayTwoPointPair* PointArray);

#ifdef UPPROCESS
#ifndef LISTARRAYLIB_IMPLEMENT
#pragma comment(lib, "ListArrayLib.lib")
#endif
#endif

#endif //_LISTARRAY_LIB_H_