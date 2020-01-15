#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using std::cout;
using std::endl;
using std::string;
// https://www.cnblogs.com/kekeoutlook/p/10533730.html
// https://github.com/ruofeidu/ImageQualityCompare/blob/master/Compare/Compare.cpp

void print_mat(const cv::Mat &m) {
//void print_mat(const cv::Mat &m, int type) {
	int channels = m.channels();
	int height = m.rows;
	int width  = m.cols;
	cout << "print_mat layout:\tCHW (" << channels << ", " << height << ", " << width << ")" << endl;

	for(int c = 0; c != channels; ++c) {
		cout << "channels:\t" << c << endl;
		for(int h = 0; h != height; ++h) {
			const uchar* data = m.ptr<uchar>(h);

			//switch (type) {
			//case CV_U8:
			//	cout << "print_mat CV_U8 type:\t" << CV_U8 << endl;
			//	const uchar* data = m.ptr<uchar>(h);

			//case CV_32FC1:
			//	cout << "print_mat CV_U8 type:\t" << CV_U8 << endl;
			//	//const uchar* data = m.ptr<uchar>(h);
			//	const float* data = m.ptr<float>(h);

			//}
			for(int w = 0; w != width; ++w) {
				//cout << (int)data[w] << " ";
				cout << data[w] << " ";
				//cout << "(" << h << "," << w << ")[" << (int)data[w] << "]" << endl;
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl;
}

void show_binary(const string& path1) {
	cout << "show_binary path1:\t" << path1 << endl;
	cv::Mat m1;

	// for color
	//m1 = cv::imread(path1);
	// for gray
	m1 = cv::imread(path1, CV_8U);
	if (m1.empty()) {
		cout << "Failed to open file1" << endl;
	}
	
	print_mat(m1);
}

void resize_img(const std::string file) {
	cout << "[-] resize_img file:\t" << file << endl;

	//cv::Mat orig(cv::imread(file, CV_U8));
	//cv::Mat img = cv::imread(file, CV_32FC1);
	cout << "[-] resize file:\t" << file << endl;
	cv::Mat img = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);

	cv::Mat dst;
	// Enlarge
	//cv::resize(img, dst, cv::Size(1272, 712), cv::INTER_CUBIC);
	// Shrink
	cv::resize(img, dst, cv::Size(640, 360), cv::INTER_AREA);

	auto pos = file.find('.');
	std::string newImgPath(file.substr(0, pos) + "-640-360.png");
	cv::imwrite(newImgPath, dst);
}

void color2grey(const std::string file) {
	cout << "[-] color2grey file:\t" << file << endl;

	cv::Mat colorImg(cv::imread(file, CV_LOAD_IMAGE_COLOR));
	cout << "colorImg channels:\t" << colorImg.channels() << endl;
	//cv::namedWindow("color_window", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Color Image", colorImg);
	//cv::waitKey(0);
	//cv::destroyWindow("color_window");

	cv::Mat greyImg;
	//cv::namedWindow("grey_window", CV_WINDOW_AUTOSIZE);
	cv::cvtColor(colorImg, greyImg, cv::COLOR_BGR2GRAY);
	cout << "greyImg channels:\t" << greyImg.channels() << endl;
	//cv::imshow("Grey Image", greyImg);
	//cv::waitKey(0);
	//cv::destroyWindow("grey_window");

	auto pos = file.find('.');
	cout << "pos:\t" << pos << endl;
	//std::string gregImgPath(file.substr(0, pos) + "-grey" + file.substr(pos));
	std::string gregImgPath(file.substr(0, pos) + "-grey.png");
	cout << "save grey image to:\t" << gregImgPath << endl;
	cv::imwrite(gregImgPath, greyImg);


	// resize_img
	cv::Mat dst720;
	cv::resize(greyImg, dst720, cv::Size(1280, 720), cv::INTER_AREA);
	std::string path720(file.substr(0, pos) + "-1280x720-gray.png");
	cout << "save grey-resized image to:\t" << path720 << endl;
	cv::imwrite(path720, dst720);

	cv::Mat dst360;
	cv::resize(greyImg, dst360, cv::Size(640, 360), cv::INTER_AREA);
	std::string resizeImgPath(file.substr(0, pos) + "-640x360-gray.png");
	cout << "save grey-resized image to:\t" << resizeImgPath << endl;
	cv::imwrite(resizeImgPath, dst360);
}

int main(int argc, char **argv) {

	cout << "main" << endl;

	//show_binary(argv[1]);

	//cout << "[-] main argv[1]:\t" << argv[1] << endl;
	color2grey(argv[1]);
	//resize_img(argv[1]);

	return 0;
}
