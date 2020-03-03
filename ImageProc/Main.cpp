#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>

#pragma comment(lib,"opencv_world420d")

using namespace cv;
using namespace std;

void ImageBlur(string imgFile);
void ImageBlend(string imgFile1, double alpha1, string imgFile2, string dstFile);
void BackgroundInvert(string imgFile, cv::Scalar bgValue = cv::Scalar(0, 0, 255, 0));

int main(int argc, char** argv)
{
	cout << "input image filename:";

	//ImageBlur("1.jpg");
	//ImageBlend("1.jpg", "2.jpg", 0.8, "result.png");
	string fn;
	cin >> fn;
	BackgroundInvert(fn);

	waitKey();

	return 0;
}

//=======================================
// src image = (R, G, B)
// dst image = (0.8*R, 0.8*G, 0.8*B, A)
// where A = 0~255, we set it to 128
//=======================================
void ImageBlur(string imgFile)
{
	Mat srcImage = imread(imgFile, IMREAD_UNCHANGED);
	int rows = srcImage.rows;
	int cols = srcImage.cols;

	assert(srcImage.channels() == 3);

	Mat dstImage = Mat(rows, cols, CV_8UC4, Scalar(0, 0, 0, 128));

	vector<Mat> srcChannels;
	vector<Mat> dstChannels;
	split(srcImage, srcChannels);
	split(dstImage, dstChannels);

	for (int i = 0; i < 3; ++i)
	{
		dstChannels[i] = 0.8*srcChannels[i];
	}

	merge(dstChannels, dstImage);

	string dstFile;
	size_t pos = imgFile.find_last_of('.');
	dstFile = imgFile.substr(0, pos) + "_blur.png";

	imwrite(dstFile, dstImage);

	imshow("blur", dstImage);
	waitKey(0);
}

void ImageBlend(string imgFile1, double alpha1, string imgFile2, string dstFile)
{
	Mat img1 = imread(imgFile1);
	Mat img2 = imread(imgFile2);
	Mat dst;

	addWeighted(img1, alpha1, img2, 1.0 - alpha1, 0.0, dst);
	imwrite(dstFile, dst);

	imshow("blend", dst);
	waitKey(0);
}

void BackgroundInvert(string imgFile, cv::Scalar bgValue)
{
	Mat srcImage = imread(imgFile);
	vector<Mat> srcChannels;
	vector<Mat> dstChannels;
	split(srcImage, srcChannels);

	imshow("channel1", srcChannels[0]);
	imshow("channel2", srcChannels[1]);
	imshow("channel3", srcChannels[2]);

	waitKey(0);
}