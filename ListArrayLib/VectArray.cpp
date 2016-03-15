/*********************************************
矢量vector的函数接口
***********************************************/

#include <ListArrayLib/ListArrayLib.h>

/****************Int结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayInt()
{
	ArrayInt* IntArray = new ArrayInt;

	return (void*)IntArray;
}

DLLAPI void STDCALL AddArrayInt(ArrayInt* IntArray, int IntValue)
{
	IntArray->push_back(IntValue);
}

DLLAPI void STDCALL InsertArrayInt(ArrayInt* IntArray, int InsertIndex, int IntValue)
{
	std::vector<int>::iterator Iter = IntArray->begin() + InsertIndex;

	IntArray->insert(Iter, IntValue);
}

DLLAPI void STDCALL EraseArrayInt(ArrayInt* IntArray, int Index)
{
	IntArray->erase(IntArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayInt(ArrayInt* IntArray)
{
	IntArray->clear();
}

DLLAPI void STDCALL FreeArrayInt(ArrayInt* IntArray)
{
	IntArray->clear();

	delete IntArray;
}

DLLAPI void STDCALL CopyArrayInt(ArrayInt* dstArray, ArrayInt* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for(UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************BlobInfo结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayBlobInfo()
{
	ArrayBlobInfo* BlobInfoArray = new ArrayBlobInfo;

	return (void*)BlobInfoArray;
}

DLLAPI void STDCALL AddArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, BlobInfo BlobInfoValue)
{
	BlobInfoArray->push_back(BlobInfoValue);
}

DLLAPI void STDCALL InsertArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, int InsertIndex, BlobInfo BlobInfoValue)
{
	std::vector<BlobInfo>::iterator Iter = BlobInfoArray->begin() + InsertIndex;

	BlobInfoArray->insert(Iter, BlobInfoValue);
}

DLLAPI void STDCALL EraseArrayBlobInfo(ArrayBlobInfo* BlobInfoArray, int Index)
{
	BlobInfoArray->erase(BlobInfoArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayBlobInfo(ArrayBlobInfo* BlobInfoArray)
{
	BlobInfoArray->clear();
}

DLLAPI void STDCALL FreeArrayBlobInfo(ArrayBlobInfo* BlobInfoArray)
{
	BlobInfoArray->clear();

	delete BlobInfoArray;
}

DLLAPI void STDCALL CopyArrayBlobInfo(ArrayBlobInfo* dstArray, ArrayBlobInfo* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Char结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayChar()
{
	ArrayChar* charArray = new ArrayChar;

	return (void*)charArray;
}

DLLAPI void STDCALL AddArrayChar(ArrayChar* charArray, char* charValue)
{
	charArray->push_back(charValue);
}

DLLAPI void STDCALL InsertArrayChar(ArrayChar* charArray, int InsertIndex, char* charValue)
{
	std::vector<char*>::iterator Iter = charArray->begin() + InsertIndex;

	charArray->insert(Iter, charValue);
}

DLLAPI void STDCALL EraseArrayChar(ArrayChar* charArray, int Index)
{
	charArray->erase(charArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayChar(ArrayChar* charArray)
{
	charArray->clear();
}

DLLAPI void STDCALL FreeArrayChar(ArrayChar* charArray)
{
	charArray->clear();

	delete charArray;
}

DLLAPI void STDCALL CopyArrayChar(ArrayChar* dstArray, ArrayChar* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Float结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayFloat()
{
	ArrayFloat* FloatArray = new ArrayFloat();

	return (void*)FloatArray;
}

DLLAPI void STDCALL AddArrayFloat(ArrayFloat* FloatArray, float FloatValue)
{
	FloatArray->push_back(FloatValue);
}

DLLAPI void STDCALL InsertArrayFloat(ArrayFloat* FloatArray, int InsertIndex, float FloatValue)
{
	std::vector<float>::iterator Iter = FloatArray->begin() + InsertIndex;

	FloatArray->insert(Iter, FloatValue);
}

DLLAPI void STDCALL EraseArrayFloat(ArrayFloat* FloatArray, int Index)
{
	FloatArray->erase(FloatArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayFloat(ArrayFloat* FloatArray)
{
	FloatArray->clear();
}

DLLAPI void STDCALL FreeArrayFloat(ArrayFloat* FloatArray)
{
	FloatArray->clear();

	delete FloatArray;
}

DLLAPI void STDCALL CopyArrayFloat(ArrayFloat* dstArray, ArrayFloat* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Point结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPoint()
{
	ArrayPoint* PointArray = new ArrayPoint();

	return (void*)PointArray;
}

DLLAPI void STDCALL AddArrayPoint(ArrayPoint* PointArray, CvPoint Point)
{
	PointArray->push_back(Point);
}

DLLAPI void STDCALL InsertArrayPoint(ArrayPoint* PointArray, int InsertIndex, CvPoint Point)
{
	std::vector<CvPoint>::iterator Iter = PointArray->begin() + InsertIndex;

	PointArray->insert(Iter, Point);
}

DLLAPI void STDCALL EraseArrayPoint(ArrayPoint* PointArray, int Index)
{
	PointArray->erase(PointArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayPoint(ArrayPoint* PointArray)
{
	PointArray->clear();
}

DLLAPI void STDCALL FreeArrayPoint(ArrayPoint* PointArray)
{
	PointArray->clear();

	delete PointArray;
}

DLLAPI void STDCALL CopyArrayPoint(ArrayPoint* dstArray, ArrayPoint* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

DLLAPI void STDCALL CopyArrayPointForNotFreeSrc(ArrayPoint* dstArray, ArrayPoint* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);
}

/****************PointPair结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPointPair()
{
	ArrayPointPair* PointArray = new ArrayPointPair();

	return (void*)PointArray;
}

DLLAPI void STDCALL AddArrayPointPair(ArrayPointPair* PointArray, PointPair Point)
{
	PointArray->push_back(Point);
}

DLLAPI void STDCALL InsertArrayPointPair(ArrayPointPair* PointArray, int InsertIndex, PointPair Point)
{
	std::vector<PointPair>::iterator Iter = PointArray->begin() + InsertIndex;

	PointArray->insert(Iter, Point);
}

DLLAPI void STDCALL EraseArrayPointPair(ArrayPointPair* PointArray, int Index)
{
	PointArray->erase(PointArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayPointPair(ArrayPointPair* PointArray)
{
	PointArray->clear();
}

DLLAPI void STDCALL FreeArrayPointPair(ArrayPointPair* PointArray)
{
	PointArray->clear();

	delete PointArray;
}

DLLAPI void STDCALL CopyArrayPointPair(ArrayPointPair* dstArray, ArrayPointPair* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************PointF结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayPointF()
{
	ArrayPointF* PointArray = new ArrayPointF();

	return (void*)PointArray;
}

DLLAPI void STDCALL AddArrayPointF(ArrayPointF* PointArray, CvPointF Point)
{
	PointArray->push_back(Point);
}

DLLAPI void STDCALL InsertArrayPointF(ArrayPointF* PointArray, int InsertIndex, CvPointF Point)
{
	std::vector<CvPointF>::iterator Iter = PointArray->begin() + InsertIndex;

	PointArray->insert(Iter, Point);
}

DLLAPI void STDCALL EraseArrayPointF(ArrayPointF* PointArray, int Index)
{
	PointArray->erase(PointArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayPointF(ArrayPointF* PointArray)
{
	PointArray->clear();
}

DLLAPI void STDCALL FreeArrayPointF(ArrayPointF* PointArray)
{
	PointArray->clear();

	delete PointArray;
}

DLLAPI void STDCALL CopyArrayPointF(ArrayPointF* dstArray, ArrayPointF* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Line结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayLine()
{
	ArrayLine* LineArray = new ArrayLine();

	return (void*)LineArray;
}

DLLAPI void STDCALL AddArrayLine(ArrayLine* LineArray, cvLineObj Line)
{
	LineArray->push_back(Line);
}

DLLAPI void STDCALL InsertArrayLine(ArrayLine* LineArray, int InsertIndex, cvLineObj Line)
{
	std::vector<cvLineObj>::iterator Iter = LineArray->begin() + InsertIndex;

	LineArray->insert(Iter, Line);
}

DLLAPI void STDCALL EraseArrayLine(ArrayLine* LineArray, int Index)
{
	LineArray->erase(LineArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayLine(ArrayLine* LineArray)
{
	LineArray->clear();
}

DLLAPI void STDCALL FreeArrayLine(ArrayLine* LineArray)
{
	LineArray->clear();

	delete LineArray;
}

DLLAPI void STDCALL CopyArrayLine(ArrayLine* dstArray, ArrayLine* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Circle结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayCircle()
{
	ArrayCircle* CircleArray = new ArrayCircle();

	return CircleArray;
}

DLLAPI void STDCALL AddArrayCircle(ArrayCircle* CircleArray, cvCircleObj Circle)
{
	CircleArray->push_back(Circle);
}

DLLAPI void STDCALL InsertArrayCircle(ArrayCircle* CircleArray, int InsertIndex, cvCircleObj Circle)
{
	std::vector<cvCircleObj>::iterator Iter = CircleArray->begin() + InsertIndex;

	CircleArray->insert(Iter, Circle);
}

DLLAPI void STDCALL EraseArrayCircle(ArrayCircle* CircleArray, int Index)
{
	CircleArray->erase(CircleArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayCircle(ArrayCircle* CircleArray)
{
	CircleArray->clear();
}

DLLAPI void STDCALL FreeArrayCircle(ArrayCircle* CircleArray)
{
	CircleArray->clear();

	delete CircleArray;
}

DLLAPI void STDCALL CopyArrayCircle(ArrayCircle* dstArray, ArrayCircle* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************Rect结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayRect()
{
	ArrayRect* RectArray = new ArrayRect();

	return (void*)RectArray;
}

DLLAPI void STDCALL AddArrayRect(ArrayRect* RectArray, CvRect Rect)
{
	RectArray->push_back(Rect);
}

DLLAPI void STDCALL InsertArrayRect(ArrayRect* RectArray, int InsertIndex, CvRect Rect)
{
	std::vector<CvRect>::iterator Iter = RectArray->begin() + InsertIndex;

	RectArray->insert(Iter, Rect);
}

DLLAPI void STDCALL EraseArrayRect(ArrayRect* RectArray, int Index)
{
	RectArray->erase(RectArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayRect(ArrayRect* RectArray)
{
	RectArray->clear();
}

DLLAPI void STDCALL FreeArrayRect(ArrayRect* RectArray)
{
	RectArray->clear();

	delete RectArray;
}

DLLAPI void STDCALL CopyArrayRect(ArrayRect* dstArray, ArrayRect* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************OCR结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayOCR()
{
	ArrayOCR* OCRArray = new ArrayOCR();

	return (void*)OCRArray;
}

DLLAPI void STDCALL AddArrayOCR(ArrayOCR* OCRArray, cvOCRObj OCR)
{
	OCRArray->push_back(OCR);
}

DLLAPI void STDCALL InsertArrayOCR(ArrayOCR* OCRArray, int InsertIndex, cvOCRObj OCR)
{
	std::vector<cvOCRObj>::iterator Iter = OCRArray->begin() + InsertIndex;

	OCRArray->insert(Iter, OCR);
}

DLLAPI void STDCALL EraseArrayOCR(ArrayOCR* OCRArray, int Index)
{
	OCRArray->erase(OCRArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayOCR(ArrayOCR* OCRArray)
{
	OCRArray->clear();
}

DLLAPI void STDCALL FreeArrayOCR(ArrayOCR* OCRArray)
{
	OCRArray->clear();

	delete OCRArray;
}

DLLAPI void STDCALL CopyArrayOCR(ArrayOCR* dstArray, ArrayOCR* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

/****************RectObj结构的遍历函数*******************/
DLLAPI void* STDCALL MallocArrayRectObj()
{
	ArrayRectObj* RectArray = new ArrayRectObj();

	return (void*)RectArray;
}

DLLAPI void STDCALL AddArrayRectObj(ArrayRectObj* RectArray, cvRectObj Rect)
{
	RectArray->push_back(Rect);
}

DLLAPI void STDCALL InsertArrayRectObj(ArrayRectObj* RectArray, int InsertIndex, cvRectObj Rect)
{
	std::vector<cvRectObj>::iterator Iter = RectArray->begin() + InsertIndex;

	RectArray->insert(Iter, Rect);
}

DLLAPI void STDCALL EraseArrayRectObj(ArrayRectObj* RectArray, int Index)
{
	RectArray->erase(RectArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayRectObj(ArrayRectObj* RectArray)
{
	RectArray->clear();
}

DLLAPI void STDCALL FreeArrayRectObj(ArrayRectObj* RectArray)
{
	RectArray->clear();

	delete RectArray;
}

DLLAPI void STDCALL CopyArrayRectObj(ArrayRectObj* dstArray, ArrayRectObj* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

DLLAPI void* STDCALL MallocArrayRectMsg()
{
	ArrayRectMsg* rectMsgArray = new ArrayRectMsg();

	return (void*)rectMsgArray;
}

DLLAPI void STDCALL AddArrayRectMsg(ArrayRectMsg* rectMsgArray, RectMsg rectMsg)
{
	rectMsgArray->push_back(rectMsg);
}

DLLAPI void STDCALL InsertArrayRectMsg(ArrayRectMsg* rectMsgArray, int InsertIndex, RectMsg rectMsg)
{
	std::vector<RectMsg>::iterator Iter = rectMsgArray->begin() + InsertIndex;

	rectMsgArray->insert(Iter, rectMsg);
}

DLLAPI void STDCALL EraseArrayRectMsg(ArrayRectMsg* rectMsgArray, int Index)
{
	rectMsgArray->erase(rectMsgArray->begin() + Index);
}

DLLAPI void STDCALL ClearArrayRectMsg(ArrayRectMsg* rectMsgArray)
{
	rectMsgArray->clear();
}

DLLAPI void STDCALL FreeArrayRectMsg(ArrayRectMsg* rectMsgArray)
{
	rectMsgArray->clear();

	delete rectMsgArray;
}

DLLAPI void STDCALL CopyArrayRectMsg(ArrayRectMsg* dstArray, ArrayRectMsg* srcArray)
{
	//先将dstArray中的元素清除
	dstArray->clear();

	for (UINT dIndex = 0; dIndex < srcArray->size(); dIndex++)
		dstArray->push_back((*srcArray)[dIndex]);

	//清除srcArray的所有元素
	srcArray->clear();
}

//开辟二维数组头指针
DLLAPI void* STDCALL MallocArrayTwoPointPair()
{
	ArrayTwoPointPair* twoPointPairArray = new ArrayTwoPointPair();

	return (void*)twoPointPairArray;
}

//向二维数组中添加对应的一维数组
DLLAPI void STDCALL AddArrayTwoPointPair(ArrayTwoPointPair* PointArray, ArrayPointPair* PointPairArray)
{
	PointArray->push_back(PointPairArray);
}

//向二维数组中插入对应的一维数组
DLLAPI void STDCALL InsertArrayTwoPointPair(ArrayTwoPointPair* PointArray, int InsertIndex, ArrayPointPair* PointPairArray)
{
	std::vector<ArrayPointPair*>::iterator Iter = PointArray->begin() + InsertIndex;

	PointArray->insert(Iter, PointPairArray);
}

//从二维数组中擦除指定位置的一维数组
DLLAPI void STDCALL EraseArrayTwoPointPair(ArrayTwoPointPair* PointArray, int Index)
{
	PointArray->erase(PointArray->begin() + Index);
}

//从二维数组中清除对应的所有的元素
DLLAPI void STDCALL ClearArrayTwoPointPair(ArrayTwoPointPair* PointArray)
{
	//将对应的一维向量释放
	for (int hIndex = PointArray->size(); hIndex >= 0; hIndex--)
	{
		ArrayPointPair* Iter = (*PointArray)[hIndex];

		Iter->clear();
	}

	PointArray->clear();
}

//释放二维数组中的所有元素
DLLAPI void STDCALL FreeArrayTwoPointPair(ArrayTwoPointPair* PointArray)
{
	for (int hIndex = PointArray->size() - 1; hIndex >= 0; hIndex--)
	{
		ArrayPointPair* Iter = (*PointArray)[hIndex];

		Iter->clear();	delete Iter;
	}

	PointArray->clear(); delete PointArray;
}