#include "../HPP/decode.h"
#include "../HPP/Test.h"
#include <filesystem>
#include <vector>
#include <fstream>
#include <thread>
using std::thread;
using std::ifstream;
using std::to_string;
Matcrypt::Matcrypt():path(),Image(),
RGBFLAG{Vec3b(1,2,3),Vec3b(4,5,6),Vec3b(7,8,9),Vec3b(10,11,12)},
RGBAFLAG{Vec4b(1,2,3,0),Vec4b(4,5,6,0),Vec4b(7,8,9,0),Vec4b(10,11,12,0)}
{}
Mat Matcrypt::encrypt(const Mat& src)
{
	if (CheckFlag(src)){return Mat();}
	if (lastW == src.cols && lastH == src.cols){}
	else
	{
		lastW = src.cols;
		lastH = src.rows;

		WArray = new int[lastW];
		HArray = new int[lastH];
		ArrayLoad(WArray,lastW,"EncryptKey/encryptKey_");
		ArrayLoad(HArray,lastH,"EncryptKey/encryptKey_");
	}

	vector<Mat> mv;
	split(src,mv);

	if (src.channels() == 1)
	{
		EnMatOperation(mv[0]);
	}
	else if (src.channels() == 3 || src.channels() == 4)
	{
		thread mvt0([&mv,this](){EnMatOperation(mv[0]);});
		thread mvt1([&mv,this](){EnMatOperation(mv[1]);});
		thread mvt2([&mv,this](){EnMatOperation(mv[2]);});
		mvt0.join();
		mvt1.join();
		mvt2.join();
	}
	Mat dst;
	merge(mv,dst);

	InstallFlag(dst);

	return dst;
}
Mat Matcrypt::decrypt(const Mat& src)
{
	if (!CheckFlag(src)){return Mat();}
	if (lastW == src.cols && lastH == src.cols){}
	else
	{
		lastW = src.cols;
		lastH = src.rows;

		WArray = new int[lastW];
		HArray = new int[lastH];

		ArrayLoad(WArray,lastW,"DecryptKey/decryptKey_");
		ArrayLoad(HArray,lastH,"DecryptKey/decryptKey_");
	}

	vector<Mat> mv;
	split(src,mv);

	if (src.channels() == 1)
	{
		EnMatOperation(mv[0]);
	}
	else if (src.channels() == 3 || src.channels() == 4)
	{
		thread mvt0([&mv,this](){EnMatOperation(mv[0]);});
		thread mvt1([&mv,this](){EnMatOperation(mv[1]);});
		thread mvt2([&mv,this](){EnMatOperation(mv[2]);});
		mvt0.join();
		mvt1.join();
		mvt2.join();
	}
	Mat dst;
	merge(mv,dst);

	return dst;
}
void Matcrypt::ArrayLoad(int* Array,const int Length,const char* str)
{
	path = std::filesystem::current_path() / "SecretKey" / (str + to_string(Length) + ".bin");
	ifstream file(path.string(),std::ios::binary);
	file.read(reinterpret_cast<char*>(Array),
		static_cast<std::streamsize>(Length*sizeof(int)));
	file.close();
}
Matcrypt::~Matcrypt()
{
	delete[] WArray;
	delete[] HArray;
}
void Matcrypt::EnMatOperation(Mat& src)
{
	Mat temp_1 = src.clone();
	for (int i=0;i<src.cols;i++)
	{
		temp_1.col(WArray[i]).copyTo(src.col(i));
	}

	Mat temp_2 = src.clone();
	for (int i=0;i<src.rows;i++)
	{
		temp_2.row(HArray[i]).copyTo(src.row(i));
	}
}
void Matcrypt::DeMatOperation(Mat& src)
{
	Mat temp_1 = src.clone();
	for (int i=0;i<src.rows;i++)
	{
		temp_1.row(HArray[i]).copyTo(src.row(i));
	}

	Mat temp_2 = src.clone();
	for (int i=0;i<src.cols;i++)
	{
		temp_2.col(WArray[i]).copyTo(src.col(i));
	}
}
void Matcrypt::InstallFlag(Mat& src)
{
	int W = src.cols;
	int H = src.rows;
	if (src.channels() == 1)
	{
		for (int i=0;i<std::min(12,W);i++)
		{
			src.at<uchar>(0,i) = uchar(i);
		}
	}
	else if (src.channels() == 3)
	{
		src.at<Vec3b>(0,0)     = RGBFLAG[0];
		src.at<Vec3b>(0,W-1)   = RGBFLAG[1];
		src.at<Vec3b>(H-1,0)   = RGBFLAG[2];
		src.at<Vec3b>(H-1,W-1) = RGBFLAG[3];
	}
	else if (src.channels() == 4)
	{
		src.at<Vec4b>(0,0) 	 = RGBAFLAG[0];
		src.at<Vec4b>(0,W-1)   = RGBAFLAG[1];
		src.at<Vec4b>(H-1,0) 	 = RGBAFLAG[2];
		src.at<Vec4b>(H-1,W-1) = RGBAFLAG[3];
	}
}
bool Matcrypt::CheckFlag(const Mat& src)
{
	int W = src.cols;
	int H = src.rows;

	if (src.channels() == 1)
	{
		for (int i=0;i<std::min(12,W);i++)
		{
			if (src.at<uchar>(0,i) == uchar(i)){}
			else {return false;}
		}
		return true;
	}
	else if (src.channels() == 3)
	{
		if (src.at<Vec3b>(0,0)     == RGBFLAG[0]&&
			src.at<Vec3b>(0,W-1)   == RGBFLAG[1]&&
			src.at<Vec3b>(H-1,0)   == RGBFLAG[2]&&
			src.at<Vec3b>(H-1,W-1) == RGBFLAG[3])
		{return true;}
		else{return false;}
	}
	else if (src.channels() == 4)
	{
		if (src.at<Vec4b>(0,0)     == RGBAFLAG[0]&&
			src.at<Vec4b>(0,W-1)   == RGBAFLAG[1]&&
			src.at<Vec4b>(H-1,0)   == RGBAFLAG[2]&&
			src.at<Vec4b>(H-1,W-1) == RGBAFLAG[3])
		{return true;}
		else{return false;}
	}
	return false;
}




