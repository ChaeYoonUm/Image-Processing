//엄채윤 미디어학과 202127252
//Homomorphic Filtering 구현 프로그램
//23.05.09

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

void visFreq(const Mat& image, Mat& output) {
	vector<Mat> ch;
	split(image, ch);
	magnitude(ch[0], ch[1], output);
	output += 1;
	log(output, output);
	output *= 0.1;
}

const float PI = 3.14159265358979;
int main()
{
	Mat img = cv::imread("C:/Users/엄채윤/Desktop/homomorphicInput.jpeg", 0);
	cv::imshow("img", img); //original
 	
	img.convertTo(img, CV_32F, 1/255.f);

	Mat F, out, img2;
	log(img+1, img);
	dft(img, F, DFT_COMPLEX_OUTPUT);

	//Homomorphic Filtering
	const float D0 = 10; //가우시안의 폭 결정 -> low ~ high freq 범위 결정
	const float k1 = 5; //없어진 부분 채우기
	const float k2 = 0.5; //조명효과
	for (Point pt(0, 0); pt.y < F.rows; pt.y++)
		for (pt.x = 0; pt.x < F.cols; pt.x++) {
			Vec2f v = F.at<Vec2f>(pt);
			float dx = pt.x < F.cols / 2 ? pt.x : F.cols - pt.x;
			float dy = pt.y < F.rows / 2 ? pt.y : F.rows - pt.y;
			float D = sqrtf(dx * dx + dy * dy);
			float H = (k1 - k2)*(1- exp(-D * D / (D0 * D0))) + k2;
			F.at<Vec2f>(pt) = v * H;
		}
	
	//inverse transform
	dft(F, img2, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	exp(img2-1, img2);

	
	cv::imshow("img2", img2); //output
	visFreq(F, out);
	cv::imshow("out", out); //frquency image
	cv::waitKey();
}
