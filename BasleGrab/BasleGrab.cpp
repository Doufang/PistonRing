#include "BasleGrab.h"


static const uint32_t c_countOfImagesToGrab = 1;


//#pragma comment(linker, "/DelayLoad:Dll.Dll")
//#pragma comment(linker, "/Delay:unload")
//extern "C" int _stdcall GrabImage(int a) 
//{
//	int exitCode = 0;
//
//	Pylon::PylonAutoInitTerm autoInitTerm;
//
//	IplImage* currImage = NULL;
//
//	try
//	{
//		CInstantCamera camer( CTlFactory::GetInstance().CreateFirstDevice());
//		cout<<"Using device "<<camer.GetDeviceInfo().GetModelName()<<endl;
//		camer.MaxNumBuffer = 5;
//
//		camer.StartGrabbing(c_countOfImagesToGrab);
//
//		CGrabResultPtr ptrGrabResult;
//
//		//ptrGrabResult->
//		while ( camer.IsGrabbing() )
//		{
//			camer.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
//			if ( ptrGrabResult->GrabSucceeded() )
//			{
//				cout<<" Image SizeX: "<<ptrGrabResult->GetWidth()<<endl;
//				cout<<" Image SizeY: "<<ptrGrabResult->GetHeight()<<endl;
//				
//				//CPylonImage pylonImage;
//				//pylonImage.AttachGrabResultBuffer(ptrGrabResult);
//				
//				/************************************************************************/
//				/* 例子使用的灰度图数据                                                 */
//				/************************************************************************/
//				//const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();//图像裸数据
//				//cvSetData(rawImage,(char*)pImageBuffer,rawImage->width);  //相机图像 转换至 OpenCV数据
//				//cvCvtColor( rawImage, rawImage0, CV_GRAY2BGR );           //转RGB，可显示彩框
//
//				//const uint32_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer;//图像裸数据
//				//cvSetData(ptrGrabResult,(char*)pImageBuffer,ptrGrabResult->GetWidth());  //相机图像 转换至 OpenCV数据
//				
//
//
//
//				//CImageFormatConverter converter;
//				//converter.Convert( pylonImage, ptrGrabResult);
//
//				////save the image
//				//CImagePersistence::Save( ImageFileFormat_Bmp, "GrabbedImage.bmp", ptrGrabResult);
//				//CImagePersistence::Save( ImageFileFormat_Jpeg, "GrabbedImage.jpg", ptrGrabResult);
//
//				uint32_t *pImageBuffer =  static_cast<uint32_t*>(ptrGrabResult->GetBuffer());
//				currImage = cvCreateImage(cvSize(4* (ptrGrabResult->GetWidth()),ptrGrabResult->GetHeight()), IPL_DEPTH_8U, 3);
//				currImage->imageData = (unsigned char *)pImageBuffer;
//				//cvShowImage("Img", currImage);
//				cvSaveImage("D:\\saveImage.jpg",currImage);
//
//
//				//uint32_t* pImageBuffer = static_cast<uint32_t*>(pylonImage.GetBuffer());
//
//
//				//currImage = (IplImage*)ptrGrabResult->GetBuffer();
//				
//				
//
//				//uint32_t* pImageBuffer = static_cast<uint32_t*>(ptrGrabResult->GetBuffer());
//				
//
//				//memcpy(currImage->imageData, ((IplImage*)pImageBuffer)->imageData, currImage->height*currImage->widthStep*sizeof(unsigned char));
//				//memset(currImage->imageData, 128, currImage->height*currImage->widthStep*sizeof(unsigned char));
//
//				//memcpy(currImage->imageData, pImageBuffer, currImage->height*currImage->widthStep*sizeof(unsigned char));
//				
//				//int step = ptrGrabResult->GetWidth() ;
//
//				//int dIndex = 0, hIndex = 0, wIndex = 0, TempPosition = 0, ImagePosition = 0, nodeIndex = 0;
//
//				//for (hIndex = 0; hIndex < ptrGrabResult->GetHeight(); hIndex++)
//				//{
//				//	//ImagePosition = hIndex * ptrGrabResult->GetWidth();
//				//	//TempPosition = hIndex * currImage->widthStep;
//				//	
//				//	int step = currImage->widthStep;
//				//	int step1 = ptrGrabResult->GetWidth();
//				//	for (wIndex = 0; wIndex <ptrGrabResult->GetWidth(); wIndex++)
//				//	{
//				//		ImagePosition = hIndex * ptrGrabResult->GetWidth() + wIndex;
//				//		
//				//		//TempPosition = hIndex * ptrGrabResult->GetWidth() + wIndex;
//				//		TempPosition = hIndex * currImage->widthStep + wIndex;
//				//	/*	uchar * pucPixel = (uchar*)currImage->imageData + hIndex * (currImage->widthStep);
//
//				//		pucPixel[wIndex + 0] = (uint8_t) pImageBuffer[ImagePosition + 3 * wIndex + 0];
//				//		pucPixel[wIndex + 1] = (uint8_t) pImageBuffer[ImagePosition + 3 * wIndex + 1];
//				//		pucPixel[wIndex + 2] = (uint8_t) pImageBuffer[ImagePosition + 3 * wIndex + 2];*/
//
//				//		currImage->imageData[ImagePosition + 3 * wIndex + 0] = (uint8_t) pImageBuffer[ImagePosition + 3 *  wIndex + 0];
//				//		currImage->imageData[ImagePosition + 3 * wIndex + 1] = (uint8_t) pImageBuffer[ImagePosition + 3 *  wIndex + 0];
//				//		currImage->imageData[ImagePosition + 3 * wIndex + 2] = (uint8_t) pImageBuffer[ImagePosition + 3 *  wIndex + 0];
//
//				//	}
//				//}
//
//				//cvSetImageData( currImage, (void *)pImageBuffer, ptrGrabResult->GetWidth()*3 );
//				//cvShowImage("Img", currImage);
//				//cvSaveImage("D:\\saveImage.jpg",currImage);
//				
//				cout<<"Gray value of first pixel: "<<(uint32_t) pImageBuffer[0]<<endl;
//				//Pylon::DisplayImage(1, ptrGrabResult);
//				waitKey(0);
//			}
//			else
//			{
//				cout<<"Error: "<<ptrGrabResult->GetErrorCode()<<" "<<ptrGrabResult->GetErrorDescription();
//			}
//		}
//	}
//	catch (GenICam::GenericException &e)
//	{
//		cerr<<"An exception occurred."<<endl
//			<<e.GetDescription()<<endl;
//		exitCode = 1;		
//	}
//	
//	cerr<<endl<<"Press Enter to exit."<<endl;
//	while(cin.get() != '\n');
//
//	cvReleaseImage(&currImage);
//	return exitCode;
//}


#define VL_MIN(x,y) (((x)<(y))?(x):(y))
#define VL_MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_GRAY_VALUE 256
#define MIN_GRAY_VALUE 0

void ShowImage( Pylon::IImage& image, const char* message = NULL)
{
	// Display the image.
	Pylon::DisplayImage(1, image);

	if ( message)
	{
		cout << endl << message << " ";
	}
}

int otsu(IplImage* dst)
{

	int i,j;
	int tmp;

	double u0,u1,w0,w1,u, uk;

	double cov;
	double maxcov=0.0;
	int maxthread=0;

	int hst[MAX_GRAY_VALUE]={0};
	double pro_hst[MAX_GRAY_VALUE]={0.0};

	int height=dst->height;
	int width=dst->width;


	int hIndex = 0, wIndex = 0, Position = 0;
	//统计每个灰度的数量
	for( hIndex =0 ; hIndex < height; hIndex++ )
	{
		
		for( wIndex = 0; wIndex < width; wIndex++)
		{
			Position = hIndex *dst->widthStep + wIndex;
			tmp = dst->imageData[Position];
			hst[tmp]++;
		}
	}

	//计算每个灰度级占图像中的概率
	for( i=MIN_GRAY_VALUE ; i<MAX_GRAY_VALUE; i++)
		pro_hst[i]=(double)hst[i]/(double)(width*height);


	//计算平均灰度值
	u=0.0;
	for( i=MIN_GRAY_VALUE; i<MAX_GRAY_VALUE; i++)
		u += i*pro_hst[i];

	double det=0.0;
	for( i= MIN_GRAY_VALUE; i< MAX_GRAY_VALUE; i++)
		det += (i-u)*(i-u)*pro_hst[i];

	//统计前景和背景的平均灰度值，并计算类间方差
	for( i=MIN_GRAY_VALUE; i<MAX_GRAY_VALUE; i++)
	{
		w0=0.0; w1=0.0; u0=0.0; u1=0.0; uk=0.0;

		for( j=MIN_GRAY_VALUE; j < i; j++)
		{
			uk += j*pro_hst[j];
			w0 += pro_hst[j];

		}
		u0=uk/w0;


		w1=1-w0;
		u1= (u - uk )/(1-w0);


		//计算类间方差
		cov=w0*w1*(u1-u0)*(u1-u0);

		if ( cov > maxcov )
		{
			maxcov=cov;
			maxthread=i;
		}
	}

	std::cout<<maxthread<<std::endl;
	return maxthread;

}



DLLAPI int STDCALL GrabImage(int a) 
{
    // The exit code of the sample application.
    int exitCode = 0;
	int nFrameNo = 0;
	int nCamNo = 0;
	CInstantCamera camera;
	DeviceInfoList_t devices; 
	// Create a target image.
	CPylonImage pylonImage;
	// Create the converter and set parameters.
	CImageFormatConverter converter;
	converter.OutputPixelFormat = PixelType_BGR8packed; //PixelType_RGBA8packed; //PixelType_Mono8;

	IplImage* mcvImg = NULL;
	CvSize mimgSize;
	bool   bIsColor = false;

	size_t lineStride;
    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
    // is initialized during the lifetime of this object.
    Pylon::PylonAutoInitTerm autoInitTerm;

    try
    {
        // Create an instant camera object with the camera device found first.		
		nCamNo = CTlFactory::GetInstance().EnumerateDevices(devices);
		if (nCamNo ==0 )
		{
			//cout << "No camera is found \n " << endl;
			MessageBox(NULL, "No camera is found!", "相机连接失败", MB_OKCANCEL);

		}
		else
		{
			// Stop the grab; close the pylon device; destroy the pylon device.
			// DestroyDevice does not throw C++ exceptions.
			camera.DestroyDevice();

			camera.Attach( CTlFactory::GetInstance().CreateDevice(devices[0]));

			// Print the model name of the camera.
			cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

			// Open the camera.
			camera.Open();

			// Get the camera control object.
			INodeMap &Control = camera.GetNodeMap();


			// Access the PixelFormat enumeration type node.
			CEnumerationPtr PtrpixelFormat( Control.GetNode( "PixelFormat"));

			// Set the pixel format to Mono8 if available.
			if ( IsAvailable( PtrpixelFormat->GetEntryByName( "BayerBG8")))
			{
				PtrpixelFormat->FromString( "BayerBG8");
				converter.OutputPixelFormat = PixelType_BGR8packed;//注意PixelType_RGB8packed不支持，只支持PixelType_BGR8packed
				bIsColor = true;
			}
			else if ( IsAvailable( PtrpixelFormat->GetEntryByName( "BayerGB8")))
			{
				PtrpixelFormat->FromString( "BayerGB8");
				converter.OutputPixelFormat = PixelType_BGR8packed;//注意PixelType_RGB8packed不支持，只支持PixelType_BGR8packed
				bIsColor = true;
			}
			else if ( IsAvailable( PtrpixelFormat->GetEntryByName( "Mono8")))
			{
				PtrpixelFormat->FromString( "Mono8");
				converter.OutputPixelFormat = PixelType_Mono8;
				bIsColor = false;
			}
			

			////-------------------------Set trigger mode start
			//// Get the trigger selector node.
			CEnumerationPtr triggerSelector(Control.GetNode("TriggerSelector"));
			// Get the trigger mode node.
			CEnumerationPtr triggerMode(Control.GetNode("TriggerMode"));

			 // Check the available camera trigger mode(s) to select the appropriate one: acquisition start trigger mode 
			// (used by previous cameras, i.e. for cameras supporting only the legacy image acquisition control mode;
			// do not confuse with acquisition start command) or frame start trigger mode
			// (used by newer cameras, i.e.for cameras using the standard image acquisition control mode;
			// equivalent to the acquisition start trigger mode in the leagcy image acquisition control mode).
			bool frameStartAvailable = false;
			bool acquisitionStartAvailable = false;
			{
				// Frame start trigger mode available?
				GenApi::IEnumEntry* frameStart = triggerSelector->GetEntryByName("FrameStart");
				frameStartAvailable = frameStart && IsAvailable(frameStart);

				// Acquisition start trigger mode available?
				GenApi::IEnumEntry* acquisitionStart = triggerSelector->GetEntryByName("AcquisitionStart");
				acquisitionStartAvailable = acquisitionStart && IsAvailable(acquisitionStart);
			}

			// Preselect the trigger mode for image acquisition.
			const char* triggerSelectorValue = "FrameStart";

			// Check to see if the camera implements the acquisition start trigger mode only.
			if ( acquisitionStartAvailable && !frameStartAvailable)
			{
				// Camera uses the acquisition start trigger as the only trigger mode.
				triggerSelector->FromString("AcquisitionStart");
				triggerMode->FromString("Off");
				triggerSelectorValue = "AcquisitionStart";
			}
			else  
			{   
				// Camera may have the acquisition start trigger mode and the frame start trigger mode implemented.
				// In this case, the acquisition trigger mode must be switched off.
				if ( acquisitionStartAvailable )
				{
					triggerSelector->FromString("AcquisitionStart");
					triggerMode->FromString("Off");
				}
				// To trigger each single frame by software or external hardware trigger: Enable the frame start trigger mode.
				assert( frameStartAvailable); //Frame start trigger mode must be available here.
				triggerSelector->FromString("FrameStart");
				triggerMode->FromString("Off");
			}

			// Note: The trigger selector must be set to the appropriate trigger mode 
			// before setting the trigger source or issuing software triggers.
			// Frame start trigger mode for newer cameras (i.e. for cameras supporting the standard image acquisition control mode).
			triggerSelector->FromString( triggerSelectorValue);

			// The trigger source must be set to 'Software'.
		   // CEnumerationPtr(Control.GetNode("TriggerSource"))->FromString("Software");

			////// The parameter MaxNumBuffer can be used to control the count of buffers
			// allocated for grabbing. The default value of this parameter is 10.
			camera.MaxNumBuffer = 15;

			//cout << "Grab using the GrabStrategy_OneByOne default strategy:" << endl << endl;

			camera.StartGrabbing(GrabStrategy_OneByOne);

			// This smart pointer will receive the grab result data.
			CGrabResultPtr ptrGrabResult;

			// Grab c_countOfImagesToGrab from the cameras.
			for( int i = 0; i < c_countOfImagesToGrab && camera.IsGrabbing(); ++i) 
			{
				//// Execute the software trigger. The call waits up to 200 ms for the camera 
				//// to be ready to be triggered.
				//if ( camera.WaitForFrameTriggerReady( 200, TimeoutHandling_ThrowException))
				//{
				//    camera.ExecuteSoftwareTrigger();
				//}


				//wait for level trigger signal.The call waits up to INFINITE ms for the camera 
				// to be ready to be triggered, you can change this to any time you need.
				camera.RetrieveResult( 500, ptrGrabResult, TimeoutHandling_ThrowException);

				// Image grabbed successfully?
				if (ptrGrabResult->GrabSucceeded())
				{					
					if ( converter.ImageHasDestinationFormat( ptrGrabResult))
					{
						// No conversion is needed. It can be skipped for saving processing
						// time.
						pylonImage.AttachGrabResultBuffer(ptrGrabResult);	
				//		ShowImage( ptrGrabResult, "The grabbed image is shown.");
					}
					else
					{
						// Conversion is needed.
						converter.Convert( pylonImage, ptrGrabResult);

						//cout << endl <<  "converter.Convert:  PixelType_RGB8packed";
				//		ShowImage( pylonImage, "The converted image is shown.");
						
						
					}

					pylonImage.GetStride(lineStride);
					//cout << endl <<  "lineStride = "<<lineStride;

					//CV Create;
					mimgSize.width = pylonImage.GetWidth();
					mimgSize.height = pylonImage.GetHeight();

					
					if (mcvImg == NULL)
					{
						if (bIsColor)
						{
							mcvImg = cvCreateImage(mimgSize,IPL_DEPTH_8U,3);
						}	
						else
						{
							mcvImg = cvCreateImage(mimgSize,IPL_DEPTH_8U,1);
						}
					}

					if (bIsColor)
					{
						// 获取图像数据地址
						uint8_t* pImageBuffer = static_cast<uint8_t*>(pylonImage.GetBuffer());
						uchar* data = (uchar*)mcvImg->imageData;
						memcpy(data, pImageBuffer, pylonImage.GetImageSize());
					//	mcvImg->imageData = (char *)pImageBuffer;

					/*	for(int ih=0;ih<mimgSize.height;ih++)
						 for(int iw=0;iw<mimgSize.width;iw++)
						{
							data[ih*mimgSize.width+iw] = pImageBuffer[ih*mimgSize.width+iw];
						}*/
					}
					else
					{
						// 获取图像数据地址
						uint8_t* pImageBuffer = static_cast<uint8_t*>(pylonImage.GetBuffer());
						uchar* data = (uchar*)mcvImg->imageData;
						memcpy(data, pImageBuffer, pylonImage.GetImageSize());
						//mcvImg->imageData = (char *)pImageBuffer;
					}			

					/************************************************************************/
					/* 做相应的图像处理操作                                                 */
					/************************************************************************/
						
					//IplImage* imgTest = cvLoadImage("E:\\wuxi\\leak\\1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
					CvSize ImgRect;
					ImgRect.height = 1234;
					ImgRect.width  = 1624;
					
					//test图片大小
					/*ImgRect.height = 460;
					ImgRect.width  = 640;*/
					IplImage* GrayImage = cvCreateImage(cvGetSize(mcvImg), IPL_DEPTH_8U, 1);
					memcpy(GrayImage->imageData, mcvImg->imageData, GrayImage->height*GrayImage->widthStep*sizeof(unsigned char));
					
					IplImage* MarkImage = cvCreateImage(cvGetSize(mcvImg), IPL_DEPTH_8U, 1);
					memset(MarkImage->imageData, 0, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

					IplImage* BinaryImage = cvCreateImage(cvGetSize(mcvImg), IPL_DEPTH_8U, 1);
					memset(BinaryImage->imageData, 0, BinaryImage->height*BinaryImage->widthStep*sizeof(unsigned char));

					IplImage* dstImage = cvCreateImage(cvGetSize(mcvImg), IPL_DEPTH_8U, 1);
					memset(dstImage->imageData, 0, dstImage->height*dstImage->widthStep*sizeof(unsigned char));


					ListRect histRectList;  ListInt histIntList;
					histRectList.Item = (ArrayRect *)MallocArrayRect();
					histIntList.Item = (ArrayInt *)MallocArrayInt();

					/*if (bIsColor)
					{
						cvCvtColor(mcvImg,GrayImage,CV_RGB2GRAY);
					}	*/				
	

					int thd = otsu(GrayImage);

					cvThreshold(GrayImage, BinaryImage, thd, 255, CV_THRESH_BINARY);  

					//cvDilationImage(IplImage* srcImage, int hModel, int wModel, int Objectgray, IplImage* dstImage)
					cvDilationImage(BinaryImage,7, 7, 255, dstImage);

					int reginNum = 0;

					//对历史图的二值化图像进行滤波，图像进行了4*4的缩减
					//FilterAllRegionRectList(histAdaptImage, histMarkImage, 255, cvRect(2, 2, histImage->width - 4, 
					//	histImage->height - 4), MinBlobNum, 1<<30, 100, KickEdge, histFilterImage, &histRectList, &histIntList);

					IplImage* FilterImage = cvCreateImage(cvGetSize(GrayImage), IPL_DEPTH_8U, 1);
					memcpy(FilterImage->imageData, GrayImage->imageData, FilterImage->height*FilterImage->widthStep*sizeof(unsigned char));
					reginNum = FilterAllRegionRectList(dstImage, MarkImage, 255, cvRect(2, 2, GrayImage->width - 4, GrayImage->height - 4), 20, 1<<30, 100, true, FilterImage,&histRectList, &histIntList);

					//cout<<"reginNum = "<<reginNum<<endl;
					//cvShowImage("cvImage",mcvImg);
					cvSaveImage("cvImage.bmp", mcvImg, NULL);
					cvSaveImage("AdaptImage.bmp", GrayImage, NULL);
					cvSaveImage("BinaryImage.bmp", BinaryImage, NULL);
					cvSaveImage("dstImage.bmp", dstImage, NULL);
					cvSaveImage("FilterImage.bmp", FilterImage, NULL);
					//cvWaitKey(0);
					//cvReleaseImage(&imgTest);
					cvReleaseImage(&GrayImage);
					cvReleaseImage(&MarkImage);
					cvReleaseImage(&BinaryImage);
					cvReleaseImage(&dstImage);
					cvReleaseImage(&FilterImage);


					
				}
				else
				{
					//cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription();
				}

				//cout << "press S to stop, press other button to continue next frame. \n " << endl;

			}


			camera.StopGrabbing();
			camera.Close();
            camera.DetachDevice();//DetachDevice

			//cvShowImage("Img", mcvImg);
			//cvSaveImage("D:\\saveImagemcvImg.jpg",mcvImg);

			if(mcvImg)
			{
				cvReleaseImage(&mcvImg);
			}
		}


    }
    catch (GenICam::GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    //cerr << endl << "Press Enter to exit." << endl;
    //while( cin.get() != '\n');

    return exitCode;
}


DLLAPI int STDCALL FilterAllRegionRectList(IplImage* srcImage, IplImage* MarkImage,int Objectgray, CvRect rect, int MinMarkNum,
										  int MaxMarkNum, float HeightWidthRatio, bool IsSide, IplImage* dstImage, ListRect* rectList, ListInt* intList)
{
	//统计变量，用于统计总的故障区域

	int errorNum = 0;

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
			}//while循环结束,即八领域标记结束

			RatioValue = (float)VL_MAX(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1)/VL_MIN(MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1);

			if (PointArray->size() > MinMarkNum || PointArray->size() < MaxMarkNum)
				errorNum++;

			if (PointArray->size() < MinMarkNum || PointArray->size() > MaxMarkNum) //不可以被滤除的斑点
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}//不可以被滤除的斑点结束
			else if(MaxXValue - MinXValue > 0.75*srcImage->width ||
				MaxYValue - MinYValue > 0.75*srcImage->height || RatioValue > HeightWidthRatio)
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinXValue <= rect.x + 1 || MaxXValue + 1 >=rect.x + rect.width))//基于X边缘滤波
			{
				//针对轴区域进行滤波
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else if(IsSide && (MinYValue <= rect.y + 1 || MaxYValue + 1 >=rect.y + rect.height))//基于Y边缘滤波
			{
				for (dIndex = 0; dIndex < PointArray->size(); dIndex++)
				{
					Position = (*PointArray)[dIndex].y*srcImage->widthStep + (*PointArray)[dIndex].x;
					dstImage->imageData[Position] = 255 - Objectgray;					
				}
			}
			else
			{
				//将相关区域添加到对应的矩形框架列表中
				AddArrayRect(rectList->Item, cvRect(MinXValue, MinYValue, MaxXValue - MinXValue + 1, MaxYValue - MinYValue + 1));
				AddArrayInt(intList->Item, PointArray->size());
			}

			if(PointArray->size() > 0)
				ClearArrayPoint(PointArray);
		}
	}//整个区域遍历结束

	FreeArrayPoint(PointArray);

	return errorNum;
}


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

