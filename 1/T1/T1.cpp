#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <queue>

using namespace cv;
using namespace std;

void grises(Mat in, Mat out);
void desplegar_gris(Mat in, Mat out);
void img_binaria(Mat in, Mat out, float val);
void dilatacion(Mat in, Mat out, int v);
void conteo(Mat in);
void clasificacio_figura(Mat in, int num_fig);
int video();

void erosion(Mat in, Mat out, int v);

int main()
{
	Mat image = imread("C:/Users/gumev/Downloads/3.jpg ");

	cout << "canales:" << image.channels() << " filas: " << image.rows << " columnas: " << image.cols << endl;
	imshow("Original", image);
	Mat auxg(image.rows, image.cols, CV_32FC1);
	Mat auxg1(image.rows, image.cols, CV_32FC1);
	Mat auxg2(image.rows, image.cols, CV_32FC1);
	Mat auxu(image.rows, image.cols, CV_32FC1);
	Mat g_sal(image.rows, image.cols, CV_8UC1);

	grises(image, auxg);
	img_binaria(auxg, auxu, 50.0);
	desplegar_gris(auxu, g_sal);
	dilatacion(auxu, auxg2, 2);
	//imshow("binaria + dilatacion", auxg2);

	conteo(auxg2);
	desplegar_gris(auxg2, g_sal);
	imshow("etiquetado", g_sal);

	waitKey(0);
	return 0;
	//video();
}

void grises(Mat in, Mat out)
{
	Mat aux(in.rows, in.cols, CV_32FC3);
	in.convertTo(aux, CV_32FC3);
	for (int i = 0; i < in.rows; i++)
		for (int j = 0; j < in.cols; j++)
		{
			Vec3f pixel = aux.at<Vec3f>(i, j);
			out.at<float>(i, j) = (pixel[0] + pixel[1] + pixel[2]) / 3.0;
		}
}

void img_binaria(Mat in, Mat out, float val)
{
	for (int i = 0; i < in.rows; i++)
		for (int j = 0; j < in.cols; j++)
		{
			if (in.at<float>(i, j) <= val)
				out.at<float>(i, j) = 0.0;
			else
				out.at<float>(i, j) = 255.0;
		}
}

void conteo(Mat in)
{
	queue<Vec2i> vecc;
	int con = 0;
	for (int i = 1; i < in.rows - 1; i++)
		for (int j = 1; j < in.cols - 1; j++)
		{
			if (in.at<float>(i, j) == 255.0)
			{
				vecc.push(Vec2i(i, j));
				con = con + 1;
				in.at<float>(i, j) = con * 10.0;
				while (!vecc.empty())
				{
					Vec2i a = vecc.front();
					vecc.pop();
					for (int ii = a[0] - 1; ii < a[0] + 2; ii++)
						for (int jj = a[1] - 1; jj < a[1] + 2; jj++)
						{
							if (in.at<float>(ii, jj) == 255.0)
							{
								vecc.push(Vec2i(ii, jj));
								in.at<float>(ii, jj) = con * 10.0;
							}
						}
				}
			}
		}
	cout << "Número de objetos: " << con << endl;

	clasificacio_figura(in, con);


}

void clasificacio_figura(Mat in, int num_fig) {

	float** uwu = new float* [num_fig];
	for (int i = 0; i < num_fig; i++) {
		uwu[i] = new float[2];
	}

	for (int i = 0; i < num_fig; i++) {
		for (int j = 0; j < 2; j++) {
			uwu[i][j] = 0;
		}
	}
	for (int i = 0; i < num_fig; i++)
		uwu[i][0] = i;


	int a = 1;

	for (int i = 0; i < in.rows; i++) {
		for (int j = 0; j < in.cols; j++)
		{
			if (in.at<float>(i, j) > 0) {
				a = (in.at<float>(i, j) / 10.0) - 1;
				uwu[a][1] = uwu[a][1] + 1;
			}
		}
	}

	for (int i = 0; i < num_fig; i++) {
		for (int j = 0; j < 2; j++) {
			cout << uwu[i][j] << " ";
		}
		cout << endl;
	}








}

int video()
{
	VideoCapture vi(0); //elegir camara
	if (!vi.isOpened())
		return -1;
	while (1)
	{
		Mat img;
		vi >> img;
		imshow("img_video", img);
		if (waitKey(10) == 27)
		{
			imwrite("img1.jpg", img); //guardar imagen en la carpeta dle codifo
			break;
		}
	}
	return 0;
}

void dilatacion(Mat in, Mat out, int v)
{
	for (int i = 0; i < in.rows; i++)
		for (int j = 0; j < in.cols; j++)
			out.at<float>(i, j) = in.at<float>(i, j);

	for (int vv = 0; vv < v; vv++)
	{
		for (int i = 1; i < in.rows - 1; i++)
			for (int j = 1; j < in.cols - 1; j++)
			{
				if (out.at<float>(i, j) > out.at<float>(i, j - 1))
					out.at<float>(i, j - 1) = out.at<float>(i, j);
			}

		for (int i = 1; i < in.rows - 1; i++)
			for (int j = in.cols - 2; j > 1; j--)
			{
				if (out.at<float>(i, j) > out.at<float>(i, j + 1))
					out.at<float>(i, j + 1) = out.at<float>(i, j);
			}

		for (int i = 1; i < in.rows - 1; i++)
			for (int j = 1; j < in.cols - 1; j++)
			{
				if (out.at<float>(i, j) > out.at<float>(i - 1, j))
					out.at<float>(i - 1, j) = out.at<float>(i, j);
			}

		for (int i = in.rows - 2; i > 1; i--)
			for (int j = 1; j < in.cols - 1; j++)
			{
				if (out.at<float>(i, j) > out.at<float>(i + 1, j))
					out.at<float>(i + 1, j) = out.at<float>(i, j);
			}

	}
}

void desplegar_gris(Mat in, Mat out)
{
	in.convertTo(out, CV_8UC1);
}
