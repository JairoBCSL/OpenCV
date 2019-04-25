//	make trocaregioes
//	./trocaregioes nome_da_imagem
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, save;
  int largura, altura;
  CvPoint p;
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  save = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data||!save.data){
    cout << "Não carregou\n";
  }
  largura=image.size().width;
  altura=image.size().height;

  p.x=0;
  p.y=0;
  
  for(int i=0; i<altura; i++){
    for(int j=0; j<largura; j++){
      image.at<uchar>(i,j) = save.at<uchar>((i+altura/2)%altura,(j+largura/2)%largura);
    }
  }

  imshow("Original", save);  
  imshow("Processada", image);
  imwrite("trocada.png", image);
  waitKey();
  return 0;
}
