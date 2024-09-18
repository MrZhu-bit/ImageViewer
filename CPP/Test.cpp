#include "../HPP/Test.h"
#include <iostream>
using std::cout;
using std::endl;
void Test(const cv::Mat& src, const char* str)
{
	cout << "____________________\n";
	cout << str << endl;
// 检查图像是否有效
	if (src.empty())
	{
		std::cerr << "Error: Image is empty." << std::endl;
		return;
	}

// 获取图像的尺寸
	int W = src.cols;
	int H = src.rows;

// 检查图像是否至少有一个像素
	if (W <= 0 || H <= 0)
	{
		std::cerr << "Error: Invalid image dimensions." << std::endl;
		return;
	}

// 根据图像的通道数选择合适的类型
	if (src.channels() == 3)
	{
// 对于 RGB 图像 (3 通道)
		std::vector<cv::Vec3b> corners(4);
		corners[0] = src.at<cv::Vec3b>(0, 0);            // 左上角
		corners[1] = src.at<cv::Vec3b>(0, W - 1);        // 右上角
		corners[2] = src.at<cv::Vec3b>(H - 1, 0);        // 左下角
		corners[3] = src.at<cv::Vec3b>(H - 1, W - 1);    // 右下角

		for (int i = 0; i < 4; i++)
		{
			std::cout << "Corner " << i << ": ";
			for (int j = 0; j < 3; j++)
			{
				std::cout << static_cast<int>(corners[i][j]) << " ";
			}
			std::cout << std::endl;
		}
	}
	else if (src.channels() == 4)
	{
// 对于 RGBA 图像 (4 通道)
		std::vector<cv::Vec4b> corners(4);
		corners[0] = src.at<cv::Vec4b>(0, 0);            // 左上角
		corners[1] = src.at<cv::Vec4b>(0, W - 1);        // 右上角
		corners[2] = src.at<cv::Vec4b>(H - 1, 0);        // 左下角
		corners[3] = src.at<cv::Vec4b>(H - 1, W - 1);    // 右下角

		for (int i = 0; i < 4; i++)
		{
			std::cout << "Corner " << i << ": ";
			for (int j = 0; j < 4; j++)
			{
				std::cout << static_cast<int>(corners[i][j]) << " ";
			}
			std::cout << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Unsupported image format." << std::endl;
	}

}