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
	cout << "���������ɼ���ͼƬ�ͽ���ͼƬ����Կ" << endl
		 << "Ĭ�����ɵ���Կ���Լ��ܵ�ͼƬ" << endl
		 << "��СΪ16����x16����" << endl
		 << "���Ϊ1920����x1920����" << endl
		 << "����Y���ٰ��»س����Խ�������" << endl
		 << "�����������ٰ��»س���Ĭ������������Կ" << endl;
	cin >> ifConfigure;
	if (ifConfigure == 'Y' || ifConfigure == 'y')
	{
		while (true)
		{
			cout << "��������С�ߴ�ֵ:";
			cin >> minSize;
			cout << "���������ߴ�ֵ:";
			cin >> maxSize;
			if (minSize > 4 && maxSize > minSize){break;}
			cout << "���벻�Ϸ�, ��С�ߴ�ֵ������ڵ���4" << endl
				 << "���ߴ�ֵ������ڵ�����С�ߴ�ֵ" << endl;
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
	cout << "��Կ���ɳɹ�!" << endl;
	cin.get();
	cin.get();
	return 0;
}
