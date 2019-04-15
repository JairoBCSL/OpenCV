//make equalize
//./equalize imagem.extensão
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, sv;
  vector<Mat> planes, svPlanes;
  Mat histR, histG, histB;
  Mat histRs, histGs, histBs;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgRs(histh, histw, CV_8UC3, Scalar(0,0,0));//#
  Mat histImgGs(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgBs(histh, histw, CV_8UC3, Scalar(0,0,0));

    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    split (image, planes);
    image.copyTo(sv);
    split (sv, svPlanes);

    equalizeHist(svPlanes[0], svPlanes[0]);//#
    equalizeHist(svPlanes[1], svPlanes[1]);
    equalizeHist(svPlanes[2], svPlanes[2]);

    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&svPlanes[0], 1, 0, Mat(), histRs, 1,//#
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&svPlanes[1], 1, 0, Mat(), histGs, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&svPlanes[2], 1, 0, Mat(), histBs, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());
    normalize(histRs, histRs, 0, histImgRs.rows, NORM_MINMAX, -1, Mat());//#
    normalize(histGs, histGs, 0, histImgGs.rows, NORM_MINMAX, -1, Mat());
    normalize(histBs, histBs, 0, histImgBs.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));
    histImgRs.setTo(Scalar(0));
    histImgGs.setTo(Scalar(0));
    histImgBs.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh), Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh), Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh), Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
      line(histImgRs, Point(i, histh), Point(i, histh-cvRound(histRs.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgGs, Point(i, histh), Point(i, histh-cvRound(histGs.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgBs, Point(i, histh), Point(i, histh-cvRound(histBs.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }

    merge(svPlanes, sv);

    histImgR.copyTo(image(Rect(0, 0      , nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh  , nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh, nbins, histh)));
    histImgRs.copyTo(sv(Rect(0, 0      , nbins, histh)));
    histImgGs.copyTo(sv(Rect(0, histh  , nbins, histh)));
    histImgBs.copyTo(sv(Rect(0, 2*histh, nbins, histh)));
    imshow("image", image);
    imshow("sv", sv);
    imwrite("imagem.png", sv);
    if(waitKey(-1));

  return 0;
}
