#include <GeometryLib/GeometryLib.h>

//指定的两个点对象构建线段对象
DLLAPI cvLineObj STDCALL DotDotLine(CvPoint Point1, CvPoint Point2)
{
	cvLineObj outLine;

	outLine.Point1 = Point1;
	outLine.Point2 = Point2;

	outLine.LineLength = sqrtf((float)((Point2.x - Point1.x)*(Point2.x - Point1.x) 
		+ (Point2.y - Point1.y)*(Point2.y - Point1.y)));

	outLine.cosBValue = (float)(Point2.x - Point1.x)/outLine.LineLength;
	outLine.sinAValue = (float)(Point2.y - Point1.y)/outLine.LineLength;

	outLine.InterceptValue = Point1.y*outLine.cosBValue - Point1.x*outLine.sinAValue;

	return outLine;
}

DLLAPI float STDCALL DotDotDis(CvPoint Point1, CvPoint Point2)
{
	float LineLength = 0;

	LineLength = sqrtf((float)((Point2.x - Point1.x)*(Point2.x - Point1.x) 
		+ (Point2.y - Point1.y)*(Point2.y - Point1.y)));

	return LineLength;
}

float LineAngle90To90(float sinAValue, float cosBValue)
{
	float Angle = 0;

	//计算-90到90度范围内的线段角度
	if(sinAValue == 0 || cosBValue == 0)
	{
		if(cosBValue == 0 && sinAValue > 0)
			Angle = VL_PI/2;
		else if(cosBValue == 0 && sinAValue < 0)
			Angle = -VL_PI/2;
		else
			Angle = 0;
	}
	else if(cosBValue > 0 && sinAValue > 0)   //第一象限
	{
		if(cosBValue > 0.707)
			Angle = asinf(sinAValue);
		else
			Angle = acosf(cosBValue);
	}
	else if(cosBValue < 0 && sinAValue > 0)   //第二象限
	{
		if(cosBValue < -0.707)
			Angle = -asinf(sinAValue);
		else
			Angle = acosf(cosBValue) - VL_PI;
	}
	else if(cosBValue < 0 && sinAValue < 0)    //第三象限
	{
		if(cosBValue < -0.707)
			Angle = -asinf(sinAValue);
		else
			Angle = VL_PI - acosf(cosBValue);
	}
	else if(cosBValue > 0 && sinAValue < 0)     //第四象限
	{
		if(cosBValue > 0.707)
			Angle = asinf(sinAValue);
		else
			Angle = -acosf(cosBValue);
	}

	Angle = Angle*180/VL_PI;

	return Angle;
}

float LineAngle0To180(float sinAValue, float cosBValue)
{
	float Angle = 0;

	//计算-90到90度范围内的线段角度
	if(sinAValue == 0 || cosBValue == 0)
	{
		if(cosBValue == 0 && sinAValue != 0)
			Angle = VL_PI/2;
		else
			Angle = 0;
	}
	else if(cosBValue > 0 && sinAValue > 0)   //第一象限
	{
		if(cosBValue > 0.707)
			Angle = asinf(sinAValue);
		else
			Angle = acosf(cosBValue);
	}
	else if(cosBValue < 0 && sinAValue > 0)   //第二象限
	{
		if(cosBValue < -0.707)
			Angle = VL_PI - asinf(sinAValue);
		else
			Angle = acosf(cosBValue);
	}
	else if(cosBValue < 0 && sinAValue < 0)    //第三象限
	{
		if(cosBValue < -0.707)
			Angle = -asinf(sinAValue);
		else
			Angle = VL_PI - acosf(cosBValue);
	}
	else if(cosBValue > 0 && sinAValue < 0)     //第四象限
	{
		if(cosBValue > 0.707)
			Angle = VL_PI + asinf(sinAValue);
		else
			Angle = VL_PI - acosf(cosBValue);
	}

	Angle = Angle*180/VL_PI;

	return Angle;
}

float LineAngle0To360(float sinAValue, float cosBValue)
{
	float Angle = 0;

	//计算-90到90度范围内的线段角度
	if(sinAValue == 0 || cosBValue == 0)
	{
		if(cosBValue == 0 && sinAValue > 0)
			Angle = VL_PI/2;
		else if(cosBValue == 0 && sinAValue < 0)
			Angle = 3*VL_PI/2;
		else if(sinAValue == 0 && cosBValue > 0)
			Angle = 0;
		else
			Angle = VL_PI;
	}
	else if(cosBValue > 0 && sinAValue > 0)   //第一象限
	{
		if(cosBValue > 0.707)
			Angle = asinf(sinAValue);
		else
			Angle = acosf(cosBValue);
	}
	else if(cosBValue < 0 && sinAValue > 0)   //第二象限
	{
		if(cosBValue < -0.707)
			Angle = VL_PI - asinf(sinAValue);
		else
			Angle = acosf(cosBValue);
	}
	else if(cosBValue < 0 && sinAValue < 0)    //第三象限
	{
		if(cosBValue < -0.707)
			Angle = VL_PI - asinf(sinAValue);
		else
			Angle = 2*VL_PI - acosf(cosBValue);
	}
	else if(cosBValue > 0 && sinAValue < 0)     //第四象限
	{
		if(cosBValue > 0.707)
			Angle = 2*VL_PI + asinf(sinAValue);
		else
			Angle = 2*VL_PI - acosf(cosBValue);
	}

	Angle = Angle*180/VL_PI;

	return Angle;
}

//基于对象的cosA与sinA值计算其角度，角度范围包括[-90,90],[0,180],[0,360]
float LineAngleByLinePara(float sinAValue, float cosBValue, int AngleRangle)
{
	float Angle = 0;

	switch(AngleRangle)
	{
	case 0:
		Angle = LineAngle90To90(sinAValue, cosBValue);
		break;
	case 90:
		Angle = LineAngle0To180(sinAValue, cosBValue);
		break;
	case 180:
		Angle = LineAngle0To360(sinAValue, cosBValue);
		break;
	default:
		Angle = 0;
	}

	return Angle;
}

//指定的两个点对象构建线段对象
DLLAPI float STDCALL DotDotAngle(CvPoint Point1, CvPoint Point2, int AngleRange)
{
	float LineLength = sqrtf((float)((Point2.x - Point1.x)*(Point2.x - Point1.x) 
		+ (Point2.y - Point1.y)*(Point2.y - Point1.y)));

	float cosBValue = (float)(Point2.x - Point1.x)/LineLength;
	float sinAValue = (float)(Point2.y - Point1.y)/LineLength;

	float Angle = LineAngleByLinePara(sinAValue, cosBValue, AngleRange);

	return Angle;
}

DLLAPI CvPoint STDCALL LineLineIntersection(cvLineObj InLine1, cvLineObj InLine2)
{
	CvPoint OutPoint;

	float MoleValue = 0, DeloValue = 0;

	MoleValue = InLine1.cosBValue*InLine2.InterceptValue - InLine2.cosBValue*InLine1.InterceptValue;
	DeloValue = InLine1.sinAValue*InLine2.cosBValue - InLine2.sinAValue*InLine1.cosBValue;

	OutPoint.x = (int)(MoleValue/DeloValue + 0.5);

	MoleValue = -InLine2.sinAValue*InLine1.InterceptValue + InLine1.sinAValue*InLine2.InterceptValue;
	DeloValue = InLine1.sinAValue*InLine2.cosBValue - InLine2.sinAValue*InLine1.cosBValue;

	OutPoint.y = (int)(MoleValue/DeloValue + 0.5);

	return OutPoint;
}

DLLAPI float STDCALL LineLineIncludeAngle(cvLineObj InLine1, cvLineObj InLine2, int AngleRangle)
{
	float sinAValue = InLine2.sinAValue*InLine1.cosBValue - InLine2.cosBValue*InLine1.sinAValue;
	float cosBValue = InLine2.cosBValue*InLine1.cosBValue + InLine2.sinAValue*InLine1.sinAValue;

	float Angle = LineAngleByLinePara(sinAValue, cosBValue, AngleRangle);

	return Angle;
}

DLLAPI float STDCALL DotLineDisAbs(CvPoint Point, cvLineObj InLine)
{
	float dis = Point.x*InLine.sinAValue - Point.y*InLine.cosBValue + InLine.InterceptValue;

	return abs(dis);
}

DLLAPI float STDCALL DotLineDis(CvPoint Point, cvLineObj InLine)
{
	float dis = Point.x*InLine.sinAValue - Point.y*InLine.cosBValue + InLine.InterceptValue;

	return dis;
}

DLLAPI float STDCALL LineLineDis(cvLineObj InLine1, cvLineObj InLine2)
{
	float dis1 = DotLineDisAbs(InLine1.Point1, InLine2);
	float dis2 = DotLineDisAbs(InLine1.Point2, InLine2);

	return (dis1 + dis2)/2;
}

DLLAPI float STDCALL LineLineDotDis(cvLineObj InLine1, cvLineObj InLine2)
{
	//基于线段1的两个端点到线段2的距离的均值
	float XValue1 = (InLine1.Point1.x + InLine1.Point2.x)/2, YValue1 = (InLine1.Point1.y + InLine1.Point2.y)/2;
	float XValue2 = (InLine2.Point1.x + InLine2.Point2.x)/2, YValue2 = (InLine2.Point1.y + InLine2.Point2.y)/2;

	float dis = sqrtf((XValue1 - XValue2)*(XValue1 - XValue2) + (YValue1 - YValue2)*(YValue1 - YValue2));

	return dis;
}