//엄채윤 미디어학과 202127252
//Median Filter 구현 프로그램
//23.04.17
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

Mat myMedian(const Mat& img, const Size& sz) {
	assert(sz.width % 2 == 1);
	
	Mat ret(img.size(), img.type());
	int wx = (sz.width - 1) / 2;
	int wy = (sz.height - 1) / 2;

	for (int y = 0; y < ret.rows; y++) for (int x = 0; x < ret.cols; x++) {
		vector<int> filterSize;
		int pixel = 0;
		
		for (int dy = -wy; dy <= wy; dy++) for(int dx = -wx; dx <= wx; dx++) {
			int xx = x + dx;
			int yy = y + dy;
			
			if (xx >= 0 && yy >= 0 && xx < img.cols && yy < img.rows) {
				pixel = img.at<uchar>(yy, xx);
				filterSize.push_back(pixel);
			}
			else {
				pixel = 0;
				filterSize.push_back(pixel);
			}
		}
		sort(filterSize.begin(), filterSize.end());
		ret.at<uchar>(y, x) = filterSize.at(filterSize.size()/2);
	}
	return ret;
}

/*
Mat box(const Mat& img, const Size& sz) {
	assert(sz.width % 2 == 1);
	Mat ret(img.size(), img.type()); //같은 크기와 같은 해상도(픽셀), L값이 동일
	int wx = (sz.width - 1) / 2;
	int wy = (sz.height - 1) / 2;
	
	for (int y = 0; y < ret.rows; y++) for (int x = 0; x < ret.cols; x++) {
		int sum = 0;
		for (int dy = -wy; dy <= wy; dy++) for (int dx = -wx; dx <= wx; dx++) {
			int xx = x + dx;
			int yy  = y + dy;
			//if (xx < 0) xx = 0;
			//else if (xx > img.cols - 1) xx = img.cols - 1;
			//if (yy < 0) yy = 0;
			//else if (yy > img.cols - 1) yy = img.rows - 1;
			//sum += img.at<uchar>(yy, xx); //x, y에 저장된 픽셀을 unsigned로 읽어온 것
			
			//0 padding
			if (xx >= 0 && yy >= 0 && xx < img.cols && yy < img.rows)
				sum += img.at<uchar>(yy, xx);
			else
				sum += 0;
		}
		ret.at<uchar>(y, x) = sum / ((wx * 2 + 1) * (wy * 2 + 1));
	}
	return ret;
}

Mat myGaussian (const Mat& img, const Size& sz) {
	assert(sz.width % 2 == 1);
	Mat ret(img.size(), img.type()); //같은 크기와 같은 해상도(픽셀), L값이 동일
	int wx = (sz.width - 1) / 2;
	int wy = (sz.height - 1) / 2;

	for (int y = 0; y < ret.rows; y++) for (int x = 0; x < ret.cols; x++) {
		float sum = 0;
		float wsum = 0;
		for (int dy = -wy; dy <= wy; dy++) for (int dx = -wx; dx <= wx; dx++) {
			int xx = x + dx;
			int yy = y + dy;
			if (xx < 0) xx = 0;
			else if (xx > img.cols - 1) xx = img.cols - 1;
			if (yy < 0) yy = 0;
			else if (yy > img.cols - 1) yy = img.rows - 1;
			float w = exp(-(dx * dx + dy * dy) / (wx * wx));
			wsum += w;
			sum += w*img.at<uchar>(yy, xx); //x, y에 저장된 픽셀을 unsigned로 읽어온 것

		}
		ret.at<uchar>(y, x) = sum / wsum;
	}
	return ret;
}

Mat boxSep(const Mat& img, bool vert, int sz) {
	assert(sz % 2 == 1);
	Mat ret(img.size(), img.type()); //같은 크기와 같은 해상도(픽셀), L값이 동일
	int w = (sz - 1) / 2;
	int w = (sz - 1) / 2;

	for (int y = 0; y < ret.rows; y++) for (int x = 0; x < ret.cols; x++) {
		int sum = 0;
		if (!vert) {
			for (int dx = -w; dx <= w; dx++) {
				int xx = x + dx;
				sum += img.at<uchar>(y, (xx + img.cols) % img.cols);
			}
		}
		else {
			for (int dy = -w; dy <= w; dy++) {
				int yy = y + dy;
				sum += img.at<uchar>((yy + img.rows) % img.rows, x);
			}
		}
		ret.at<uchar>(y, x) = sum / ((w * 2 + 1) * (w * 2 + 1));
		}
	
	return ret;
}
*/

int main()
{
	Mat medianFilter;
	Mat image = cv::imread("C:/Users/엄채윤/Desktop/image.jpg", 0); //읽을 때 흑백으로 한 채널짜리로 읽음, 8비트
	Mat img2 = myMedian(image, Size(15,15));
	//medianBlur(image, medianFilter, 15); //비교를 위한 median 필터
	cv::imshow("image", image);
	cv::imshow("image2", img2);
	//cv::imshow("image3", medianFilter);
	cv::waitKey();
}
