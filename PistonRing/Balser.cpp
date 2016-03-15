#include "stdafx.h"
#include "Balser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool OpenCamera(const String_t& CameraIP)
{
	try
	{
		CTlFactory& TlFactory = CTlFactory::GetInstance();
		CDeviceInfo di;

		//通过设置IP地址来连接特定相机
		CBaslerGigEDeviceInfo diIP;
		diIP.SetIpAddress(CameraIP);
		IPylonDevice* deviceIP = TlFactory.CreateDevice(diIP);

		CInstantCamera m_camera;
		m_camera.Attach(deviceIP);

		m_camera.Open();

		GenApi::INodeMap *m_pCameraNodeMap;
		m_pCameraNodeMap = &m_camera.GetNodeMap();
		CEnumerationPtr ptrPixelFormat = m_pCameraNodeMap->GetNode("PixelFormat");

		ptrPixelFormat->FromString("Mono8");

		Pylon::PixelType m_PixelFormat;
		m_PixelFormat = PixelType_Mono8;
		bool bIsColor = false;
		return true;

	}
	catch (GenICam::GenericException &e)
	{
		std::cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;

	}
	return false;
}
