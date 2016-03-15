#include <ImagePre/ImagePre.h>

DLLAPI void STDCALL AdaptiveStretchImage(IplImage* srcImage, CvRect rect, float lowValue,
										 float highValue, IplImage* dstImage)
{
	//计算最高和最低像素的个数
	int lowTotal = rect.height*rect.width*lowValue;
	int hightTotal = rect.height*rect.width*(1 - highValue);

	int GradGraye[256];
	memset(GradGraye, 0, 256*sizeof(int));

	int hIndex = 0, wIndex = 0, ImagePosition = 0, ColorValue = 0;

	//生成图像直方图
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		ImagePosition = hIndex*srcImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, ImagePosition++)
		{
			ColorValue = srcImage->imageData[ImagePosition];

			GradGraye[ColorValue]++;
		}
	}

	int lowPixelValue = 0, highPixelValue = 0;

	//计算图像的最大和最小灰度
	for (hIndex = 0, ColorValue = 0; hIndex < 256; hIndex++)
	{
		ColorValue += GradGraye[hIndex];

		if (ColorValue > lowTotal)
		{
			lowPixelValue = hIndex;
			break;
		}
	}

	for (hIndex = 255, ColorValue = 0; hIndex >= 0; hIndex--)
	{
		ColorValue += GradGraye[hIndex];

		if (ColorValue > hightTotal)
		{
			highPixelValue = hIndex;
			break;
		}
	}

	//生成图像直方图
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++)
	{
		ImagePosition = hIndex*srcImage->widthStep + rect.x;

		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++, ImagePosition++)
		{
			ColorValue = srcImage->imageData[ImagePosition];

			if(ColorValue > highPixelValue)
				dstImage->imageData[ImagePosition] = 255;
			else if(ColorValue < lowPixelValue)
				dstImage->imageData[ImagePosition] = 0;
			else
				dstImage->imageData[ImagePosition] = cvRound(255*(float)(ColorValue-lowPixelValue)/(highPixelValue-lowPixelValue));
		}
	}
}

DLLAPI void STDCALL cvDrawSectorM(IplImage * image, cvDrawSector * DrawSector, CvPoint centerOut, CvPoint centerIn, CvSize axesOut, CvSize axesIn, double RotationAngleOut, double RotationAngleIn, double start_angleOut, double end_angleOut, double start_angleIn, double end_angleIn)
{

	cvEllipse(image, centerOut, axesOut, RotationAngleOut, start_angleOut, end_angleOut, CV_RGB(0, 255, 0), 1, 8, 0);

	cvEllipse(image, centerIn, axesIn, RotationAngleIn, start_angleIn, end_angleIn, CV_RGB(0, 255, 0), 1, 8, 0);

	return ;

}