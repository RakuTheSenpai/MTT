#include <opencv2/opencv.hpp> 
#include <iostream>

void show_process(cv::Mat &frame, const cv::Ptr<cv::BackgroundSubtractorKNN>&background_sub){
    if(frame.empty()) return;
    cv::Mat masked;
    //Resize frame to half
    //Image substraction
    background_sub->apply(frame, masked);
    cv::imshow("First", masked);
    cv::threshold(masked, masked, 254, 255, cv::THRESH_BINARY);
    //Contour finding
    std::vector<std::vector<cv::Point>>contours;
    std::vector<cv::Vec4i>hierarchy;
    cv::findContours(masked, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    //Draw Contours
    for( size_t i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( 0, 255, 0 );
        int area = cv::contourArea(contours[i]);
        if(area > 100){
            auto bound = cv::boundingRect(contours[i]);
            cv::rectangle(frame,bound,color);
        }
    }
    cv::imshow("Threshold", masked);
    cv::imshow("Frame",frame);
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
        cv::Mat frame;
        cap >> frame;
        show_process(frame, background_sub);
        if(cv::waitKey(30)>= 0 || frame.empty())break;
    }
    return 0;
}