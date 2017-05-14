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
	uchar * src_pic = new uchar[n];
	int * lap_pic = new int[n];
	uchar * scale_pic = new uchar[n];
	int * res_pic = new int[n];
	int dets[9] = { -cols - 1, -cols, -cols + 1, -1, 0, +1, +cols - 1, +cols, +1 };
	int masks[9] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			src_pic[i*cols + j] = src.ptr<uchar>(i)[j];
		}
	}
	// laplace 掩模计算
	int min = 2500,max = -2500;
	for (int i = 0; i < n; i++){
			lap_pic[i] = 0;
			for (int k = 0; k < 9; k++){
				int g = i + dets[k];
				if (g >= 0 && g <= n){
					 lap_pic[i] += masks[k] * src_pic[g];
				}
			}
			max = lap_pic[i] > max ? lap_pic[i] : max;
			min = lap_pic[i] < min ? lap_pic[i] : min;
	}
	// 对laplace变换进行标定
	for (int i = 0; i < n; i++){
		scale_pic[i] = (uchar)(255 * (lap_pic[i] - min) / (max - min));
	}
	Mat mat;
	mat.create(src.size(), src.type());
	for (int i = 0; i<rows; i++)
	{
		uchar * p = mat.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int l = i*cols + j;
			p[j] = scale_pic[l];
		}
	}
	// 将 laplace 掩模加载到新的图像上
	for (int i = 0; i < n; i++){
		res_pic[i] = src_pic[i] - lap_pic[i];
		if (res_pic[i]<0)
		{
			res_pic[i] = 0;
		}
		else if (res_pic[i]>255)
		{
			res_pic[i] = 255;
		}
	}
	Mat res;
	res.create(src.size(), src.type());
	for (int i = 0; i<rows; i++)
	{
		uchar * p = res.ptr<uchar>(i);
		for (int j = 0; j<cols;j++)
		{
			int l = i*cols + j;
			p[j] = res_pic[l];
		}
	}
	imshow("src", src);
	imshow("Laplace", mat);
	imshow("res", res);
	waitKey();
}