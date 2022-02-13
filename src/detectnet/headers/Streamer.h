//
// Created by Fabricio Sisalima on 2/1/22.
//
#include <chrono>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include "nadjieb/mjpeg_streamer.hpp"
#include <atomic>

#ifndef JETSON_INFERENCE_STREAMER_H
#define JETSON_INFERENCE_STREAMER_H


class Streamer {
public:
    Streamer();
    void start(int port);
    void send(cv::Mat image, std::string text);
    void stop();
    bool isAlive();

private:
    nadjieb::MJPEGStreamer* streamer;
    std::vector<int> params;
};


#endif //JETSON_INFERENCE_STREAMER_H
