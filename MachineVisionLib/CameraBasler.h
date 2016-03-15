#pragma once

#include "CameraPublic.h"


class CameraAPI CameraBasler
{
public:
	CameraBasler();
	~CameraBasler();

public:
	CInstantCamera	myCamera;
	IplImage*		myImage;
	String_t		cameraIP;
	String_t		cameraSN;
	double			manualExposureTime;
	double			manualFrameRate;
	int64_t			cameraWidth;
	int64_t			cameraHeight;
	bool			cameraRun;

private:
	//打开相机
	bool OpenCamera(String_t cameraStr);
	//关闭相机
	void CloseCamera();

public:
	void CameraInit(String_t cameraString, double exposureTime, double frameRate, int64_t width, int64_t height);
	
	bool SetCameraPixels(int64_t pixelWidth, int64_t pixelHeight);

	bool SetCameraExposureTime(double cameraExposureTime);
	
	bool SetCameraFrameRate(double cameraFrameRate);

	bool GrabImage();

};

