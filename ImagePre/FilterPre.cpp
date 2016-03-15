
#include <ImagePre/ImagePre.h>
#include <ListArrayLib/ListArrayLib.h>

DLLAPI void STDCALL MarkEdgeIplImageForRectBorder(IplImage* srcImage, CvRect rect, int ObjectGray, IplImage* EdgeImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		Position = hIndex*srcImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, Position++)
		{
			//���ܵı�Ե����---������Ե�����ж�
			if(hIndex == rect.y || hIndex == rect.y + rect.height - 1
				|| wIndex == rect.x || wIndex == rect.x + rect.width - 1)
			{
				if(srcImage->imageData[Position] == ObjectGray)
					EdgeImage->imageData[Position] = ObjectGray;
				else
				{
					if ((wIndex == rect.x && srcImage->imageData[Position + 1] == ObjectGray)
						|| (wIndex == rect.x + rect.width - 1 && srcImage->imageData[Position - 1] == ObjectGray)
						|| (hIndex == rect.y && srcImage->imageData[Position + srcImage->widthStep] == ObjectGray)
						|| (hIndex == rect.y + rect.height - 1 && srcImage->imageData[Position - srcImage->widthStep] == ObjectGray))
						EdgeImage->imageData[Position] = ObjectGray;
					else
						EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
			}
			else
			{
				if(srcImage->imageData[Position] == ObjectGray)
				{
					if (srcImage->imageData[Position + 1] == 255 - ObjectGray
						|| srcImage->imageData[Position - 1] == 255 - ObjectGray
						|| srcImage->imageData[Position + srcImage->widthStep] == 255 - ObjectGray
						|| srcImage->imageData[Position - srcImage->widthStep] == 255 - ObjectGray) 
						EdgeImage->imageData[Position] = ObjectGray;
					else
						EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
				else
				{
					EdgeImage->imageData[Position] = 255 - ObjectGray;
				}
			}//���������жϽ���			
		}
	}
}

DLLAPI void STDCALL TraceEdgeForIplImageModel(IplImage* srcImage, IplImage* MarkImage, int MarkValue, 
											  int hIndex, int wIndex, ListPoint* EdgePoint)
{	
	int dIndex = 0, Position = hIndex*srcImage->widthStep + wIndex;

	int PointX = 0, PointY = 0, NumIndex = 0;

	int DirectIndex = 0, IsFindStartPoint = 0, IsFindIndex = 3;

	static int Direction[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

	//��̬��������ӵ�һ�����ص�����
	AddArrayPoint(EdgePoint->Item, cvPoint(wIndex, hIndex));

	MarkImage->imageData[Position] = 128;   //��ʶ��ǰ���ص��Ѿ�������

	//��ʾΪĿ��ɫ����ʼ���أ���ʼ���и��ٲ���---��ǰ����������
	while(NumIndex < EdgePoint->Item->size())
	{
		NumIndex++;

		for (DirectIndex = 0; DirectIndex < 8; DirectIndex++)
		{
			PointX = (*EdgePoint->Item)[NumIndex - 1].x + Direction[IsFindIndex][0];
			PointY = (*EdgePoint->Item)[NumIndex - 1].y + Direction[IsFindIndex][1];

			if(PointX >= srcImage->width || PointX < 0 || PointY >= srcImage->height || PointY < 0)
				continue;

			Position = PointY*srcImage->widthStep + PointX;

			//��ʾ��ǰ����Ϊ��Ե��
			if(srcImage->imageData[Position] == MarkValue && MarkImage->imageData[Position] != 128)
			{
				//������������뵽��̬������
				AddArrayPoint(EdgePoint->Item, cvPoint(PointX, PointY));

				MarkImage->imageData[Position] = 128;     //��ʶ��ǰ���ص��Ѿ�������

				if(DirectIndex - 1 < 0)
					IsFindIndex = 0;
				else
					IsFindIndex = DirectIndex - 1;

				break;
			}
			else
			{
				IsFindIndex++;

				if(IsFindIndex == 8)
					IsFindIndex = 0;
			}
		}

	}//whileѭ���������������������
}

/****************************************************************
	��ȡ��Եͼ�������б�Ե�ı�Ե�������ı�Ե
*****************************************************************/
DLLAPI int STDCALL ExtractMaxEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect, 
												  int ObjectGray, ListPoint* PointList)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, Position = 0, EdgeNum = 0;

	//���ڶ�ֵͼ��λ���˺Ŷ�Ӧ��λ��
	ListPoint EdgePoint;
	EdgePoint.Item = (ArrayPoint *)MallocArrayPoint();

	//���ڵ㼯�ϼ�����С��Ӿ��ε��ĸ�����
	cvRectObj TempRect; CvPoint TempPoint;

	memset(MarkImage->imageData, 255 - ObjectGray, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

	//ȷ������Χ�ڱ�Ե��Ŀ��������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		Position = hIndex*EdgeImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, Position++)
		{
			if(EdgeImage->imageData[Position] == 255 - ObjectGray
				|| MarkImage->imageData[Position] == 128)
				continue;

			TraceEdgeForIplImageModel(EdgeImage, MarkImage, ObjectGray, hIndex, wIndex, &EdgePoint);

			if(EdgePoint.Item->size() > 30)
				EdgeNum++;

			if(PointList->Item->size() < EdgePoint.Item->size())
				CopyArrayPoint(PointList->Item, EdgePoint.Item);

			ClearArrayPoint(EdgePoint.Item);
		}
	}//���������������

	FreeArrayPoint(EdgePoint.Item);

	return EdgeNum;
}

/****************************************************************
	��ȡ��Եͼ�������б�Ե���ص�
*****************************************************************/
DLLAPI int STDCALL ExtractAllEdgePointNumForItem(IplImage* EdgeImage, IplImage* MarkImage, CvRect rect,
												 int ObjectGray, ListPoint* PointList)
{
	int hIndex = 0, wIndex = 0, dIndex = 0, Position = 0, EdgeNum = 0;

	//���ڶ�ֵͼ��λ���˺Ŷ�Ӧ��λ��
	ListPoint EdgePoint;
	EdgePoint.Item = (ArrayPoint *)MallocArrayPoint();

	//���ڵ㼯�ϼ�����С��Ӿ��ε��ĸ�����
	cvRectObj TempRect; CvPoint TempPoint;

	memset(MarkImage->imageData, 255 - ObjectGray, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

	/*cvShowImage("edgeImg", EdgeImage);
	cvWaitKey(0);*/

	//ȷ������Χ�ڱ�Ե��Ŀ��������
	for (hIndex = rect.y; hIndex < VL_MIN(MarkImage->height,(rect.y + rect.height)); hIndex++)
	{
		Position = hIndex*EdgeImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < VL_MIN(MarkImage->width,(rect.x + rect.width)); wIndex++, Position++)
		{
			if(EdgeImage->imageData[Position] == 255 - ObjectGray
				|| MarkImage->imageData[Position] == 128)
				continue;

			TraceEdgeForIplImageModel(EdgeImage, MarkImage, ObjectGray, hIndex, wIndex, &EdgePoint);

			if(EdgePoint.Item->size() > 30)
			{
				EdgeNum++;

				if(PointList->Item->size() == 0)
					CopyArrayPoint(PointList->Item, EdgePoint.Item);
				else
				{
					for(dIndex = 0; dIndex < EdgePoint.Item->size(); dIndex++)
					{
						TempPoint = (*EdgePoint.Item)[dIndex];

						AddArrayPoint(PointList->Item, TempPoint);
					}
				}
			}			

			ClearArrayPoint(EdgePoint.Item);
		}
	}//���������������

	FreeArrayPoint(EdgePoint.Item);

	return EdgeNum;
}

DLLAPI void STDCALL FilterRegionListForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
											int MinMarkNum, int MaxMarkNum, IplImage* dstImage, ListRect* rectList, ListInt* intList, ArrayPoint* PointArray)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//ʹ��C++��׼���е�ʸ��������̬����
	//ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0; 

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				Position = YValue*srcImage->widthStep + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			if(PointArray->size() > MaxMarkNum || PointArray->size() < MinMarkNum)
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}	
			else
			{
				//�����������ӵ���Ӧ�ľ��ο���б���
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
			}

			//if(PointArray->size() > 0)
				//ClearArrayPoint(PointArray);				
		}
	}//���������������

	//FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterBlobNoiseForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray,
										   int MinMarkNum, int MaxMarkNum, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));
			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				//����ʸ����̬�����е�Ԫ��,ȷ�������ĵ�ǰ���ص�λ��
				if (XValue <= rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			if (PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum) //�����Ա��˳��İߵ�
			{
				//�������Ҫɾ���İߵ㣬���ȱ�ʾΪgray
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}//�����Ա��˳��İߵ����

			//��ʸ����̬�������
			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//���������������

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
												   int MinMarkNum, int MaxMarkNum, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, TempGray = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//����ʸ����̬�����е�Ԫ��,ȷ�������ĵ�ǰ���ص�λ��
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			if(PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum)
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}
				
			if(MaxXValue - MinXValue > 0.90*rect.width || MaxYValue - MinYValue > 0.90*rect.height)
			{
				
			}
			else
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}		

			ClearArrayPoint(PointArray);
		}
	}//���������������

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterMaxHeightWidthRegionForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, 
													  int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//����ʸ����̬�����е�Ԫ��,ȷ�������ĵ�ǰ���ص�λ��
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			//�����εĸ߶Ȼ��߿������Ҫ��ֱ���˳�
			if(MaxXValue - MinXValue > 0.90*rect.width || MaxYValue - MinYValue > 0.90*rect.height)
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
				continue;
			}

			if(PointArray->size() > BestArray->size())
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				if(PointArray->size() > 0)
					ClearArrayPoint(PointArray);
			}				
		}
	}//���������������

	FreeArrayPoint(BestArray);
	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterAreaEdgeRatioForRect(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, 
											   int MinAreaNum, int MaxAreaNum, IplImage* dstImage, 
											   ListRect* rectList, ListInt* intList, ListFloat* floatList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = MaxXValue = wIndex;
			MinYValue = MaxYValue = hIndex;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue < XValue ? MinXValue : XValue;
				MaxXValue = MaxXValue > XValue ? MaxXValue : XValue;
				MinYValue = MinYValue < YValue ? MinYValue : YValue;
				MaxYValue = MaxYValue > YValue ? MaxYValue : YValue;

				//����ʸ����̬�����е�Ԫ��,ȷ�������ĵ�ǰ���ص�λ��
				if (XValue < rect.x || XValue >= rect.x + rect.width
					|| YValue < rect.y || YValue >= rect.y + rect.height)
					continue;

				Position = YValue*srcImage->widthStep + XValue;

				//�����������жϵ�ǰ�����Ƿ�Ϊ��Ե����
				if((XValue - 1 >= rect.x && srcImage->imageData[Position - 1] == 255 - Objectgray)
					|| (XValue + 1 < rect.x + rect.width && srcImage->imageData[Position + 1] == 255 - Objectgray)
					|| (YValue - 1 >= rect.y && srcImage->imageData[Position - srcImage->widthStep] == 255 - Objectgray)
					|| (YValue + 1 < rect.y + rect.height && srcImage->imageData[Position + srcImage->widthStep] == 255 - Objectgray))
					EdgeNum++;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			//�����εĸ߶Ȼ��߿������Ҫ��ֱ���˳�
			//if(PointArray->size()/EdgeNum > MinRatoValue)
			//{
			//	//����ǰĿ������Ϊ����ɫ
			//	for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
			//	{
			//		Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

			//		dstImage->imageData[Position] = 255 - Objectgray;
			//	}

			//	ClearArrayPoint(PointArray);
			//	continue;
			//}

			if(PointArray->size() > MaxAreaNum || PointArray->size() < MinAreaNum)
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}	
			else
			{
				float TempValue = (float)PointArray->size()/EdgeNum;
				//�����������ӵ���Ӧ�ľ��ο���б���
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
				AddArrayFloat(floatList->Item, TempValue);
			}
			
			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);				
		}
	}//���������������

	FreeArrayPoint(PointArray);
}


DLLAPI void STDCALL FilterMaxRegionForRectImage(IplImage* srcImage, IplImage* MarkImage, CvRect rect, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	//��������������ؽ��б���������
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0; 

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				Position = YValue*srcImage->widthStep + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue >= rect.y
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			if(PointArray->size() > BestArray->size())
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				//����ǰĿ������Ϊ����ɫ
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				if(PointArray->size() > 0)
					ClearArrayPoint(PointArray);
			}				
		}
	}//���������������

	FreeArrayPoint(BestArray);	FreeArrayPoint(PointArray);
}

DLLAPI bool STDCALL FilterMaxRegionForImage(IplImage* srcImage, IplImage* MarkImage, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, TempGray = 0;

	int iStartNum = 0, iEndNum = 0, EdgeNum = 0, IsExistOthers = 0;

	int XValue = 0, YValue = 0, dIndex = 0; 
	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	//ʹ��C++��׼���е�ʸ��������̬����
	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	//��������������ؽ��б���������
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�жϵ�ǰ�����Ƿ�Ϊ����ɫ
			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			//�жϵ�ǰ�����Ƿ񱻱�ǹ�
			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;  EdgeNum = 0;

			//��ǰ�������ݼ��뵽ʸ����̬������
			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			MinXValue = 1<<30; MinYValue = 1<<30; MaxXValue = 0; MaxYValue = 0;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				//����ʸ����̬�����е�Ԫ��,ȷ�������ĵ�ǰ���ص�λ��
				/*if (XValue < 0 || XValue >= srcImage->width
					|| YValue < 0 || YValue >= srcImage->height)
					continue;*/

				Position = YValue*srcImage->width + XValue;

				//��������������Ͻ�����
				if (XValue - 1 >= 0 && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				//�����������������
				if (XValue >= 0 && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep] != 128 
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				//��������������Ͻ�����
				if (XValue + 1 < srcImage->width && YValue - 1 >= 0
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] != 128
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				//������������ұ�����
				if (XValue + 1 < srcImage->width && YValue >= 0
					&& MarkImage->imageData[Position + 1] != 128 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				//��������������½�����
				if (XValue + 1 < srcImage->width && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] != 128 
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (XValue >= 0 && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep] != 128
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= 0 && YValue + 1 < srcImage->height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] != 128
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= 0 && YValue >= 0
					&&MarkImage->imageData[Position - 1] != 128 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			//��ʾ�������鶼����0����ʾ��������Ŀ��
			if(BestArray->size() > 20 && PointArray->size() > 20)
				IsExistOthers = 1;

			if (PointArray->size() > BestArray->size())    //������������Ҫ��
			{
				//��BestArray��Ӧλ������
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);
			}
			else
			{
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
			}
		}
	}//���������������

	FreeArrayPoint(BestArray);
	FreeArrayPoint(PointArray);

	if(IsExistOthers == 1)
		return true;
	else
		return false;
}

DLLAPI void STDCALL FilterEdgeRegionRectList(IplImage* srcImage, IplImage* MarkImage, int Objectgray, CvRect rect, int MinMarkNum,
											 int MaxMarkNum, IplImage* dstImage, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, IsEdgeRegion = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0; IsEdgeRegion = 0; 

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				if (XValue == rect.x || XValue == rect.x + rect.width - 1
					|| YValue == rect.y || YValue == rect.y + rect.height - 1)
				{
					IsEdgeRegion = 1;
				}

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			if (IsEdgeRegion == 1)
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else
			{
				if(PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum || MaxXValue - MinXValue > 2*(MaxYValue - MinYValue)) //�����Ա��˳��İߵ�
				{
					for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
					{
						Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
						dstImage->imageData[Position] = 255 - Objectgray;					
					}
				}//�����Ա��˳��İߵ����
				else
				{
					//�����������ӵ���Ӧ�ľ��ο���б���
					AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
					AddArrayInt(intList->Item, PointArray->size());
				}
			}

			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//���������������
	
	FreeArrayPoint(PointArray);
}


DLLAPI void STDCALL FilterAllRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
											int MaxMarkNum, float HeightWidthRatio, bool IsSide, IplImage* dstImage, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0;  float RatioValue = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0;

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			RatioValue = (float)VL_MAX(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1)/VL_MIN(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

			if (PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum) //�����Ա��˳��İߵ�
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}//�����Ա��˳��İߵ����
			else if(MaxXValue - MinXValue > 0.75*srcImage->width ||
				MaxYValue - MinYValue > 0.75*srcImage->height || RatioValue > HeightWidthRatio)
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinXValue <= rect.x + 1 || MaxXValue + 1 >=rect.x + rect.width))//����X��Ե�˲�
			{
				//�������������˲�
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinYValue <= rect.y + 1 || MaxYValue + 1 >=rect.y + rect.height))//����Y��Ե�˲�
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else
			{
				//�����������ӵ���Ӧ�ľ��ο���б���
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
			}

			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//���������������

	FreeArrayPoint(PointArray);
}

DLLAPI void STDCALL FilterMaxRectRegion(IplImage* srcImage, IplImage* MarkImage,int Objectgray, IplImage* dstImage, 
										CvRect* rectRegion, ListRect* rectList, ListInt* intList)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int iStartNum = 0, iEndNum = 0, IsEdgeRegion = 0;

	int XValue = 0, YValue = 0, dIndex = 0;

	int MinXValue = 0, MaxXValue = 0, MinYValue = 0, MaxYValue = 0;

	ArrayPoint* PointArray = (ArrayPoint*)MallocArrayPoint();
	ArrayPoint* BestArray = (ArrayPoint*)MallocArrayPoint();

	CvRect rect = cvRect(0, 0, srcImage->width, srcImage->height), TempRect;

	(*rectRegion) = cvRect(0, 0, 0, 0);

	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if (srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			if (MarkImage->imageData[Position] == 128)
				continue;

			iStartNum = -1; iEndNum = 0, IsEdgeRegion = 0;

			MaxXValue = 0; MaxYValue = 0; MinXValue = 1<<30; MinYValue = 1<<30;

			AddArrayPoint(PointArray, cvPoint(wIndex, hIndex));

			MarkImage->imageData[Position] = 128;

			while(iStartNum < iEndNum)
			{
				iStartNum = iStartNum + 1;

				XValue = (*PointArray)[iStartNum].x;
				YValue = (*PointArray)[iStartNum].y;

				MinXValue = MinXValue > XValue ? XValue : MinXValue;
				MaxXValue = MaxXValue < XValue ? XValue : MaxXValue;

				MinYValue = MinYValue > YValue ? YValue : MinYValue;
				MaxYValue = MaxYValue < YValue ? YValue : MaxYValue;

				Position = YValue*srcImage->widthStep + XValue;

				if (XValue - 1 >= rect.x && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep - 1] = 128;
				}

				if (YValue - 1 >= rect.y && XValue >= rect.x
					&& MarkImage->imageData[Position - srcImage->widthStep] == 0
					&& srcImage->imageData[Position - srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue - 1 >= rect.y
					&& MarkImage->imageData[Position - srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position - srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue - 1));

					MarkImage->imageData[Position - srcImage->widthStep + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue >= rect.y 
					&& MarkImage->imageData[Position + 1] == 0 
					&& srcImage->imageData[Position + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue));

					MarkImage->imageData[Position + 1] = 128;
				}

				if (XValue + 1 < rect.x + rect.width && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep + 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep + 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue + 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep + 1] = 128;
				}

				if (YValue + 1 < rect.y + rect.height && XValue >= rect.x
					&& MarkImage->imageData[Position + srcImage->widthStep] == 0
					&& srcImage->imageData[Position + srcImage->widthStep] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep] = 128;
				}

				if (XValue - 1 >= rect.x && YValue + 1 < rect.y + rect.height
					&& MarkImage->imageData[Position + srcImage->widthStep - 1] == 0
					&& srcImage->imageData[Position + srcImage->widthStep - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue + 1));

					MarkImage->imageData[Position + srcImage->widthStep - 1] = 128;
				}

				if (XValue - 1 >= rect.x && YValue > rect.x
					&& MarkImage->imageData[Position - 1] == 0 
					&& srcImage->imageData[Position - 1] == Objectgray)
				{
					AddArrayPoint(PointArray, cvPoint(XValue - 1, YValue));

					MarkImage->imageData[Position - 1] = 128;
				}

				iEndNum = PointArray->size() - 1;
			}//whileѭ������,���������ǽ���

			TempRect = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

			if(PointArray->size() > 30)
			{
				AddArrayRect(rectList->Item, TempRect);
				AddArrayInt(intList->Item, PointArray->size());
			}

			if (TempRect.height*TempRect.width > rectRegion->height*rectRegion->width)    //������������Ҫ��
			{
				//��BestArray��Ӧλ������
				for(dIndex = 0; dIndex < BestArray->size(); dIndex++)
				{
					Position = (*BestArray)[dIndex].y*srcImage->widthStep + (*BestArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				CopyArrayPoint(BestArray, PointArray);

				(*rectRegion) = cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);
			}
			else
			{
				for(dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;

					dstImage->imageData[Position] = 255 - Objectgray;
				}

				ClearArrayPoint(PointArray);
			}
		}
	}//���������������

	FreeArrayPoint(PointArray);
	FreeArrayPoint(BestArray);
}