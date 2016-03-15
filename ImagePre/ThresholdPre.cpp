/*********************************************
图像的预处理函数库---包括图像的二值化部分
***********************************************/

#include <ImagePre/ImagePre.h>
#include <ListArrayLib/ListArrayLib.h>

//基于输入图像计算累积和
void cvIntegralImage(IplImage* srcImage, float* sumImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	sumImage[0] = srcImage->imageData[0];

	//第一行的累积
	for(wIndex = 1; wIndex < srcImage->width; wIndex++)
		sumImage[wIndex] = sumImage[wIndex - 1] + srcImage->imageData[wIndex];

	//第一列的累积
	for(hIndex = 1; hIndex < srcImage->height; hIndex++)
	{
		Position = hIndex*srcImage->widthStep;

		sumImage[Position] = sumImage[Position - srcImage->widthStep] + srcImage->imageData[Position];
	}

	//整个区域的累积
	for (hIndex = 1; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 1; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			sumImage[Position] = sumImage[Position - 1] + sumImage[Position - srcImage->widthStep];
			sumImage[Position] -= sumImage[Position - srcImage->widthStep - 1];
			sumImage[Position] += srcImage->imageData[Position];
		}
	}
}

//基于相关参数判断当前像素的二值化值
void cvLocalPixelValue(IplImage* srcImage, int DiffValue, int Position, float meanValue, IplImage* dstImage)
{
	int threshold = 0;

	threshold = cvRound(meanValue + DiffValue);

	if(srcImage->imageData[Position] > threshold)
		dstImage->imageData[Position] = 255;
	else
		dstImage->imageData[Position] = 0;
}

/****************************************************
自适应二值化算法，基于图像的均值和差值方法
*****************************************************/
DLLAPI void STDCALL cvAdaptiveDevision(IplImage* srcImage, int hModel, int wModel, int DiffValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	float* sumImage = (float*)malloc(srcImage->height*srcImage->widthStep*sizeof(float));
	memset(sumImage, 0, srcImage->height*srcImage->widthStep*sizeof(float));

	//计算累积和
	cvIntegralImage(srcImage, sumImage);

	Position = srcImage->height*srcImage->widthStep - 1;

	float meanValue = 0, sumValue = 0;

	//对整体图像进行自适应二值化操作
	//高度在模板范围内
	for (hIndex = 0; hIndex < hModel; hIndex++)
	{
		//宽度在左模板范围内
		for (wIndex = 0; wIndex < wModel; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + wIndex + wModel;

			meanValue = sumImage[Position]/((hIndex + hModel + 1)*(wIndex + wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}  //左模板范围内计算结束

		for (wIndex = wModel; wIndex < srcImage->width - wModel; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue = sumImage[Position];

			Position = (hIndex + hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((hIndex + hModel + 1)*(2*wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//中间区域处理结束

		//右模板宽度范围内
		for(wIndex = srcImage->width - wModel; wIndex < srcImage->width; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + srcImage->width - 1;

			sumValue = sumImage[Position];

			Position = (hIndex + hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((hIndex + hModel + 1)*(srcImage->width + wModel - wIndex));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//右模板范围结束
	}//上模板范围结束

	//在高度模板范围内进行计算
	for (hIndex = hModel; hIndex < srcImage->height - hModel; hIndex++)
	{
		//左模板范围的计算
		for (wIndex = 0; wIndex < wModel; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = (hIndex + hModel)*srcImage->widthStep;
			sumValue -= sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((2*hModel + 1)*(wIndex + wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//左模板范围计算结束

		for (wIndex = wModel; wIndex < srcImage->width - wModel; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue -= sumImage[Position];

			Position = (hIndex + hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((2*hModel + 1)*(2*wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//整体模板范围内计算结束

		for(wIndex = srcImage->width - wModel; wIndex < srcImage->width; wIndex++)
		{
			Position = (hIndex + hModel)*srcImage->widthStep + srcImage->width - 1;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + srcImage->width - 1;

			sumValue -= sumImage[Position];

			Position = (hIndex + hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((2*hModel + 1)*(srcImage->width + wModel - wIndex));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//右模板范围计算结束
	}//中间部分遍历结束

	int dValue = (srcImage->height - 1)*srcImage->widthStep;

	//下模板范围遍历范围
	for (hIndex = srcImage->height - hModel; hIndex < srcImage->height; hIndex++)
	{
		//左模板范围遍历
		for (wIndex = 0; wIndex < wModel; wIndex++)
		{
			Position = dValue + wIndex + wModel;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue -= sumImage[Position];

			Position = dValue;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((srcImage->height + hModel - hIndex)*(wIndex + wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}

		//宽度模板范围内计算
		for (wIndex = wModel; wIndex < srcImage->width - wModel; wIndex++)
		{
			Position = dValue + wIndex + wModel;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = dValue + wIndex - wModel;

			sumValue -= sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex + wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((srcImage->height + hModel - hIndex)*(2*wModel + 1));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}

		//右模板范围内计算
		for (wIndex = srcImage->width - wModel; wIndex < srcImage->width; wIndex++)
		{
			Position = dValue + srcImage->width - 1;

			sumValue = sumImage[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + wIndex - wModel;

			sumValue += sumImage[Position];
			sumValue += srcImage->imageData[Position];

			Position = (hIndex - hModel)*srcImage->widthStep + srcImage->width - 1;

			sumValue -= sumImage[Position];

			Position = dValue + wIndex - wModel;

			sumValue -= sumImage[Position];

			meanValue = sumValue/((srcImage->height + hModel - hIndex)*(srcImage->width + wModel - wIndex));

			Position = hIndex*srcImage->widthStep + wIndex;

			cvLocalPixelValue(srcImage, DiffValue, Position, meanValue, dstImage);
		}//右模板范围计算结束
	}//下模板范围计算结束

	cvFixThreshold(dstImage, 128, dstImage);

	if(sumImage != NULL)
	{
		free(sumImage);
		sumImage = NULL;
	}
}

DLLAPI void STDCALL cvAdaptiveDevisionImageByProb(IplImage* srcImage, float MinValue, float MaxValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int dValue = 0, histImage[256];

	float Prob[256], sumProb[256];

	memset(histImage, 0, 256*sizeof(int));
	memset(Prob, 0, 256*sizeof(float));
	memset(sumProb, 0, 256*sizeof(float));

	//基于输入图像的概率部分确定阈值
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			histImage[dValue]++;
		}
	}

	//累积直方图概率和
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		Prob[hIndex] = (float)histImage[hIndex]/(srcImage->height*srcImage->width);

		if(hIndex == 0)
			sumProb[0] = Prob[0];
		else
			sumProb[hIndex] = sumProb[hIndex - 1] + Prob[hIndex];
	}

	int Imin = 0, Imax = 0;

	//确定滤波的截止灰度等级
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		if(sumProb[hIndex] <= MinValue)
			continue;

		Imin = hIndex;
		break;
	}

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(sumProb[hIndex] >= MaxValue)
			continue;

		Imax = hIndex;
		break;
	}

	for(hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			if(dValue >= Imin && dValue <= Imax)
				dstImage->imageData[Position] = 255;
			else
				dstImage->imageData[Position] = 0;
		}
	}
}

DLLAPI void STDCALL OstuThreshold(IplImage* srcImage, IplImage* dstImage)
{
	int threshold = 1; // 阈值
	int ihist[256]; // 图像直方图，256个点

	// 对直方图置零...
	memset(ihist, 0, sizeof(ihist));

	int Gmin = 255, Gmax = 0, Pixel = 0;

	int hIndex = 0, wIndex = 0, Position = 0;

	//生成直方图
	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			Pixel = srcImage->imageData[Position];

			ihist[Pixel]++;
			if(Pixel > Gmax) Gmax = Pixel;
			if(Pixel < Gmin) Gmin = Pixel;
		}
	}

	// set up everything
	int IntegralSum = 0, IntegralSum1 = 0.0, SumNum = 0, SumNum1 = 0, SumNum2 = 0;

	float ProbValue = 0, ProbValue1 = 0, ProbValue2 = 0, MaxProbValue = 0;

	for (hIndex = 0; hIndex <= 255; hIndex++) 
	{
		IntegralSum += hIndex * ihist[hIndex]; /* x*f(x) 质量矩*/
		SumNum += ihist[hIndex];       /* f(x) 质量 */
	}

	if (!SumNum)    //表示数据存储有问题
		threshold = 160;

	// do the otsu global thresholding method
	MaxProbValue = -1.0;
	for (hIndex = 0; hIndex < 255 && SumNum > 0; hIndex++) 
	{
		SumNum1 += ihist[hIndex];
		if (!SumNum1)
			continue;

		SumNum2 = SumNum - SumNum1;

		if (SumNum2 == 0) 
			break;

		IntegralSum1 += hIndex *ihist[hIndex];

		ProbValue1 = (float)IntegralSum1/SumNum1;
		ProbValue2 = (float)(IntegralSum - IntegralSum1)/SumNum2;

		ProbValue = SumNum1 * SumNum2 *(ProbValue1 - ProbValue2) * (ProbValue1 - ProbValue2);
		
		if (ProbValue > MaxProbValue)
		{
			MaxProbValue = ProbValue;
			threshold = hIndex;
		}
	}

	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if(srcImage->imageData[Position] >= threshold)
				dstImage->imageData[Position] = 255;
			else 
				dstImage->imageData[Position] = 0;
		}
	}
}

DLLAPI void STDCALL cvFixThreshold(IplImage* srcImage, int threshold, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	//基于指定的固定阈值进行二值化操作
	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if(srcImage->imageData[Position] >= threshold)
				dstImage->imageData[Position] = 0;
			else 
				dstImage->imageData[Position] = 255;
		}
	}
}

DLLAPI void STDCALL cvFixThreshold_Three(IplImage* currImage, IplImage* currThreeImage,
										 int currThreshold, int currThreeThreshold, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	//基于指定的固定阈值进行二值化操作
	for (hIndex = 0; hIndex < currImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < currImage->width; wIndex++) 
		{
			Position = hIndex*currImage->widthStep + wIndex;

			//两者都满足，则赋值为255
			if(currThreeImage->imageData[Position] < currThreeThreshold)
				dstImage->imageData[Position] = 0;
			else 
				dstImage->imageData[Position] = 255;

			if(dstImage->imageData[Position] == 0)
			{
				if(currImage->imageData[Position] > currThreshold)
					dstImage->imageData[Position] = 255;
			}
		}
	}
}

DLLAPI bool STDCALL cvFixBottomThreshold_Three(IplImage* currImage, IplImage* currThreeImage,
											   int currThreshold, int currThreeThreshold, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	//基于指定的固定阈值进行二值化操作
	for (hIndex = 0; hIndex < currImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < currImage->width; wIndex++) 
		{
			Position = hIndex*currImage->widthStep + wIndex;

			//两者都满足，则赋值为255
			if(currThreeImage->imageData[Position] < currThreeThreshold)
				dstImage->imageData[Position] = 0;
		}
	}

	return true;
}

DLLAPI void STDCALL cvFixThreshold_Inverse(IplImage* srcImage, int threshold, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	//基于指定的固定阈值进行二值化操作
	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if(srcImage->imageData[Position] >= threshold)
				dstImage->imageData[Position] = 0;
			else 
				dstImage->imageData[Position] = 255;
		}
	}
}

DLLAPI void STDCALL PathContrastForRectImage(IplImage* srcImage, CvRect rect, int PathValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int ColorValue = 0, threshold = 0, IminValue = 0, ImaxValue = 0, PixelNum = 0;

	int GradGraye[256];
	memset(GradGraye, 0, 256*sizeof(int));

	//统计检测区域的灰度直方图
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++) 
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++) 
		{
			Position = hIndex*dstImage->widthStep + wIndex;

			ColorValue = srcImage->imageData[Position];

			GradGraye[ColorValue]++;
		}
	}

	PixelNum = VL_MAX(0.001*rect.height*rect.width, 6);

	//统计区域内灰度的最大值和最小值
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		if(GradGraye[hIndex] < PixelNum)
			continue;

		IminValue = hIndex;
		break;
	}

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(GradGraye[hIndex] < PixelNum)
			continue;

		ImaxValue = hIndex;
		break;
	}

	threshold = IminValue + (ImaxValue - IminValue)*PathValue/100;

	//基于指定的路径阈值进行二值化操作
	for (hIndex = rect.y; hIndex < rect.y + rect.height; hIndex++) 
	{
		for (wIndex = rect.x; wIndex < rect.x + rect.width; wIndex++) 
		{
			Position = hIndex*dstImage->widthStep + wIndex;

			if(srcImage->imageData[Position] >= threshold)
				dstImage->imageData[Position] = 255;
			else
				dstImage->imageData[Position] = 0;
		}
	}
}

DLLAPI void STDCALL PathContrastForMarkImage(IplImage* srcImage, IplImage* markImage, int ObjectGray, int PathValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int ColorValue = 0, threshold = 0, IminValue = 0, ImaxValue = 0, PixelNum = 6;

	int GradGraye[256];
	memset(GradGraye, 0, 256*sizeof(int));

	//统计检测区域的灰度直方图
	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			if(markImage->imageData[Position] == ObjectGray)
				continue;

			ColorValue = srcImage->imageData[Position];

			GradGraye[ColorValue]++;
		}
	}

	//统计区域内灰度的最大值和最小值
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		if(GradGraye[hIndex] < PixelNum)
			continue;

		IminValue = hIndex;
		break;
	}

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(GradGraye[hIndex] < PixelNum)
			continue;

		ImaxValue = hIndex;
		break;
	}

	threshold = IminValue + (ImaxValue - IminValue)*PathValue/100;

	//基于指定的路径阈值进行二值化操作
	for (hIndex = 0; hIndex < srcImage->height; hIndex++) 
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++) 
		{
			Position = hIndex*dstImage->widthStep + wIndex;

			if(markImage->imageData[Position] == ObjectGray)
			{
				dstImage->imageData[Position] = 255 - ObjectGray;
				continue;
			}

			if(srcImage->imageData[Position] >= threshold)
				dstImage->imageData[Position] = 255 - ObjectGray;
			else
				dstImage->imageData[Position] = ObjectGray;
		}
	}
}

DLLAPI bool STDCALL ProbDevisionImageByLow(IplImage* srcImage, float MinValue, float MaxValue, int deltaThreshold, int deltaValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int dValue = 0, histImage[256];

	float Prob[256], sumProb[256];

	memset(histImage, 0, 256*sizeof(int));
	memset(Prob, 0, 256*sizeof(float));
	memset(sumProb, 0, 256*sizeof(float));

	//基于输入图像的概率部分确定阈值
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			histImage[dValue]++;
		}
	}

	//累积直方图概率和
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		Prob[hIndex] = (float)histImage[hIndex]/(srcImage->height*srcImage->width);

		if(hIndex == 0)
			sumProb[0] = Prob[0];
		else
			sumProb[hIndex] = sumProb[hIndex - 1] + Prob[hIndex];
	}

	int Imin = 0, Imax = 0;

	//确定滤波的截止灰度等级
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		if(sumProb[hIndex] <= MinValue)
			continue;

		Imin = hIndex;
		break;
	}

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(sumProb[hIndex]>= MaxValue)
			continue;

		Imax = hIndex;
		break;
	}

	if(Imin > deltaThreshold || Imax - Imin < 20)
		return false;

	int threshold = Imin + deltaValue;

	for(hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			if(dValue >= threshold)
				dstImage->imageData[Position] = 255;
			else
				dstImage->imageData[Position] = 0;
		}
	}

	return true;
}

DLLAPI void STDCALL ProbDevisionImageByHigh(IplImage* srcImage, float MaxValue, int deltaValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int dValue = 0, histImage[256];

	float Prob[256], sumProb[256];

	memset(histImage, 0, 256*sizeof(int));
	memset(Prob, 0, 256*sizeof(float));
	memset(sumProb, 0, 256*sizeof(float));

	//基于输入图像的概率部分确定阈值
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			histImage[dValue]++;
		}
	}

	//累积直方图概率和
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		Prob[hIndex] = (float)histImage[hIndex]/(srcImage->height*srcImage->width);

		if(hIndex == 0)
			sumProb[0] = Prob[0];
		else
			sumProb[hIndex] = sumProb[hIndex - 1] + Prob[hIndex];
	}

	int Imin = 0, Imax = 0;

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(sumProb[hIndex] >= MaxValue)
			continue;

		Imax = hIndex;
		break;
	}

	int threshold = Imax - deltaValue;

	for(hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			if(dValue >= threshold)
				dstImage->imageData[Position] = 255;
			else
				dstImage->imageData[Position] = 0;
		}
	}
}

DLLAPI bool STDCALL FixThresholdForBottomImage(IplImage* srcImage, float MaxValue, int channelID,int deltaThreshold, int deltaValue, IplImage* dstImage)
{
	int hIndex = 0, wIndex = 0, Position = 0;

	int dValue = 0, histImage[256];

	float Prob[256], sumProb[256];

	memset(histImage, 0, 256*sizeof(int));
	memset(Prob, 0, 256*sizeof(float));
	memset(sumProb, 0, 256*sizeof(float));

	//基于输入图像的概率部分确定阈值
	for (hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			histImage[dValue]++;
		}
	}

	//统计灰度直方图小于
	int sumPixelNum = histImage[255] + histImage[254] + histImage[253] + histImage[252] + histImage[251];
	
	sumPixelNum += histImage[250] + histImage[249] + histImage[248] + histImage[247] + histImage[246];

	//累积直方图概率和
	for (hIndex = 0; hIndex < 256; hIndex++)
	{
		Prob[hIndex] = (float)histImage[hIndex]/(srcImage->height*srcImage->width);

		if(hIndex == 0)
			sumProb[0] = Prob[0];
		else
			sumProb[hIndex] = sumProb[hIndex - 1] + Prob[hIndex];
	}

	int Imin = 0, Imax = 0;

	for (hIndex = 255; hIndex >= 0; hIndex--)
	{
		if(sumProb[hIndex] >= 1 - MaxValue)
			continue;

		Imax = hIndex;
		break;
	}

	if(Imax > deltaThreshold && sumPixelNum < 400)
		return false;           //若阈值大于指定值，则返回true
	
	int threshold = Imax + deltaValue;

	if(channelID == 12 || channelID == 13)
	{
	}
	else
	{
		if(sumPixelNum > 400)
			threshold = 240;
	}	

	for(hIndex = 0; hIndex < srcImage->height; hIndex++)
	{
		for (wIndex = 0; wIndex < srcImage->width; wIndex++)
		{
			Position = hIndex*srcImage->widthStep + wIndex;

			dValue = srcImage->imageData[Position];

			if(channelID == 12 || channelID == 13)
			{
				if(dValue >= threshold && dValue < 200)
					dstImage->imageData[Position] = 255;
				else
					dstImage->imageData[Position] = 0;
			}
			else
			{
				if(dValue >= threshold && dValue < 245)
					dstImage->imageData[Position] = 255;
				else
					dstImage->imageData[Position] = 0;
			}			
		}
	}

	return true;
}

