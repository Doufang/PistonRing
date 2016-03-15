#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

using namespace cv;

#define VL_MIN(x,y) (((x)<(y))?(x):(y))
#define VL_MAX(x,y) (((x)>(y))?(x):(y))

#define MAX_GRAY_VALUE 256
#define MIN_GRAY_VALUE 0

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


int main()
{
	/************************************************************************/
	/* 做相应的图像处理操作                                                 */
	/************************************************************************/
		
	IplImage* imgTest = cvLoadImage("E:\\wuxi\\new\\2506.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	CvSize ImgRect;
	//ImgRect.height = 1234;
	//ImgRect.width  = 1624;
	
	//test图片大小
	ImgRect.height = 460;
	ImgRect.width  = 640;
	IplImage* GrayImage = cvCreateImage(cvGetSize(imgTest), IPL_DEPTH_8U, 1);
	memcpy(GrayImage->imageData, imgTest->imageData, GrayImage->height*GrayImage->widthStep*sizeof(unsigned char));
	
	IplImage* MarkImage = cvCreateImage(cvGetSize(imgTest), IPL_DEPTH_8U, 1);
	memset(MarkImage->imageData, 0, MarkImage->height*MarkImage->widthStep*sizeof(unsigned char));

	IplImage* BinaryImage = cvCreateImage(cvGetSize(imgTest), IPL_DEPTH_8U, 1);
	memset(BinaryImage->imageData, 0, BinaryImage->height*BinaryImage->widthStep*sizeof(unsigned char));

	IplImage* dstImage = cvCreateImage(cvGetSize(imgTest), IPL_DEPTH_8U, 1);
	memset(dstImage->imageData, 0, dstImage->height*dstImage->widthStep*sizeof(unsigned char));

	//ListRect histRectList;  ListInt histIntList;
	//histRectList.Item = (ArrayRect *)MallocArrayRect();
	//histIntList.Item = (ArrayInt *)MallocArrayInt();

	/*if (bIsColor)
	{
		cvCvtColor(mcvImg,GrayImage,CV_RGB2GRAY);
	}	*/				

	int thd = otsu(GrayImage);
	cvThreshold(GrayImage, BinaryImage, thd, 255, CV_THRESH_BINARY); 
	cvShowImage("grayImage.jpg", GrayImage);
	cvShowImage("BinaryImage.jpg", BinaryImage);
	waitKey(0);
	cout<<endl<<"thd is "<<thd<<endl;
}
