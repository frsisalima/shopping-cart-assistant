//
// Created by Fabricio Sisalima on 6/1/22.
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <json.hpp>

#ifndef JETSON_INFERENCE_CONFIG_FILE_H
#define JETSON_INFERENCE_CONFIG_FILE_H


class ConfigFile {
private:
    std::string path;
public:
    ConfigFile();
    ConfigFile(std::string path);

    nlohmann::json config;

    void load();
    void save();

    //setters
    void setInput(std::string path);
    void setInputWidth(int width);
    void setInputHeight(int height);
    void setInputCodec(std::string codec);
    void setInputLoop(int loop);
    void setDetectionNetwork(std::string network);
    void setDetectionNetworkInputBlob(std::string blob);
    void setDetectionNetworkOutputBbox(std::string bbox);
    void setDetectionNetworkOutputCvg(std::string cvg);
    void setDetectionLabels(std::string labels);
    void setSegmentationNetwork(std::string network);
    void setSegmentationClasses(std::string network);
    void setSegmentationColors(std::string network);
    void setStreamPort(int port);
    void setApiPort(int port);
    void setEnableDraw(bool enable);
    void setEnableProcessInfo(bool enable);
    void setEnableDetection(bool enable);
    void setEnableSegmentation(bool enable);
    void setEnableStreaming(bool enable);
    void setEnableTTS(bool enable);
    void setEnableBeepAlert(bool enable);
    void setImageDisplayType(std::string type);
    void setMsgObstacle(std::string msg);
    void setMsgPerson(std::string msg);
    void setMsgSign(std::string msg);
    void setSignTexts(std::string texts);

    //getters
    std::string getInput();
    int getInputWidth();
    int getInputHeight();
    std::string getInputCodec();
    int getInputLoop();
    std::string getDetectionNetwork();
    std::string getDetectionNetworkInputBlob();
    std::string getDetectionNetworkOutputBbox();
    std::string getDetectionNetworkOutputCvg();
    std::string getDetectionLabels();
    std::string getSegmentationNetwork();
    std::string getSegmentationClasses();
    std::string getSegmentationColors();
    int getStreamPort();
    int getApiPort();
    bool getEnableDraw();
    bool getEnableProcessInfo();
    bool getEnableDetection();
    bool getEnableSegmentation();
    bool getEnableStreaming();
    bool getEnableTTS();
    bool getEnableBeepAlert();
    std::string getImageDisplayType();
    std::string getMsgObstacle();
    std::string getMsgPerson();
    std::string getMsgSign();
    std::string getSignTexts();
};

#endif //JETSON_INFERENCE_CONFIG_FILE_H
