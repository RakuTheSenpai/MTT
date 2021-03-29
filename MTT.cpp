#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cout << "usage: MTT.exe <Video_Path>\n";
        return -1;
    }
    VideoCapture cap(argv[1]);
    if(!cap.isOpened()){
        std::cout << "Couldn't open VideoCapture\n";
        return -1;
    }
    namedWindow("MTT", 1);
    while(1){
        Mat frame;
        cap >> frame;
        imshow("MTT", frame);
        if(waitKey(30)>= 0)break;
    }
    return 0;
}