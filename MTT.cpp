#include <opencv2/opencv.hpp> 
#include <iostream>

void show_process(cv::Mat &frame, const cv::Ptr<cv::BackgroundSubtractorKNN>&background_sub){
    if(frame.empty()) return;
    cv::Mat gray, thresh;
    int height = frame.size().height;
    int width = frame.size(). width;
    //Create black empty Matrix
    cv::Mat complete{cv::Size{width, height}, frame.type()};
    //Resize frame to half
    int nwidth = width/2;
    int nheight = height/2;
    cv::resize(frame, frame, cv::Size{nwidth, nheight});
    //Color gray then turn back to BGR channel to permit copy (cv doesn't allow images with different channels to be copied)
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    //Perform threshold
    cv::threshold(gray, thresh, 127, 255, cv::THRESH_BINARY_INV);
    //Image substraction
    background_sub->apply(thresh, thresh);
    cv::cvtColor(thresh, thresh, cv::COLOR_GRAY2BGR);
    //Copy images into new Mat 
    cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);
    frame.copyTo(complete(cv::Rect(0,0,nwidth, nheight)));
    gray.copyTo(complete(cv::Rect(nwidth, 0, nwidth, nheight)));
    thresh.copyTo(complete(cv::Rect(0, nheight, nwidth, nheight)));
    cv::imshow("Frame", complete);
}
int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cout << "usage: MTT.exe <Video_Path>\n";
        return -1;
    }
    cv::VideoCapture cap(argv[1]);
    if(!cap.isOpened()){
        std::cout << "Couldn't open VideoCapture\n";
        return -1;
    }
    auto background_sub = cv::createBackgroundSubtractorKNN();
    while(1){
        cv::Mat frame, gray, thresh;
        cap >> frame;
        show_process(frame, background_sub);
        if(cv::waitKey(30)>= 0)break;
    }
    return 0;
}