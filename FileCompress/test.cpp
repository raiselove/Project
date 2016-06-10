#define _CRT_SECURE_NO_WARNINGS 1
#include<time.h>
#include <iostream>
using namespace std;

#include "HaffmanTree.h"
#include "Compress.h"
void Test()
{
	double t1;
	double t2;
	FileCompress f;
	//f.Compress("input.txt");
	f.Compress("Input.BIG");
	t1 = clock();
	printf("压缩文件需要的时间: %f s \n", t1 / CLOCKS_PER_SEC);
	//f.UnCompress("input.txt");
	f.UnCompress("Input.BIG");
	t2 = clock();
	printf("解压缩文件需要的时间: %f s \n", t2 / CLOCKS_PER_SEC);
}
int main()
{
	Test();
	system("pause");
	return 0;
}
