#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <conio.h>

using namespace cv;
using namespace std;

double gi = 0.0, gs = 1, c = 1, d0 = 2;
char key;
Mat imagem, filtro;
vector<Mat> planos;
void troca(Mat& imagem);
void menu();

int main(int argc, char** argv) {
	Mat original, imagemRe, imagemIm, imagemComp;
	Mat filtroComp;
	int largura, altura;



	original = imread("nissan.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	largura = getOptimalDFTSize(original.cols);
	altura = getOptimalDFTSize(original.rows);
	copyMakeBorder(original, imagem, 0, largura - original.cols, 0, altura - original.rows, BORDER_CONSTANT, Scalar(0, 0, 0));

	imagemIm = Mat_<float>::zeros(imagem.size());
	imagemComp = Mat(imagem.size(), CV_32FC2, Scalar(0));
	filtroComp = imagemComp.clone();
	filtro = Mat(altura, largura, CV_32F);

	for (;;) {
		for (int i = 0; i < largura; i++) {
			for (int j = 0; j < altura; j++) {
				filtro.at<float>(Point(i, j)) = (gs - gi) * (1 - exp(-c * (((i - largura / 2) * (i - largura / 2) + (j - altura / 2) * (j - altura / 2)) / (d0 * d0)))) + gi;
			}
		}

		Mat comps[] = { filtro, filtro };
		merge(comps, 2, filtroComp);

		planos.clear();
		imagemRe = Mat_<float>(imagem);
		planos.push_back(imagemRe);
		planos.push_back(imagemIm);
		merge(planos, imagemComp);
		dft(imagemComp, imagemComp);
		troca(imagemComp);
		mulSpectrums(imagemComp, filtroComp, imagemComp, 0);
		planos.clear();
		troca(imagemComp);
		idft(imagemComp, imagemComp);
		planos.clear();
		split(imagemComp, planos);
		normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

		//imshow("Ori", original);
		//waitKey();
		//imshow("Ima", planos[0]);
		//waitKey();
		//imshow("Fil", filtro);
		imshow("Original", imagem);
		imshow("Imagem", planos[0]);
		imshow("Filtro", filtro);
		menu();
	}

	return 0;
}

void troca(Mat& imagem) {
	Mat tmp, A, B, C, D;

	imagem = imagem(Rect(0, 0, imagem.cols & -2, imagem.rows & -2));
	int cx = imagem.cols / 2, cy = imagem.rows / 2;
	A = imagem(Rect(0, 0, cx, cy));
	B = imagem(Rect(cx, 0, cx, cy));
	C = imagem(Rect(0, cy, cx, cy));
	D = imagem(Rect(cx, cy, cx, cy));

	A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);
	C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);

}

void menu() {
	key = waitKey();
	switch (key) {
	case 'q':
		gi += 0.1;
		cout << "Gi: " << gi << endl;
		break;
	case 'a':
		gi -= 0.1;
		cout << "Gi: " << gi << endl;
		break;
	case 'w':
		gs += 0.1;
		cout << "Gs: " << gs << endl;
		break;
	case 's':
		gs -= 0.1;
		cout << "Gs: " << gs << endl;
		break;
	case 'e':
		c += 0.1;
		cout << "C:  " << c << endl;
		break;
	case 'd':
		c -= 0.1;
		cout << "C:  " << c << endl;
		break;
	case 'r':
		d0 += 0.1;
		cout << "D0: " << d0 << endl;
		break;
	case 'f':
		d0 -= 0.1;
		cout << "D0: " << d0 << endl;
		break;
	case 't':
		imwrite("originalHmorfico.png", imagem);
		imwrite("hmorfico.png", planos[0]*255);
		imwrite("filtroHmorfico.png", filtro*255);
		break;
	default:
		break;

	}
}
