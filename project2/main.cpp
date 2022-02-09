#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define T 100000
#define N 50000

using namespace std;
using namespace cv;

int main()
{
	Mat img;						//Это текущций кадр
	Mat img1[60];					// Это 60 кадров для усреднения
	Mat sum_img;					//суммарный кадр для усреднения
	Mat diff;						// кадр для нахождения различий

	int b = 640 * 480;				//Количество пикселей в картинке
	long n, count = 0;				//кол-во всех пикселей и счетчик изменившихся пикселей
	char k[640 * 480];				//массив пикселей для усреднения. только с использованием типа char удается запустить программу без ее вылета
	char sum[640 * 480];			//массив пикселей после усреднения

	VideoCapture cap(0);


	//Пробуем сделать усреднение попиксельно 
	//Усреднение целыми объектами Mat дает ошибку do not match array to array

	                              
	for (b = 0; b < 60; b++)        // Захватываем картинку с камеры, проходя по строкам и столбцам разбиваем ее на пиксели
	{
		cap >> img1[b];

		int i, j;
		for (i = 0; i < img1[1].cols; i++)
		{
			for (j = 0; j < img1[1].rows; j++)
			{
				k[b] = img1[b].at<uchar>(j, i);
				sum[b] = k[b] + sum[b];			     //Суммируем пиксели, находящиеся в одних местах на разных кадрах	

			}
		}

		sum[b] = sum[b] / 60;                       //Усредняем по 60 значениям

		std::cout << sum[b] << "\t\n";

		waitKey(1);

	}

	b = 0;
	
	for (int i1 = 0; i1 < img1[1].cols; i1++)       //Собираем из пикселей усредненную картинку 
	{
		for (int j1 = 0; j1 < img1[1].rows; j1++)
		{
			sum_img.at<uchar>(j1, i1) = sum[b];
			b++;

		}
	}
	

	//imshow("window label", sum_img);
	

	cap >> sum_img;   // Здесь захватываем новую картинку, так как усреднение на удалось

	while (true)
	{
		cap >> img;
		count = 0;
		subtract(sum_img, img, diff);

		//imshow("window label", diff);

		n = diff.total();

		int i, j;
		for (i = 0; i < diff.cols; i++)
		{
			for (j = 0; j < diff.rows; j++)
			{
				int k = diff.at<uchar>(j, i);
				if (k != 0)
				{
					count++;
					//std::cout << "col=" << i << "row=" << j << "k= " << k << "\t\n";
					//std::cout << "cnt=" << count << "\t\n";
					
				}
			}
		}

		imshow("window label", diff);

		if (count > T)
		{

			if ((count - T) > N) std::cout << "cam is closed  " << count << "\t\n";

		}

		std::cout << count;
		std::cout << "///";

		waitKey(100);
	}

	/*while (true)
	{
		cap >> img;
		Mat edges;
		cvtColor(img, edges, CV_BGR2GRAY);
		Canny(edges, edges, 30, 60);
		imshow("window label", img);
		waitKey(1);
	}*/
	return 0;
}
