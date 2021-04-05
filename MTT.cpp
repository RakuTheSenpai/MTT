#include <opencv2/opencv.hpp> 
#include <opencv2/tracking.hpp>
#include <iostream>

void process(cv::Mat &frame, const cv::Ptr<cv::BackgroundSubtractorKNN>&background_sub){
    if(frame.empty()) return;
    cv::Mat masked;
    //Image substraction
    background_sub->apply(frame, masked);
    cv::threshold(masked, masked, 254, 255, cv::THRESH_BINARY);
    std::vector<std::vector<cv::Point>>contours;
    cv::findContours(masked, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point>>convex_hulls(contours.size());
    for(int i = 0; i < contours.size(); ++i) cv::convexHull(contours[i], convex_hulls[i]);
    for(auto &convex_hull:convex_hulls){
        auto rect = cv::boundingRect(convex_hull);
        double diagonal = std::sqrt(std::pow(rect.width, 2) + std::pow(rect.height, 2));
        if(rect.area() > 100 && rect.width > 15 && rect.height > 20 && diagonal > 30){
            cv::rectangle(frame, rect, cv::Scalar(0,255,0), 2);
        }
    }
    cv::imshow("MTT", frame);
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
    auto background_sub = cv::createBackgroundSubtractorKNN(100, 450);
    cv::Ptr<cv::Tracker>tracker;
    tracker = cv::TrackerKCF::create();
    cv::Mat frame;
    while(cap.isOpened()){
        cap >> frame;
        process(frame, background_sub);
        if(cv::waitKey(30)>= 0 || frame.empty())break;
    }
    return 0;
}