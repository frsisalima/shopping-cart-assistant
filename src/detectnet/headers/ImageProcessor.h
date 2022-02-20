//
// Created by Fabricio Sisalima on 12/2/22.
//

#ifndef JETSON_INFERENCE_IMAGEPROCESSOR_H
#define JETSON_INFERENCE_IMAGEPROCESSOR_H

#include <csignal>
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <nadjieb/mjpeg_streamer.hpp>
#include <atomic>
#include <json.hpp>
#include <utility>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <string>
#include <regex>

#include "videoSource.h"
#include "segNet.h"
#include "cudaMappedMemory.h"
#include "detectNet.h"

#include "Ocr.h"
#include "Tts.h"
#include "ConfigFile.h"
#include "Streamer.h"

using namespace std;
using namespace cv;
using namespace nlohmann;

class ImageProcessor {
private:
    Ocr* ocr;
    TTS* tts;
    Streamer* streamer;
    ConfigFile* configFile;
    typedef uchar3 pixelType;		// this can be uchar3, uchar4, float3, float4

    uint32_t overlayFlagsDetection =  detectNet::OverlayFlags::OVERLAY_NONE;

    pixelType* imgMask      = NULL;	// color of each segmentation class
    pixelType* imgOverlay   = NULL;	// input + alpha-blended mask
    pixelType* imgComposite = NULL;	// overlay with mask next to it

    int2 maskSize;
    int2 overlaySize;
    int2 compositeSize;
    int2 outputSize;

    segNet* segmentNet=NULL;
    detectNet* net = NULL;
    videoSource* input= NULL;

    Rect secureArea =Rect(0,0,0,0);
    Rect secureAreaPerson =Rect(0,0,0,0);

    int countObstacles = 0;
    int framesLimiteToDetermineObstacle = 3;

    int countObstaclesPerson = 0;
    int framesLimiteToDetermineObstaclePerson = 10;
    bool enableProcess=true;
    string  status ="INITIALIZING";
    vector<std::string> signsTexts;

    commandLine cmdLine;

    bool allocBuffers( int width, int height, uint32_t flags );
    void processSegmentation(uchar3* imgInput,videoSource* input, const char* visualizationFlag);
    void drawPolygon(Mat &image, bool obstacles);
    bool detectObstacle(Mat image,Rect rect);
    void processOcr(Mat &image, Rect rect);
    string getSingTextAvailable(string text);
    void objectDetection(Mat &image, detectNet::Detection* detections, int numDetections, detectNet* net);

public:
    ImageProcessor();
    ImageProcessor(ConfigFile* configFile, commandLine &cmdLine);
    void process();
    void stopProcess();
    void initNetworks();
    void initInput();
};


#endif //JETSON_INFERENCE_IMAGEPROCESSOR_H
