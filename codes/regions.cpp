//	make regions
//	./regions nome_imagem xmin ymin xmax ymax
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char** argv){
  Mat image;
  int xmin, ymin, xmax, ymax;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  xmin = atoi(argv[2]);
  ymin = atoi(argv[3]);
  xmax = atoi(argv[4]);
  ymax = atoi(argv[5]);

  if(xmin < 0 || xmax > image.size().width || ymin < 0 || ymax > image.size().height || xmax < xmin || ymax < ymin){
    std::cout << "Parâmetros inválidos, escolha pontos dentro da largura da imagem\n";
    return 0;
  }

  for(int i=xmin; i<xmax; i++){
    for(int j=ymin; j<ymax; j++){
      image.at<uchar>(i,j)=(unsigned char)(256-(int)image.at<uchar>(i,j));
    }
  }

  imshow("Processada", image);
  imwrite("negativo.png", image);
  
  waitKey();
  return 0;
}
