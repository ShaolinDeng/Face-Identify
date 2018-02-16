#include "base64.h"
#include <iostream>
#include <string.h>
#include <json.h>
#include <fstream>
#include <sstream>
#include<vector> 
#include<opencv2/opencv.hpp> 
#include <cv.h>
#include <highgui.h>
#include "detect.h"

using namespace cv;
const char* file = "test.txt";
const char* picname = "1.jpg";

int main() {
	VideoCapture  capture(0);
	Mat frame;
	
	if (!capture.isOpened())
	{
		cout << "摄像头打开失败！" << endl;
		return -1;
	}
	char key;
	char filename[200];

	namedWindow("【视频】", 1);
	namedWindow("【图片】", 1);

	int count = 0;
	while (1)
	{
		key = waitKey(50);
		capture >> frame;
		imshow("【视频】", frame);

		if (key == 27)
			break;//按ESC键退出程序  
		if (key == 32)//按空格键进行拍照  
		{
			sprintf(filename, "1.jpg",++count);
			imwrite(filename, frame);
			imshow("【图片】", frame);
			std::string access_token;
			const static std::string AK = "eb3M8gTn19iYbghBb3qceQDz";
			const static std::string SK = "y1XLzsvQyVb3TdCvtjY4jtxhWK3GCyw0";
			get_access_token(access_token, AK, SK);
			std::string result, result1;
			std::string test;
			fileBase64Encode(picname, file);
			char * fn = "test.txt";
			test = readFileIntoString(fn);
			face_detect(result, test, access_token);
			std::cout << result << std::endl;
		}

	}

	/*cout << imgbase64 << endl;
		cv::Mat img_encode;
		img_encode = imread("C:\\Users\\Angelo\\Pictures\\2.jpg", CV_LOAD_IMAGE_COLOR);

		encode image and save to file  
		std::vector<uchar> data_encode;
		imencode(".jpeg", img_encode, data_encode);
		std::string str_encode(data_encode.begin(), data_encode.end());
		cout << str_encode << endl;
		int i = str_encode.length()+100;
		char c[104957];
		strcpy(c, str_encode.c_str());
		cout << c << endl;
		printf("%x", c);
		ofstream fout;
		 istream fin;
		fout.open("C:\\Users\\Angelo\\Music\\1.txt");
		fout << c << endl;
		fout.close();
		unsigned char *p = img.ptr();
		cout << p << endl;
		getchar();
		const char* pSrc = "wocaonima";*/

	
	printf("success!\n");
	std::string access_token;
	const static std::string AK = "eb3M8gTn19iYbghBb3qceQDz";
	const static std::string SK = "y1XLzsvQyVb3TdCvtjY4jtxhWK3GCyw0";
	get_access_token(access_token, AK, SK);
	std::string result, result1;
	std::string test;
	fileBase64Encode(picname, file);
	char * fn = "test.txt";
	test = readFileIntoString(fn);
	face_detect(result, test, access_token);
	std::cout << result << std::endl;
		
}


