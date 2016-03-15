/*********************************************
���ڴ���ı�Եͼ�񡢻Ҷ�ͼ����ȡ�߶ζ���
***********************************************/
#include <ListArrayLib/ListArrayLib.h>
#include <ObjectLib/ObjectLib.h>
#include <GeometryLib/GeometryLib.h>

#define LINEDIS      2.4
#define MINCOMBNUM   10
#define MINLINENUM   40

/********************************************
ֱ�ӷ��࣬�߶η�Χ�ںͷ�Χ��,�����BestPoint��TempPoint
��Ӧ��Ϊ��
*********************************************/
void EstimateLine(ListPoint PointSet, cvLineObj Line, ArrayPoint* BestPoint, ArrayPoint* TempPoint)
{
	float dis = 0;

	CvPoint dTempPoint;

	for (int hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		dTempPoint = (*PointSet.Item)[hIndex];

		dis = abs(Line.sinAValue*dTempPoint.x - Line.cosBValue*dTempPoint.y + Line.InterceptValue);

		if(dis > LINEDIS)
			AddArrayPoint(TempPoint, dTempPoint);
		else
			AddArrayPoint(BestPoint, dTempPoint);
	}
}

/************************************************
���ڴ���ĵ㼯�Ͻ���ֱ���������õ�����߶ε��������
*************************************************/
DLLAPI void STDCALL RansacLinePoint(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	cvLineObj TempLine;

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = 0; hIndex < PointSet.Item->size()*0.30; hIndex++)
	{
		wIndex = PointSet.Item->size() - hIndex - 1;

		if(abs(hIndex - wIndex) < 16)
			continue;

		if(hIndex < 0 || hIndex >= PointSet.Item->size()
			|| wIndex < 0 || wIndex >= PointSet.Item->size())
			continue;

		//�Ե�����ŵĵ㼯����߶Σ�����ֱ�ߵı���
		TempLine = DotDotLine((*PointSet.Item)[hIndex], (*PointSet.Item)[wIndex]);

		EstimateLine(PointSet, TempLine, dBestPoint, dTempPoint);

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

DLLAPI void STDCALL RansacLinePointForEdge(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	cvLineObj TempLine;

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = 0; hIndex < VL_MIN(PointSet.Item->size()*0.1, 20); hIndex++)
	{
		wIndex = PointSet.Item->size() - hIndex - 1;

		if(abs(hIndex - wIndex) < 16)
			continue;

		if(hIndex < 0 || hIndex >= PointSet.Item->size()
			|| wIndex < 0 || wIndex >= PointSet.Item->size())
			continue;

		//�Ե�����ŵĵ㼯����߶Σ�����ֱ�ߵı���
		TempLine = DotDotLine((*PointSet.Item)[hIndex], (*PointSet.Item)[wIndex]);

		EstimateLine(PointSet, TempLine, dBestPoint, dTempPoint);

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

DLLAPI void STDCALL RansacLinePointForLess(ListPoint PointSet, ListPoint* BestPoint, ListPoint* TempPoint)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	cvLineObj TempLine;

	ArrayPoint* dBestPoint = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* dTempPoint = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = 0; hIndex < PointSet.Item->size()/2 - 4; hIndex++)
	{
		wIndex = PointSet.Item->size() - hIndex - 1;

		if(hIndex < 0 || wIndex < 0 || abs(hIndex - wIndex) < 4)
			break;

		//�Ե�����ŵĵ㼯����߶Σ�����ֱ�ߵı���
		TempLine = DotDotLine((*PointSet.Item)[hIndex], (*PointSet.Item)[wIndex]);

		EstimateLine(PointSet, TempLine, dBestPoint, dTempPoint);

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
	}//�����㼯����

	for (hIndex = 0; hIndex < PointSet.Item->size()/4 - 4; hIndex++)
	{
		wIndex = PointSet.Item->size()/2 - hIndex - 1;	

		if(hIndex < 0 || wIndex < 0 || abs(hIndex - wIndex) < 4)
			break;

		//�Ե�����ŵĵ㼯����߶Σ�����ֱ�ߵı���
		TempLine = DotDotLine((*PointSet.Item)[hIndex], (*PointSet.Item)[wIndex]);

		EstimateLine(PointSet, TempLine, dBestPoint, dTempPoint);

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
	}//�����㼯����

	for (hIndex = PointSet.Item->size()/2; hIndex < PointSet.Item->size()*0.75 - 4; hIndex++)
	{
		wIndex = 1.5*PointSet.Item->size() - hIndex - 1;

		if(hIndex < 0 || wIndex < 0 || abs(hIndex - wIndex) < 4)
			break;

		//�Ե�����ŵĵ㼯����߶Σ�����ֱ�ߵı���
		TempLine = DotDotLine((*PointSet.Item)[hIndex], (*PointSet.Item)[wIndex]);

		EstimateLine(PointSet, TempLine, dBestPoint, dTempPoint);

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
	}//�����㼯����

	FreeArrayPoint(dBestPoint);
	FreeArrayPoint(dTempPoint);
}

DLLAPI void STDCALL FitLineObj(ListPoint PointSet, cvLineObj* outLine)
{
	int hIndex = 0, wIndex = 0, dIndex = 0;

	float sumX1 = 0, sumY1 = 0, sumX2 = 0, sumY2 = 0, sumX1Y1 = 0;

	float a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;

	CvPoint Point, Point1, Point2;

	int MinXValue = 1<<30, MaxXValue = 0, MinYValue = 1<<30, MaxYValue = 0;

	//����PointNum��Ŀ��������ֱ�����
	for(hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		Point = (*PointSet.Item)[hIndex];
		sumX1 += Point.x; sumY1 += Point.y;
		sumX2 += Point.x*Point.x; sumY2 += Point.y*Point.y;
		sumX1Y1 += Point.x*Point.y;

		MinXValue = MinXValue > Point.x ? Point.x : MinXValue;
		MinYValue = MinYValue > Point.y ? Point.y : MinYValue;

		MaxXValue = MaxXValue < Point.x ? Point.x : MaxXValue;
		MaxYValue = MaxYValue < Point.y ? Point.y : MaxYValue;
	}

	//��ϳ�������һ����ֱ��
	if(sumX1*sumX1 - sumX2*PointSet.Item->size() == 0)
	{
		b1 = 0;	
		a1 = 1;	
		c1 = -sumX1/PointSet.Item->size(); //��ʾֱ��x+c=0����ֱ��
	}
	else
	{
		a1 = (sumY1*sumX1-sumX1Y1*PointSet.Item->size())/(sumX1*sumX1-sumX2*PointSet.Item->size());//���ֱ��б��
		c1 = (sumY1-sumX1*a1)/PointSet.Item->size(); //���ֱ��ƫ����
		b1=-1;
	}

	//��ϳ�������һ��ˮƽ��
	if(sumY1*sumY1 - sumY2*PointSet.Item->size() == 0)
	{
		a2=a1;
		b2=b1;
		c2=c1;
	}
	else
	{
		b2=(sumY1*sumX1 - sumX1Y1*PointSet.Item->size())/(sumY1*sumY1 - sumY2*PointSet.Item->size());//���ֱ��б��
		c2=(sumX1 - sumY1*b2)/PointSet.Item->size();                                           //���ֱ��ƫ����
		a2=-1;
	}

	float min1 = 0, min2 = 0;

	//ͳ�Ƶ㼯���߶εľ������
	for (hIndex = 0; hIndex < PointSet.Item->size(); hIndex++)
	{
		Point = (*PointSet.Item)[hIndex];
		min1 += (a1*Point.x + b1*Point.y + c1)*(a1*Point.x + b1*Point.y + c1);
		min2 += (a2*Point.x + b2*Point.y + c2)*(a2*Point.x + b2*Point.y + c2);
	}

	min1 = min1/(a1*a1 + b1*b1);
	min2 = min2/(a2*a2 + b2*b2);

	//�����������ǵ�һ��ϵ����׼ȷ
	if(min1 < min2)
	{
		if(abs(a1) > abs(b1))   //�߶���б�Ǵ���45�ȣ�ƫ���ڴ�ֱ����
		{
			Point1.x = (int)(-c1/a1 - b1*MinYValue/a1 + 0.5);
			Point1.y = MinYValue;

			Point2.x = (int)(-c1/a1 - b1*MaxYValue/a1 + 0.5);
			Point2.y = MaxYValue;

			(*outLine) = DotDotLine(Point1, Point2);
		}
		else
		{
			Point1.x = MinXValue;
			Point1.y = (int)(-c1/b1 - a1*MinXValue/b1 + 0.5);

			Point2.x = MaxXValue;
			Point2.y = (int)(-c1/b1 - a1*MaxXValue/b1 + 0.5);

			(*outLine) = DotDotLine(Point1, Point2);
		}
	}
	else
	{
		if (abs(a2) > abs(b2))
		{
			Point1.x = (int)(-c2/a2 - b2*MinYValue/a2 + 0.5);
			Point1.y = MinYValue;

			Point2.x = (int)(-c2/a2 - b2*MaxYValue/a2 + 0.5);
			Point2.y = MaxYValue;

			(*outLine) = DotDotLine(Point1, Point2);
		} 
		else
		{
			Point1.x = MinXValue;
			Point1.y = (int)(-c2/b2 - a2*MinXValue/b2 + 0.5);

			Point2.x = MaxXValue;
			Point2.y = (int)(-c2/b2 - a2*MaxXValue/b2 + 0.5);

			(*outLine) = DotDotLine(Point1, Point2);
		}
	}//ֱ����ϲ��֣���������ϵ���жϵõ�����׼ȷ���߶β���

	outLine->LineDegree = PointSet.Item->size()/outLine->LineLength;
}