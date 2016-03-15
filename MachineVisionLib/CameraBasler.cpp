#ifndef DLL_Camera
#define DLL_Camera
#endif

#include "CameraBasler.h"

//��point��ȱʡ���캯��

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

//�����
bool CameraBasler::OpenCamera(String_t cameraStr)
{
	try
	{
		CTlFactory& TlFactory = CTlFactory::GetInstance();
		size_t n = cameraStr.length();
		if (n < 11)
		{
			//ʹ��SN���������			
			CDeviceInfo di;
			di.SetSerialNumber(cameraStr);
			IPylonDevice* devices = TlFactory.CreateDevice(di);
			myCamera.Attach(devices);
			cameraSN = cameraStr;
		}
		else
		{
			//ʹ��IP			
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

		// ��ȡ�����ͼƬ�ߴ���Ϣ�ڵ�
		const CIntegerPtr width = control.GetNode("Width");
		const CIntegerPtr height = control.GetNode("Height");

		//��������Ĳɼ��ߴ�
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

		//��ȡ����ع���Ϣ�ڵ�
		CEnumerationPtr exposureAuto = control.GetNode("ExposureAuto");
		const CFloatPtr exposureTime = control.GetNode("ExposureTimeAbs");

		//�����Զ��ع�Ϊoff��0Ϊoff��1Ϊһ���Զ��ع⣬2Ϊ�����Զ��ع⡣
		exposureAuto->SetIntValue(0);

		//�����ع�ʱ�䣬��λΪ΢�롣
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

		//��ȡ���֡����Ϣ�ڵ�
		CBooleanPtr frameEnable = control.GetNode("AcquisitionFrameRateEnable");
		const CFloatPtr frameRate = control.GetNode("AcquisitionFrameRateAbs");

		//��������������֡�ʡ�0Ϊ��ֹ��1Ϊ����
		frameEnable->SetValue(1);

		//����֡�ʣ���λΪHz��
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
				// ��ȡͼ�����ݵ�ַ
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