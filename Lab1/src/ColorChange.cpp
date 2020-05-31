/**
	@file ColorChange.cpp
	@brief Source file for processing of image.
	@author Di Nardo Di Maio Raffaele
*/

#include "ColorChange.h"
#include <iostream>

void changeOnClickRGB(Mat img, int x, int y)
{
	Vec3i color = { 0,0,0 };
	square_mean(img,x,y,color);
	Mat new_img = img.clone();

	float treshold = 0.67;

	for (int i = 0; i < new_img.rows; i++)
	{
		for (int j = 0; j < new_img.cols; j++)
		{
			int color0_check1 = (img.at<Vec3b>(i, j)[0] <= static_cast<int>((1 + treshold) * color[0]));
			int color0_check2 = (img.at<Vec3b>(i, j)[0] >= static_cast<int>((1 - treshold) * color[0]));
			int color1_check1 = (img.at<Vec3b>(i, j)[1] <= static_cast<int>((1 + treshold) * color[1]));
			int color1_check2 = (img.at<Vec3b>(i, j)[1] >= static_cast<int>((1 - treshold) * color[1]));
			int color2_check1 = (img.at<Vec3b>(i, j)[2] <= static_cast<int>((1 + treshold) * color[2]));
			int color2_check2 = (img.at<Vec3b>(i, j)[2] >= static_cast<int>((1 - treshold) * color[2]));

			int check = color0_check1 && color0_check2 && color1_check1 && color1_check2 && color2_check1 && color2_check2;

			if (check)
			{
				new_img.at<Vec3b>(i, j)[0]= 201;
				new_img.at<Vec3b>(i, j)[1]=37;
				new_img.at<Vec3b>(i, j)[2]= 92;
			}
		}
	}

	imshow("RGB", new_img);
}

void square_mean(Mat img, int x, int y, Vec3i &color)
{
	/*
	*	from__________
	*     |           |
	*	  |           |
	*     |			  |
	*     |___________to
	*/

	int from_x = (x - GAP);
	int from_y = (y - GAP);
	from_x = (from_x < 0) ? 0 : from_x;
	from_y = (from_y < 0) ? 0 : from_y;

	int to_x = (x + GAP);
	int to_y = (y + GAP);
	to_x = (to_x >= img.cols) ? img.cols - 1 : to_x;
	to_y = (to_y >= img.rows) ? img.rows - 1 : to_y;

	int num_elements = (to_x - from_x + 1) * (to_y - from_y + 1);

	for (int i = from_y; i <= to_y; i++)
	{
		for (int j = from_x; j <= to_x; j++)
		{
			color[0] += img.at<Vec3b>(i, j)[0];
			color[1] += img.at<Vec3b>(i, j)[1];
			color[2] += img.at<Vec3b>(i, j)[2];
		}
	}

	color[0] = color[0] / num_elements;
	color[1] = color[1] / num_elements;
	color[2] = color[2] / num_elements;
}

void changeOnClickHSV(Mat img, int x, int y)
{
	Mat new_img = img.clone();
	cvtColor(new_img, new_img, COLOR_BGR2HSV);
	
	Vec3i color = Vec3i(0, 0, 0);
	square_mean(new_img, x, y, color);

	for (int i = 0; i < new_img.rows; i++)
	{
		for (int j = 0; j < new_img.cols; j++)
		{
			int color0_check1 = (new_img.at<Vec3b>(i, j)[0] <= static_cast<int>((1 + 0.3) * color[0]));
			int color0_check2 = (new_img.at<Vec3b>(i, j)[0] >= static_cast<int>((1 - 0.3) * color[0]));

			int check = color0_check1 && color0_check2;

			if (check)
			{
				new_img.at<Vec3b>(i, j)[0] += 30;
			}
		}
	}
	
	cvtColor(new_img, new_img, COLOR_HSV2BGR);
	imshow("HSV", new_img);
}