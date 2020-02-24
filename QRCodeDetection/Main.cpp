#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#pragma comment(lib,"opencv_world420d")

using namespace cv;
using namespace std;

void imageTophat(const string& in_file)
{
	Mat m = imread(in_file);
	cvtColor(m, m, COLOR_RGB2GRAY);
	threshold(m, m, 100, 200, THRESH_BINARY);
	Mat se = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat dst;
	erode(m, dst, se);	
	imshow("TopHat", m - dst);
	waitKey();
}

void drawQRCodeContour(Mat& color_image, vector<Point> transform)
{
	if (!transform.empty())
	{
		double show_radius = (color_image.rows  > color_image.cols)
			? (2.813 * color_image.rows) / color_image.cols
			: (2.813 * color_image.cols) / color_image.rows;
		double contour_radius = show_radius * 0.4;

		vector< vector<Point> > contours;
		contours.push_back(transform);
		drawContours(color_image, contours, 0, Scalar(211, 0, 148), cvRound(contour_radius));

		RNG rng(1000);
		for (size_t i = 0; i < 4; i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			circle(color_image, transform[i], cvRound(show_radius), color, -1);
		}
	}
}

void imageQRCodeDetect(const string& in_file)
{
	Mat color_src = imread(in_file, IMREAD_COLOR);
	Mat src;
	cvtColor(color_src, src, COLOR_BGR2GRAY);
	
	vector<Point> transform;
	QRCodeDetector qrcode;
	bool result_detection = qrcode.detect(src, transform);
	if (result_detection)
	{
		Mat straight_barcode;
		string decoded_info = qrcode.decode(src, transform, straight_barcode);
		if (!decoded_info.empty())
		{
			cout << "size: " << color_src.size() << endl;
			cout << "info: " << decoded_info << endl;
			drawQRCodeContour(color_src, transform);
			//putText(color_src, decoded_info, Point(10, 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(1, 0, 0));
		}
		else
		{
			cout << "no info" << endl;
		}
	}
	else
	{
		cout << "detection failed" << endl;
	}

	imshow("QRCodeDetect", color_src);
	waitKey();
}

int main(void)
{
	cout << "ImageFile:";
	string file;
	cin >> file;
	//imageTophat(file);
	imageQRCodeDetect(file);
	return 0;
}
