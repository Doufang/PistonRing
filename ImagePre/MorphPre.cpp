#include <ImagePre/ImagePre.h>

/************************************************
针对二值图像，指定的目标色进行膨胀操作
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

			//整个区域进行水平方向膨胀操作
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
				}//当前像素水平方向膨胀结束
			}//整个模板范围内的膨胀

		}
	}//整个区域的水平方向膨胀结束
}

/*************************************************
针对二值图像，指定的目标色进行腐蚀操作
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

			//整个区域进行水平方向腐蚀操作
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
			}//当前像素水平方向膨胀结束
		}
	}//整个区域的水平方向腐蚀结束
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

			//整个区域进行水平方向膨胀操作
			if(srcImage->imageData[Position] == 255 - Objectgray)
				continue;

			for (dIndex = wIndex - wModel; dIndex <= wIndex + wModel; dIndex++)
			{
				if(dIndex < 0 || dIndex >= srcImage->width)
					continue;

				Position = hIndex*srcImage->widthStep + dIndex;

				TempImage->imageData[Position] = Objectgray;
			}//当前像素水平方向膨胀结束
		}
	}//整个区域的水平方向膨胀结束

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
			}//当前像素点的垂直方向膨胀结束
		}
	}//整个区域的垂直方向膨胀结束

	cvReleaseImage(&TempImage);
}