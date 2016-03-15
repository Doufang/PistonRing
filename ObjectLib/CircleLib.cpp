#include <ObjectLib/ObjectLib.h>
#include <ListArrayLib/ListArrayLib.h>

#define CIRCLEDIS        4.0
#define MAXCIRCLEVALUE   8000

void EstimateCircle(ListPoint PointSet, cvCircleObj Circle, ArrayPoint* BestPoint, ArrayPoint* TempPoint)
{
	float dis = 0;

	CvPoint dTempPoint;

	float MinDis = (Circle.Radius - CIRCLEDIS)*(Circle.Radius - CIRCLEDIS);
	float MaxDis = (Circle.Radius + CIRCLEDIS)*(Circle.Radius + CIRCLEDIS);

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		dTempPoint = (*PointSet.Item)[hIndex];

		dis = (dTempPoint.x - Circle.CirclePoint.x)*(dTempPoint.x - Circle.CirclePoint.x)
			+ (dTempPoint.y - Circle.CirclePoint.y)*(dTempPoint.y - Circle.CirclePoint.y);

		if(dis >= MinDis && dis <= MaxDis)
			AddArrayPoint(BestPoint, dTempPoint);
		else
			AddArrayPoint(TempPoint, dTempPoint);
	}
}

int DotThreeCircle(CvPoint Point1, CvPoint Point2, CvPoint Point3, cvCircleObj* OutCircle)
{
	float SumX1Value = Point1.x + Point2.x + Point3.x;
	float SumY1Value = Point1.y + Point2.y + Point3.y;

	float SumX2Value = Point1.x*Point1.x + Point2.x*Point2.x + Point3.x*Point3.x;
	float SumY2Value = Point1.y*Point1.y + Point2.y*Point2.y + Point3.y*Point3.y;

	float SumX3Value = Point1.x*Point1.x*Point1.x + Point2.x*Point2.x*Point2.x + Point3.x*Point3.x*Point3.x;
	float SumY3Value = Point1.y*Point1.y*Point1.y + Point2.y*Point2.y*Point2.y + Point3.y*Point3.y*Point3.y;

	float SumX1Y1Value = Point1.x*Point1.y + Point2.x*Point2.y + Point3.x*Point3.y;
	float SumX1Y2Value = Point1.x*Point1.y*Point1.y + Point2.x*Point2.y*Point2.y + Point3.x*Point3.y*Point3.y;
	float SumX2Y1Value = Point1.x*Point1.x*Point1.y + Point2.x*Point2.x*Point2.y + Point3.x*Point3.x*Point3.y;

	int N = 3, result = 1;

	float C = N*SumX2Value - SumX1Value*SumX1Value;
	float D = N*SumX1Y1Value - SumX1Value*SumY1Value;
	float E = N*SumX3Value + N*SumX1Y2Value - (SumX2Value + SumY2Value)*SumX1Value;
	float G = N*SumY2Value - SumY1Value*SumY1Value;
	float H = N*SumX2Y1Value + N*SumY3Value - (SumX2Value + SumY2Value)*SumY1Value;

	float A1 = 0, B1 = 0, C1 = 0;

	if(C*G - D*D < 1 && C*G - D*D > -1)
	{
		A1 = 0; B1 = 0; C1 = 0;

		result = 0;
	}
	else
	{
		A1 = (H*D-E*G)/(C*G-D*D);
		B1 = (H*C-E*D)/(D*D-G*C);
		C1 = -(A1*SumX1Value + B1*SumY1Value + SumX2Value + SumY2Value)/N;
	}

	OutCircle->CirclePoint.x = -A1/2;
	OutCircle->CirclePoint.y = -B1/2;
	OutCircle->Radius = sqrtf(A1*A1 + B1*B1 - 4*C1)/2;

	return result;
}

/************************************************
���ڴ���ĵ㼯�Ͻ���Բ���������õ����Բ�ܵ��������
*************************************************/
DLLAPI void STDCALL RansacCirclePoint(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, IsCircleObj = 0;

	cvCircleObj TempCircle;

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	int TempValue = PointSet.Item->size()*0.30;

	for (hIndex = 0; hIndex < TempValue; hIndex++)
	{
		wIndex = 0.60*PointSet.Item->size() + hIndex - 2;

		dIndex = (hIndex + wIndex)/2;

		//�Ե�����ŵĵ㼯���Բ�ܣ�����Բ�ܵı���
		/*if(hIndex < 0 || hIndex >= PointSet.Item->size()
			|| dIndex < 0 || dIndex >= PointSet.Item->size()
			|| wIndex < 0 || wIndex >= PointSet.Item->size())
			continue;*/

		if(!DotThreeCircle((*PointSet.Item)[hIndex], (*PointSet.Item)[dIndex], (*PointSet.Item)[wIndex], &TempCircle))
			continue;

		if(TempCircle.Radius > MAXCIRCLEVALUE)
			continue;

		EstimateCircle(PointSet, TempCircle, dBestPoint, dTempPoint);

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
	}//0---1/3֮�󲿷ֵ�ͼ�񹹽�

	for (hIndex = 0; hIndex < TempValue; hIndex++)
	{
		wIndex = PointSet.Item->size()*0.40 + hIndex - 2;

		dIndex = (hIndex + wIndex)/2;

		//�Ե�����ŵĵ㼯���Բ�ܣ�����Բ�ܵı���
		/*if(hIndex < 0 || hIndex >= PointSet.Item->size()
			|| dIndex < 0 || dIndex >= PointSet.Item->size()
			|| wIndex < 0 || wIndex >= PointSet.Item->size())
			continue;*/

		if(!DotThreeCircle((*PointSet.Item)[hIndex], (*PointSet.Item)[dIndex], (*PointSet.Item)[wIndex], &TempCircle))
			continue;

		if(TempCircle.Radius > MAXCIRCLEVALUE)
			continue;

		EstimateCircle(PointSet, TempCircle, dBestPoint, dTempPoint);

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
	}//0---1/3֮�󲿷ֵ�ͼ�񹹽�

	for (hIndex = TempValue; hIndex < PointSet.Item->size()*0.56; hIndex++)
	{
		wIndex = PointSet.Item->size()*0.70 + (hIndex - TempValue);

		dIndex = (hIndex + wIndex)/2;

		/*if(hIndex < 0 || hIndex >= PointSet.Item->size()
			|| dIndex < 0 || dIndex >= PointSet.Item->size()
			|| wIndex < 0 || wIndex >= PointSet.Item->size())
			continue;*/

		//�Ե�����ŵĵ㼯���Բ�ܣ�����Բ�ܵı���
		if(!DotThreeCircle((*PointSet.Item)[hIndex], (*PointSet.Item)[dIndex], (*PointSet.Item)[wIndex], &TempCircle))
			continue;

		if(TempCircle.Radius > MAXCIRCLEVALUE)
			continue;

		EstimateCircle(PointSet, TempCircle, dBestPoint, dTempPoint);

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
	}//0---1/3֮�󲿷ֵ�ͼ�񹹽�

	FreeArrayPoint(dBestPoint);
	FreeArrayPoint(dTempPoint);
}

DLLAPI void STDCALL RansacCirclePointForLess(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, IsCircleObj = 0;

	cvCircleObj TempCircle;

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	int TempValue = PointSet.Item->size()*0.30;

	for (hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		for (wIndex = hIndex + 0.1*PointSet.Item->size(); wIndex < PointSet.Item->size(); wIndex++)
		{
			dIndex = (hIndex + wIndex)/2;

			//�Ե�����ŵĵ㼯���Բ�ܣ�����Բ�ܵı���
			/*if(hIndex < 0 || hIndex >= PointSet.Item->size()
				|| dIndex < 0 || dIndex >= PointSet.Item->size()
				|| wIndex < 0 || wIndex >= PointSet.Item->size())
				continue;*/

			if(!DotThreeCircle((*PointSet.Item)[hIndex], (*PointSet.Item)[dIndex], (*PointSet.Item)[wIndex], &TempCircle))
				continue;

			if(TempCircle.Radius > MAXCIRCLEVALUE)
				continue;

			EstimateCircle(PointSet, TempCircle, dBestPoint, dTempPoint);

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
	}//0---1/3֮�󲿷ֵ�ͼ�񹹽�
}

/************************************************
���ڴ���ĵ㼯����Բ��������ϣ��õ�Բ�ܵĲ���
*************************************************/
DLLAPI int STDCALL FitCircleObj(ListPoint PointSet, cvCircleObj* OutCircle)
{
	float SumX1Value = 0, SumY1Value = 0, SumX2Value = 0, SumY2Value = 0;
	float SumX3Value = 0, SumY3Value = 0, SumX1Y1Value = 0, SumX1Y2Value = 0, SumX2Y1Value = 0;

	CvPoint TempPoint;

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		TempPoint = (*PointSet.Item)[hIndex];

		SumX1Value += TempPoint.x;
		SumY1Value += TempPoint.y;

		SumX2Value += TempPoint.x*TempPoint.x;
		SumY2Value += TempPoint.y*TempPoint.y;

		SumX3Value += TempPoint.x*TempPoint.x*TempPoint.x;
		SumY3Value += TempPoint.y*TempPoint.y*TempPoint.y;

		SumX1Y1Value += TempPoint.x*TempPoint.y;
		SumX1Y2Value += TempPoint.x*TempPoint.y*TempPoint.y;
		SumX2Y1Value += TempPoint.x*TempPoint.x*TempPoint.y;
	}

	int N = PointSet.Item->size(), result = 1;

	float C = N*SumX2Value - SumX1Value*SumX1Value;
	float D = N*SumX1Y1Value - SumX1Value*SumY1Value;
	float E = N*SumX3Value + N*SumX1Y2Value - (SumX2Value + SumY2Value)*SumX1Value;
	float G = N*SumY2Value - SumY1Value*SumY1Value;
	float H = N*SumX2Y1Value + N*SumY3Value - (SumX2Value + SumY2Value)*SumY1Value;

	float A1 = 0, B1 = 0, C1 = 0;

	if(C*G - D*D < 1 && C*G - D*D > -1)
	{
		A1 = 0; B1 = 0; C1 = 0;

		result = 0;
	}
	else
	{
		A1 = (H*D-E*G)/(C*G-D*D);
		B1 = (H*C-E*D)/(D*D-G*C);
		C1 = -(A1*SumX1Value + B1*SumY1Value + SumX2Value + SumY2Value)/N;
	}

	OutCircle->CirclePoint.x = -A1/2;
	OutCircle->CirclePoint.y = -B1/2;
	OutCircle->Radius = sqrtf(A1*A1 + B1*B1 - 4*C1)/2;

	//Բ�ܵıպ϶�---ʹ����ϵĵ㼯����2*PI*R�ı�ֵ
	OutCircle->CloseDegree = (PointSet.Item->size()*360/(2*VL_PI*OutCircle->Radius));

	if(OutCircle->CloseDegree > 360)
		OutCircle->CloseDegree = 360;

	//��¼��ǰԲ�ܵ��ۻ���ʱ����
	OutCircle->SumValue[0] = N;

	OutCircle->SumValue[1] = SumX1Value;
	OutCircle->SumValue[2] = SumY1Value;

	OutCircle->SumValue[3] = SumX2Value;
	OutCircle->SumValue[4] = SumY2Value;

	OutCircle->SumValue[5] = SumX3Value;
	OutCircle->SumValue[6] = SumY3Value;

	OutCircle->SumValue[7] = SumX1Y1Value;
	OutCircle->SumValue[8] = SumX1Y2Value;

	OutCircle->SumValue[9] = SumX2Y1Value;

	return result;
}


DLLAPI void STDCALL SortPointsListByXValue(ListPoint* PointList)
{
	ListPoint TempRectPoint; CvPoint hTempPoint, wTempPoint;

	bool IsInsertPoint = false;

	TempRectPoint.Item = (ArrayPoint*)MallocArrayPoint();

	//�������PointList��ListArray��̬���
	for (int hIndex = 0; hIndex < PointList->Item->size(); hIndex++)
	{
		hTempPoint = (*PointList->Item)[hIndex];

		IsInsertPoint = false;

		if(TempRectPoint.Item->size() == 0)
			AddArrayPoint(TempRectPoint.Item, hTempPoint);
		else
		{
			//����X����������
			for (int wIndex = 0; wIndex < TempRectPoint.Item->size(); wIndex++)
			{
				wTempPoint = (*TempRectPoint.Item)[wIndex];

				if(hTempPoint.x  < wTempPoint.x )
				{
					IsInsertPoint = true;
					InsertArrayPoint(TempRectPoint.Item, wIndex, hTempPoint);
					break;
				}
			}

			if(IsInsertPoint == false)
				AddArrayPoint(TempRectPoint.Item, hTempPoint);
		}
	}

	//�����ݿ�����RectList��
	CopyArrayPoint(PointList->Item, TempRectPoint.Item);

	FreeArrayPoint(TempRectPoint.Item);
}