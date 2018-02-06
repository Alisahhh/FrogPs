#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include<vector>
#include <ctime>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double calcLength(double x1,double y1,double x2,double y2){
  return (y2-y1)*(y2-y1)+(x2-x1)*(x2-x1);
}

void drawDetectLines(Mat& image,const vector<Vec4i>& lines,const Scalar & color,double &x,double &y){
   vector<Vec4i>::const_iterator it=lines.begin();
   double mx=0;
   while(it!=lines.end()){
      double x1=(*it)[0],y1=(*it)[1],x2=(*it)[2],y2=(*it)[3];
       Point pt1((*it)[0],(*it)[1]);
       Point pt2((*it)[2],(*it)[3]);
       //line(image,pt1,pt2,color,1);
       ++it;
       if(calcLength(x1,y1,x2,y2)>mx){
         mx=calcLength(x1,y1,x2,y2);
         if(x1>x2){
           swap(x1,x2);
           swap(y1,y2);
         }
         x=x1,y=y1;
       }
   }
   //imshow("image",image);
   cout<<"OK"<<endl;
}

int calc(double &x,double &y,Mat image,double lineSize){
  cout<<lineSize<<endl;
  Mat contours;
  Canny(image,contours,125,350);
  threshold(contours,contours,128,255,THRESH_BINARY);
  //imshow("canny",contours);
  vector<Vec4i>lines;
  HoughLinesP(contours,lines,1,CV_PI/180,10,lineSize,10);
  if(lines.size()<=1) return -1;
  drawDetectLines(image,lines,Scalar(0,255,0),x,y);
  //imshow("Lines",image);

  waitKey(0);
  //我觉得可以单独开一个文件写这个函数...这才是重点？
  //if can't find return -1;
  return 1;
}

Mat select(){
  srand(time(0));
  Mat frog[4];
  frog[0]=imread("./picture/frog1.jpg");
  if(!frog[0].data){
    printf("-1");
    exit;
  }
  frog[1]=imread("./picture/frog2.jpg");
  frog[2]=imread("./picture/frog3.jpg");
  frog[3]=imread("./picture/frog4.jpg");
  int tmp=std::rand()%4;
  //imshow("frog",frog[0]);
  return frog[tmp];
}

int main(int argc, char** argv )
{
    if ( argc != 2 ){
      printf("usage: DisplayImage.out <Image_Path>\n");
      return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    //imshow("image",image);
    if ( !image.data ){
      printf("No image data \n");
      return -1;
    }
    //Mat imageTmp;
    //resize(image,imageTmp,Size(300,300),0,0);
    //imshow("imageTmp",imageTmp);
    //此处应该加一个对读入图片处理的过程，比如...放到指定的大小以方便和青蛙配对
    //我选择回来再
    int minSize=min(image.rows,image.cols);
    Mat frog=select();
    Mat frogMask;
    Mat frogTmp;
    minSize=minSize/5;
    resize(frog,frogTmp,Size(minSize,minSize));
    cvtColor(frogTmp, frogMask, CV_BGR2GRAY);
    //imshow("frogTmp",frogTmp);
    double x,y;
    int ifPlain=calc(x,y,image,minSize);
    cout<<x<<" "<<y<<endl;
    if(ifPlain==-1){
      std::cout<<"You frog can't take photo there"<<std::endl;
      return 0;
    }
    Mat ROI = image(cv::Rect(x,y,frogMask.cols,frogMask.rows));
    frogTmp.copyTo(ROI,frogMask);
    imshow("Newfrog",image);
    imwrite("Newfrog.jpg",image);
    waitKey(0);
    return 0;
    //waitKey(0);
}
