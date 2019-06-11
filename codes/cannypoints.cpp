#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int step_slider = 2, step_slider_max = 10;
int jitter_slider = 3, jitter_slider_max = 20;
int raio_slider = 2, raio_slider_max = 5;
int raio_edge = 1, raio_edge_max = 5;
int lowTH_slider = 200, lowTH_slider_max = 200;
int highTH_slider = 600, highTH_slider_max = 600;
char step_Track[10], jitter_Track[10], raio_Track[10];
char raioEdge_Track[10], lowTH_Track[10], highTH_Track[10];

Mat image, points, border, borderpoints, fim;

vector<int> yrange, xrange;

int width, height, gray;
int x, y;

void on_trackbar_pontos(int, void*) {
	Canny(image, border, lowTH_slider, highTH_slider);
	if (step_slider < 2) step_slider = 1;
	if (jitter_slider < 2) jitter_slider = 1;
	if (raio_slider < 2) raio_slider = 1;
	xrange.resize(height / step_slider);
	yrange.resize(width / step_slider);

	iota(xrange.begin(), xrange.end(), 0);
	iota(yrange.begin(), yrange.end(), 0);

	for (uint i = 0; i < xrange.size(); i++) {
		xrange[i] = xrange[i] * step_slider + step_slider / 2;
	}

	for (uint i = 0; i < yrange.size(); i++) {
		yrange[i] = yrange[i] * step_slider + step_slider / 2;
	}

	points = Mat(height, width, CV_8U, Scalar(255));

	random_shuffle(xrange.begin(), xrange.end());
	for (int i = 0; i < xrange.size(); i++) {
		random_shuffle(yrange.begin(), yrange.end());
		for (int j = 0; j < yrange.size(); j++) {
			x = xrange[i] + rand() % (2 * jitter_slider) - jitter_slider + 1;
			y = yrange[j] + rand() % (2 * jitter_slider) - jitter_slider + 1;
			if (x >= height) x = height - 1;
			if (y >= width) y = width - 1;
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			gray = image.at<uchar>(x, y);
			circle(points, cv::Point(y, x), raio_slider, CV_RGB(gray, gray, gray), -1, CV_AA);
		}
	}

	borderpoints = Mat(height, width, CV_8U, Scalar(0));

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			if (border.at<uchar>(x, y) == 255) {
				//if (25 > (rand() % 100)) {
				gray = image.at<uchar>(x, y);
				circle(borderpoints, cv::Point(y, x), raio_edge, CV_RGB(gray, gray, gray), -1, CV_AA);
				//}
			}
		}
	}
	imshow("Teste", borderpoints | points);
}

int main(int argc, char** argv) {


	image = imread("nissan.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	srand(time(0));

	if (!image.data) {
		cout << "nao abriu nissan.jpg" << endl;
		exit(0);
	}

	width = image.size().width;
	height = image.size().height;

	sprintf_s(lowTH_Track, "LowTH", lowTH_slider);
	sprintf_s(highTH_Track, "highTH", highTH_slider);
	sprintf_s(step_Track, "stepTH", step_slider);
	sprintf_s(jitter_Track, "jitterTH", jitter_slider);
	sprintf_s(raio_Track, "raioTH", raio_slider);
	sprintf_s(raio_Track, "raioTH", raio_edge);

	createTrackbar(lowTH_Track, "Canny", &lowTH_slider, lowTH_slider_max,
		on_trackbar_pontos);
	createTrackbar(highTH_Track, "Canny", &highTH_slider, highTH_slider_max,
		on_trackbar_pontos);
	createTrackbar(step_Track, "Canny", &step_slider, step_slider_max,
		on_trackbar_pontos);
	createTrackbar(jitter_Track, "Canny", &jitter_slider, jitter_slider_max,
		on_trackbar_pontos);
	createTrackbar(raio_Track, "Canny", &raio_slider, raio_slider_max,
		on_trackbar_pontos);
	createTrackbar(raioEdge_Track, "Canny", &raio_edge, raio_edge_max,
		on_trackbar_pontos);

	on_trackbar_pontos(lowTH_slider, 0);
	on_trackbar_pontos(highTH_slider, 0);
	on_trackbar_pontos(step_slider, 0);
	on_trackbar_pontos(jitter_slider, 0);
	on_trackbar_pontos(raio_slider, 0);
	on_trackbar_pontos(raio_edge, 0);

	namedWindow("Canny", 1);

	createTrackbar(lowTH_Track, "Canny", &lowTH_slider, lowTH_slider_max, on_trackbar_pontos);
	createTrackbar(highTH_Track, "Canny", &highTH_slider, highTH_slider_max, on_trackbar_pontos);
	createTrackbar(step_Track, "Canny", &step_slider, step_slider_max, on_trackbar_pontos);
	createTrackbar(jitter_Track, "Canny", &jitter_slider, jitter_slider_max, on_trackbar_pontos);
	createTrackbar(raio_Track, "Canny", &raio_slider, raio_slider_max, on_trackbar_pontos);
	createTrackbar(raioEdge_Track, "Canny", &raio_edge, raio_edge_max, on_trackbar_pontos);

	imwrite("canny.png", borderpoints);
	imwrite("points.png", points);
	imwrite("cannypoints.png", borderpoints | points);

	waitKey(0);
	return 0;
}
