//엄채윤 미디어학과 202127252
//Connected Components 구현 프로그램
//23.06.19
#include <iostream>
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

// Public 변수 선언
Mat img;


void mouseEvent(int event, int x, int y, int flag, void* dstImage) {
	
	Mat output = Mat::zeros(img.rows, img.cols, 0);
	Mat tmpImg, dilate_img, and_img, result;
	
	if (event == EVENT_LBUTTONDOWN) {

		//Connected Components
		output.at<uchar>(Point(x,y)) = 255; //점 찍기

		// 최초실행
		// dilate + and 연산
		dilate(output, dilate_img, Mat()); // Mat() : default SE >> 3x3 정사각형
		bitwise_and(img, dilate_img, and_img);

		//이전 상태 저장
		tmpImg = and_img.clone();

		//여러번의 dilate & and 실행
		while (true) {		

			// dilate + and 연산
			dilate(and_img, dilate_img, Mat()); // Mat() : default SE >> 3x3 정사각형
			bitwise_and(img, dilate_img, and_img);

			//compare(and_img, tmpImg, result, CMP_EQ);
			if (countNonZero(and_img) == countNonZero(tmpImg)) {
				and_img *= 255;
				imshow("result", and_img);
				break;
			}

			// 이전 실행 저장
			tmpImg = and_img.clone();
		}

		
		
	}
}


int main()
{
	//32F : float, C1 : 한 채널(흑백 이미지), zeros : 0으로 차 있는 이미지
	img = cv::imread("C:/Users/엄채윤/Desktop/connectedComponents.png", 0); //input
	cv::imshow("img", img); //original
	setMouseCallback("img", mouseEvent, &img);
	cv::waitKey();

}