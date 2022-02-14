//
// Created by Fabricio Sisalima on 2/1/22.
//

#include "../headers/Streamer.h"

void Streamer::start(int port){
    params = {cv::IMWRITE_JPEG_QUALITY, 90};
    std::cout << " Streamer::start port=" << port<< "\n";
    streamer->start(port);

    std::cout << " Streamer::start ok" << "\n";

}

void Streamer::send(cv::Mat image,std::string text){
    if(streamer->isAlive()){
        rectangle( image, cv::Point( 0, 0), cv::Point( image.cols, 40), cv::Scalar( 0, 0, 0 ),cv::FILLED,cv::LINE_8 );
        putText(image, text, cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1,cv::Scalar(0, 255, 0), 2);
        std::vector<uchar> buff_hsv;
        cv::cvtColor( image, image, cv::COLOR_BGR2RGB);
        imencode(".jpg", image, buff_hsv, params);
        streamer->publish("/hsv.mp4", std::string(buff_hsv.begin(), buff_hsv.end()));
    }
}

void Streamer::stop(){
    streamer->stop();
}

bool Streamer::isAlive(){
    streamer->isAlive();
}

Streamer::Streamer() {
    streamer= new nadjieb::MJPEGStreamer();
}
