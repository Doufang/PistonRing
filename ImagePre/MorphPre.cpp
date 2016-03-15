#include <ImagePre/ImagePre.h>

/************************************************
��Զ�ֵͼ��ָ����Ŀ��ɫ�������Ͳ���
*************************************************/
DLLAPI void STDCALL cvDilationImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, mIndex = 0, nIndex = 0;

	memcpy(dstImage->imageData, srcImage->imageData, srcImage->height*srcImage->widthStep*sizeof(unsigned char));

	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�����������ˮƽ�������Ͳ���
			if(srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			for (mIndex = hIndex - hModel; mIndex <= hIndex + hModel; mIndex++)
			{
				for (nIndex = wIndex - wModel; nIndex <= wIndex + wModel; nIndex++)
				{
					if(mIndex < 0 || mIndex >= srcImage->height || nIndex < 0 || nIndex >= srcImage->width)
						continue;

					Position = mIndex*srcImage->widthStep + nIndex;

					dstImage->imageData[Position] = Objectgray;
				}//��ǰ����ˮƽ�������ͽ���
			}//����ģ�巶Χ�ڵ�����

		}
	}//���������ˮƽ�������ͽ���
}

/*************************************************
��Զ�ֵͼ��ָ����Ŀ��ɫ���и�ʴ����
**************************************************/
DLLAPI void STDCALL cvErosionImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, mIndex = 0, nIndex = 0;

	memcpy(dstImage->imageData, srcImage->imageData, srcImage->height*srcImage->widthStep*sizeof(unsigned char));

	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�����������ˮƽ����ʴ����
			if(srcImage->imageData[Position] == Objectgray)
				continue;

			for (mIndex = hIndex - hModel; mIndex <= hIndex + hModel; mIndex++)
			{
				for (nIndex = wIndex - wModel; nIndex <= wIndex + wModel; nIndex++)
				{
					if(mIndex < 0 || mIndex >= srcImage->height || nIndex < 0 || nIndex >= srcImage->width)
						continue;

					Position = mIndex*srcImage->widthStep + nIndex;

					dstImage->imageData[Position] = 255 - Objectgray;
				}
			}//��ǰ����ˮƽ�������ͽ���
		}
	}//���������ˮƽ����ʴ����
}

DLLAPI void STDCALL MorphDilationImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0, dIndex = 0;

	IplImage* TempImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);

	memcpy(TempImage->imageData, srcImage->imageData, srcImage->height*srcImage->widthStep*sizeof(unsigned char));

	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			//�����������ˮƽ�������Ͳ���
			if(srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			for (dIndex = wIndex - wModel; dIndex <= wIndex + wModel; dIndex++)
			{
				if(dIndex < 0 || dIndex >= srcImage->width)
					continue;

				Position = hIndex*srcImage->widthStep + dIndex;

				TempImage->imageData[Position] = Objectgray;
			}//��ǰ����ˮƽ�������ͽ���
		}
	}//���������ˮƽ�������ͽ���

	memcpy(dstImage->imageData, TempImage->imageData, srcImage->height*srcImage->widthStep*sizeof(unsigned char));

	for (wIndex = 0; wIndex < srcImage->width; wIndex++)
	{
		for (hIndex = 0; hIndex < srcImage->height; hIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if(TempImage->imageData[Position] == 255 - Objectgray)
				continue;

			for (dIndex = hIndex - hModel; dIndex <= hIndex + hModel; dIndex++)
			{
				if(dIndex < 0 || dIndex >= srcImage->height)
					continue;

				Position = dIndex*srcImage->widthStep + wIndex;

				dstImage->imageData[Position] = Objectgray;
			}//��ǰ���ص�Ĵ�ֱ�������ͽ���
		}
	}//��������Ĵ�ֱ�������ͽ���

	cvReleaseImage(&TempImage);
}