#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
using namespace cv;

int main(){
	Mat src = imread("test.jpg",0);
	int cols = src.cols;
	int rows = src.rows;
	int n = cols*rows;
	int col_nums[256] = { 0 };
	uchar trans_col[256] = { 0 };
	// 统计直方图
	for (int i = 0; i < rows; i++){
		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j < cols; j++){
			col_nums[p[j]] ++;
		}
	}
	// 计算转换函数
	trans_col[0] = 255 * col_nums[0] / n;
	for (int i = 1; i < 256; i++){
		col_nums[i] = col_nums[i - 1] + col_nums[i];
		trans_col[i] = 255 * col_nums[i] / n;
	}
	Mat res;
	res.create(src.size(), src.type());
	for (int i = 0; i < rows; i++){
		uchar* s = src.ptr<uchar>(i);
		uchar* r = res.ptr<uchar>(i);
		for (int j = 0; j < cols; j++){
			r[j] = trans_col[s[j]];
		}
	}
	imshow("src", src);
	imshow("res", res);
	waitKey();
}