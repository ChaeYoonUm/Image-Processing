//엄채윤 미디어학과 202127252
//Wiener Filtering 구현 프로그램
//23.05.25

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_world470d")
#else
#pragma comment(lib,"opencv_world470")
#endif
using namespace cv;
using namespace std;

/*
Mat degrade(const Mat& src, const Mat& kernel) {
	Mat sf, kf, rf, ret; //source FFT, kernel FFT
	dft(src, sf, DFT_COMPLEX_OUTPUT);
	dft(kernel, kf, DFT_COMPLEX_OUTPUT);
	//kf*sf => 곱하기는 행렬끼리의 곱
	//but, 이미지끼리의 곱을 해야함
	//multiply : 실수는 실수끼리, 허수는 허수끼리 곱해야 하므로 사용X
	mulSpectrums(sf, kf, rf, DFT_ROWS);
	dft(rf, ret, DFT_INVERSE | DFT_SCALE|DFT_REAL_OUTPUT);
	return ret;
}*/

Mat wienerFilter(const Mat& src, const Mat& kernel, const float k) {
	Mat sf, kf, rf, ret; 
	dft(src, sf, DFT_COMPLEX_OUTPUT);
	dft(kernel, kf, DFT_COMPLEX_OUTPUT);
	rf.create(sf.size(), sf.type());
	
	
	for (Point pt(0, 0); pt.y < rf.rows; pt.y++) {
		for (pt.x = 0; pt.x < rf.cols; pt.x++) {
			// (a+bi)/(c+di) = (a+bi)*(c-di)/(c*c+d*d)
			// {(a*c+b*d)} + i(bc-ad)} / (c*c+d*d)
			float a = sf.at<Vec2f>(pt)[0];
			float b = sf.at<Vec2f>(pt)[1];
			float c = kf.at<Vec2f>(pt)[0];
			float d = kf.at<Vec2f>(pt)[1];

			
			
			/*
			float dx = pt.x > kf.cols / 2 ? kf.cols - pt.x:pt.x;
			float dy = pt.y > kf.rows / 2 ? kf.rows - pt.y:pt.y;
			float D = sqrtf(dx * dx + dy * dy);
			if (D > 20) rf.at<Vec2f>(pt)= Vec2f(0,0); //주파수 70이상이면 0으로 만드는 연산
			*/

			rf.at<Vec2f>(pt) = Vec2f( a * c + b * d, b*c-a*d)/(c*c+d*d) * ((c*c+d*d)/((c*c+d*d)+k));
		}		
			
	}

	dft(rf, ret, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
	return ret;
}



int main()
{
	//32F : float, C1 : 한 채널(흑백 이미지), zeros : 0으로 차 있는 이미지
	Mat img = cv::imread("C:/Users/엄채윤/Desktop/Wiener_Input1.png", 0);
	Mat img2 = cv::imread("C:/Users/엄채윤/Desktop/Wiener_Input2.png", 0);
	Mat kernel = cv::imread("C:/Users/엄채윤/Desktop/Wiener_Kernel.png", 0);
	
	img.convertTo(img, CV_32F, 1 / 255.f);
	img2.convertTo(img2, CV_32F, 1/255.f);
	kernel.convertTo(kernel, CV_32F, 1 / 255.f);
	
	kernel /= sum(kernel)[0];

	Mat res1 = wienerFilter(img, kernel, 0.0007); // 0.009, 0.007, 0.0007(베스트)
	Mat res2 = wienerFilter(img2, kernel, 0.00429999); //0.00429999, 0.00430009, 0.00439999, 0.004499, 0.0045
	
	cv::imshow("img", img); //original69
	cv::imshow("img2", img2); //original
	cv::imshow("res1", res1);
	cv::imshow("res2", res2);
	cv::waitKey();
}