#include <stdio.h>
#include<cstdlib>
#include<ctime>
#include <opencv2/opencv.hpp>

using namespace cv;

int calc(int *x,int *y,Mat image){

  //我觉得可以单独开一个文件写这个函数...这才是重点？
  //if can't find return -1;
}

Mat select(){
  srand(time(0));
  Mat frog[4];
  frog[0]=imread("./picture/frog1.jpg");
  frog[1]=imread("./picture/frog2.jpg");
  frog[2]=imread("./picture/frog3.jpg");
  frog[3]=imread("./picture/frog4.jpg");
  int tmp=std::rand()%4;
  //imshow("frog",frog[1]);
  return frog[tmp];
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    imshow("image",image);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    //此处应该加一个对读入图片处理的过程，比如...放到指定的大小以方便和青蛙配对
    //我选择回来再写
    Mat frog=select();
    Mat frogMask;
    cvtColor(frog, frogMask, CV_BGR2GRAY);
    int x,y;
    bool ifPlain=calc(&x,&y,image);
    if(ifPlain==-1){
      std::cout<<"You frog can't take photo there"<<std::endl;
      return 0;
    }
    Mat ROI = image(cv::Rect(x,y,frogMask.cols,frogMask.rows));
    frog.copyTo(ROI,frogMask);
    imshow("Newfrog",image);
    imwrite("Newfrog.jpg",image);
    waitKey(0);
    return 0;
}
