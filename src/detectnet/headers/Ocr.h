//
// Created by Fabricio Sisalima on 8/1/22.
//
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include "nadjieb/mjpeg_streamer.hpp"
#include <atomic>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#ifndef JETSON_INFERENCE_OCR_H
#define JETSON_INFERENCE_OCR_H

using namespace std;
using namespace cv;

class Ocr {
public:
    void init();
    void end();
    string getText(Mat image);

private:
    tesseract::TessBaseAPI *ocr;
};


#endif //JETSON_INFERENCE_OCR_H
