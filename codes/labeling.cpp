//make labeling
//./labeling bolhas.png
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

//saturação => direção da luz;

int main(int argc, char** argv){
  Mat image, save;
  int width, height;
  int nobjects, fundo, furo;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  save = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data||!save.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  // busca objetos presentes
  nobjects=0;
  furo=0;
  //std::cout << (int)image.at<uchar>(0,width-1) << std::endl;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){//############
		// achou um objeto
		//std::cout << image.at<int>(i,j) << std::endl;
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,(nobjects%254));
      }
    }
  }
  fundo=254;
  printf("Tem %d objetos\n", nobjects);

  // eliminando as bolhas da borda de cima
  p.y=0;
  for(int i=0; i<width; i++){
    if(image.at<uchar>(p.y,i) != 0){ //###############
      nobjects--;
      p.x=i;
      floodFill(image,p,0);
    }
  }
  //printf("Agora tem %d objetos\n", nobjects);
  // eliminando as bolhas da borda de baixo
  p.y=height-1;
  for(int i=0; i<width; i++){
    if(image.at<uchar>(p.y,i) != 0){//#############
      nobjects--;
      p.x=i;
      floodFill(image,p,0);
    }
  }
  //printf("Agora tem %d objetos\n", nobjects);
  // eliminando as bolhas da borda da esquerda
  p.x=0;
  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,p.x) != 0){//##########
      nobjects--;
      p.y=i;
      floodFill(image,p,0);
    }
  }
  //printf("Agora tem %d objetos\n", nobjects);
  // eliminando as bolhas da borda da direita
  p.x=width-1;
  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,p.x) != 0){//#############
      nobjects--;
      p.y=i;
      floodFill(image,p,0);
    }
  }
  //printf("Agora tem %d objetos\n", nobjects);

  //achando buracos
  p.x=0;p.y=0;
  floodFill(image,p,fundo);
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){//############
	if(image.at<uchar>(i-1,j) !=fundo){//##########
	  furo++;
	  p.x=j;
	  p.y=i;
	  floodFill(image,p,fundo);
	  p.x--;
	  floodFill(image,p,fundo);
	}
	else{
	  p.x=j;
	  p.y=i;
	  floodFill(image,p,fundo);
	}
      }
    }
  }
  printf("Tem %d bolhas com buracos\n", furo);

  // exibindo
  imshow("Original", save);
  imshow("Processada", image);
  imwrite("labeling.png", image);
  
  waitKey();
  return 0;
}
