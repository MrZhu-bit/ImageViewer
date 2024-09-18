#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
using Path = std::filesystem::path;
int main()
{
	Path encryptFolder = std::filesystem::current_path() / "SecretKey/EncryptKey";
	Path decryptFolder = std::filesystem::current_path() / "SecretKey/DecryptKey";
	Path encryptFile;
	Path decryptFile;
	int minSize;
	int maxSize;
	char ifConfigure;
	cout << "本程序将生成加密图片和解密图片的密钥" << endl
		 << "默认生成的密钥可以加密的图片" << endl
		 << "最小为16像素x16像素" << endl
		 << "最大为1920像素x1920像素" << endl
		 << "按下Y键再按下回车可以进行配置" << endl
		 << "按下其他键再按下回车则按默认配置生成密钥" << endl;
	cin >> ifConfigure;
	if (ifConfigure == 'Y' || ifConfigure == 'y')
	{
		while (true)
		{
			cout << "请输入最小尺寸值:";
			cin >> minSize;
			cout << "请输入最大尺寸值:";
			cin >> maxSize;
			if (minSize > 4 && maxSize > minSize){break;}
			cout << "输入不合法, 最小尺寸值必须大于等于4" << endl
				 << "最大尺寸值必须大于等于最小尺寸值" << endl;
		}
	}
	else{minSize=16;maxSize=1920;}

	for (int i=minSize;i<=maxSize;i++)
	{
		int* encryptArray = new int[i];
		for (int j=0;j<i;j++){encryptArray[j]=j;}

		random_device rd;
		mt19937 gen(rd());
		shuffle(encryptArray,encryptArray+i,gen);

		encryptFile = encryptFolder / ("encryptKey_" + to_string(i) + ".bin");
		ofstream outFileEn(encryptFile,std::ios::binary);
		outFileEn.write(reinterpret_cast<char*>(encryptArray),
			static_cast<std::streamsize>(i*sizeof(int)));
		outFileEn.close();

		int *decryptArray = new int[i];
		for (int j=0;j<i;j++)
		{
			for (int k=0;k<i;k++)
			{
				if (encryptArray[k] == j)
				{
					decryptArray[j] = k;
					break;
				}
			}
		}
		decryptFile = decryptFolder / ("decryptKey_" + to_string(i) + ".bin");
		ofstream outFileDe(decryptFile,std::ios::binary);
		outFileDe.write(reinterpret_cast<char*>(decryptArray),
			static_cast<std::streamsize>(i*sizeof(int)));
		outFileDe.close();

		delete[] encryptArray;
		delete[] decryptArray;
	}
	cout << "密钥生成成功!" << endl;
	cin.get();
	cin.get();
	return 0;
}
