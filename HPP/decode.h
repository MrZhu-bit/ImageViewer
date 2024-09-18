#pragma once
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
using cv::Mat;
using cv::Vec3b;
using cv::Vec4b;
using std::vector;
using Path = std::filesystem::path;

class Matcrypt
{
 public:
	Matcrypt();
	~Matcrypt();
	Mat encrypt(const Mat& src);
	Mat decrypt(const Mat& src);
 private:
	void ArrayLoad(int* Array,const int Length,const char* str);
	void EnMatOperation(Mat& src);
	void DeMatOperation(Mat& src);
	void InstallFlag(Mat& src);
	bool CheckFlag(const Mat& src);
 private:
	int* WArray = nullptr;
	int* HArray = nullptr;
	Path path;
	Mat Image;
	int lastW = 0;
	int lastH = 0;
	const vector<Vec3b> RGBFLAG;
	const vector<Vec4b> RGBAFLAG;
};