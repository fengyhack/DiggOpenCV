#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <time.h>

#pragma comment(lib,"opencv_world310d")

using namespace cv;
using namespace std;

char filename[1024] = { 0 };

void curtime2filename(char* str, char* prefix, char* suffix)
{
	time_t current_time = time(NULL);
	tm* ptm = localtime(&current_time);

	int day = ptm->tm_wday;
	int hour = ptm->tm_hour + 8;
	day = hour >= 24 ? day + 1 : day;
	hour %= 24;

	sprintf(str, "%s%4d%02d%02d%02d%02d%02d%s",
		prefix, ptm->tm_year, ptm->tm_mon, day, hour, ptm->tm_min, ptm->tm_sec, suffix);
}

int main(int argc, char** argv)
{
	VideoCapture cap;

	// open the default camera using default API
	cap.open(0);

	// OR advance usage: select any API backend
	// 0 = open default camera
	int deviceID = 0;             
	
	// 0 = autodetect default API
	int apiID = cv::CAP_ANY;      
				
	// open selected camera using selected API
	cap.open(deviceID + apiID);

	// check if we succeeded
	if (!cap.isOpened()) 
	{
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl
		<< "Press Q to terminate" << endl
		<< "Press C to take snapshot" << endl;

	Mat frame;
	int key = 0;

	for (;;)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) 
		{
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", frame);
		key = waitKey(5);

		if ( key > 0 )
		{
			bool quit = false;
			switch (key)
			{
			case 'q':
			case 'Q':
				quit = true;
				break;
			case 'c':
			case 'C':
				curtime2filename(filename, "CAP_", ".jpg");
				imwrite(filename, frame);
				break;
			default:
				break;
			}

			if (quit)
			{
				break;
			}
		}
	}

	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}