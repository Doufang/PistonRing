#ifndef DLL_Camera
#define DLL_Camera
#endif

#include "CameraBasler.h"

//类point的缺省构造函数

CameraBasler::CameraBasler()
{
	cameraIP = "";
	cameraSN = "";
	manualExposureTime = 10000.0;
	manualFrameRate = 10.0;
	cameraWidth = 640;
	cameraHeight = 480;
	cameraRun = false;
}

CameraBasler::~CameraBasler()
{
	CloseCamera();
}

//打开相机
bool CameraBasler::OpenCamera(String_t cameraStr)
{
	try
	{
		CTlFactory& TlFactory = CTlFactory::GetInstance();
		size_t n = cameraStr.length();
		if (n < 11)
		{
			//使用SN号连接相机			
			CDeviceInfo di;
			di.SetSerialNumber(cameraStr);
			IPylonDevice* devices = TlFactory.CreateDevice(di);
			myCamera.Attach(devices);
			cameraSN = cameraStr;
		}
		else
		{
			//使用IP			
			CBaslerGigEDeviceInfo diIP;
			diIP.SetIpAddress(cameraStr);
			IPylonDevice* deviceIP = TlFactory.CreateDevice(diIP);
			myCamera.Attach(deviceIP);
			cameraIP = cameraStr;
		}
		myCamera.Open();

		GenApi::INodeMap *m_pCameraNodeMap;
		m_pCameraNodeMap = &myCamera.GetNodeMap();
		CEnumerationPtr ptrPixelFormat = m_pCameraNodeMap->GetNode("PixelFormat");
		ptrPixelFormat->FromString("Mono8");
		cameraRun = true;
	}
	catch (GenICam::GenericException &e)
	{
		return false;
	}
	return true;
}

void CameraBasler::CloseCamera()
{
	if (myCamera.IsPylonDeviceAttached())
	{
		myCamera.StopGrabbing();
		myCamera.Close();
		myCamera.DetachDevice();
		if (myImage)
		{
			cvReleaseImage(&myImage);
		}
		cameraRun = false;
	}
}

bool CameraBasler::SetCameraPixels(int64_t pixelWidth, int64_t pixelHeight)
{
	try
	{
		// Get the camera control object.
		INodeMap &control = myCamera.GetNodeMap();

		// 获取相机的图片尺寸信息节点
		const CIntegerPtr width = control.GetNode("Width");
		const CIntegerPtr height = control.GetNode("Height");

		//设置相机的采集尺寸
		width->SetValue(pixelWidth);
		height->SetValue(pixelHeight);
		cameraWidth = pixelWidth;
		cameraHeight = pixelHeight;
		if (myImage)
		{
			cvReleaseImage(&myImage);
		}
		myImage = cvCreateImage(cvSize(cameraWidth, cameraHeight), IPL_DEPTH_8U, 1);
	}
	catch (GenICam::GenericException& e)
	{
		return false;
	}
	return true;
}

bool CameraBasler::SetCameraExposureTime(double cameraExposureTime)
{
	try
	{
		// Get the camera control object.
		INodeMap &control = myCamera.GetNodeMap();

		//获取相机曝光信息节点
		CEnumerationPtr exposureAuto = control.GetNode("ExposureAuto");
		const CFloatPtr exposureTime = control.GetNode("ExposureTimeAbs");

		//设置自动曝光为off。0为off，1为一次自动曝光，2为连续自动曝光。
		exposureAuto->SetIntValue(0);

		//设置曝光时间，单位为微秒。
		exposureTime->SetValue(cameraExposureTime);
		manualExposureTime = cameraExposureTime;
	}
	catch (GenICam::GenericException& e)
	{
		return false;
	}
	return true;
}

bool CameraBasler::SetCameraFrameRate(double cameraFrameRate)
{
	try
	{
		// Get the camera control object.
		INodeMap &control = myCamera.GetNodeMap();

		//获取相机帧率信息节点
		CBooleanPtr frameEnable = control.GetNode("AcquisitionFrameRateEnable");
		const CFloatPtr frameRate = control.GetNode("AcquisitionFrameRateAbs");

		//设置允许调节相机帧率。0为禁止，1为允许。
		frameEnable->SetValue(1);

		//设置帧率，单位为Hz。
		frameRate->SetValue(cameraFrameRate);
		manualFrameRate = cameraFrameRate;
	}
	catch (GenICam::GenericException& e)
	{
		return false;
	}
	return true;
}

bool CameraBasler::GrabImage()
{
	CGrabResultPtr resultGrab;
	CPylonImage pylonImage;
	try
	{
		if (myCamera.IsPylonDeviceAttached())
		{
			myCamera.GrabOne(5000, resultGrab, TimeoutHandling_ThrowException);

			if (resultGrab->GrabSucceeded())
			{
				pylonImage.AttachGrabResultBuffer(resultGrab);
				// 获取图像数据地址
				uint8_t* pImageBuffer = static_cast<uint8_t*>(pylonImage.GetBuffer());
				uchar* data = (uchar*)myImage->imageData;
				memcpy(data, pImageBuffer, pylonImage.GetImageSize());
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	catch (GenICam::GenericException &e)
	{
		return false;
	}
	return true;
}

void CameraBasler::CameraInit(String_t cameraString, double exposureTime, double frameRate, int64_t width, int64_t height)
{
	if (OpenCamera(cameraString))
	{
		SetCameraExposureTime(exposureTime);
		SetCameraFrameRate(frameRate);
		SetCameraPixels(width, height);
	}
}