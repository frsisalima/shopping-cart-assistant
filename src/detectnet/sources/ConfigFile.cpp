//
// Created by Fabricio Sisalima on 6/1/22.
//

#include "../headers/ConfigFile.h"

ConfigFile::ConfigFile(std::string path){
    this->path= path;
}

void ConfigFile::load(){
    std::ifstream ifs(path);
    ifs >> config;
    ifs.close();
}

void ConfigFile::save(){
    std::ofstream ost(path);
    ost << std::setw(4) << config << std::endl;
    ost.close();
}

void ConfigFile::setInput(std::string path) {
    config["input"]=path;
}

void ConfigFile::setInputWidth(int width) {
    config["input-width"]=width;
}

void ConfigFile::setInputHeight(int height) {
    config["input-height"]=height;
}

void ConfigFile::setInputCodec(std::string codec) {
    config["input-codec"]=codec;
}

void ConfigFile::setInputLoop(int loop) {
    config["input-loop"]=loop;
}

void ConfigFile::setDetectionNetwork(std::string network) {
    config["network"]=network;
}

void ConfigFile::setDetectionNetworkInputBlob(std::string blob) {
    config["input-blob"]=blob;
}

void ConfigFile::setDetectionNetworkOutputBbox(std::string bbox) {
    config["output-bbox"]=bbox;
}

void ConfigFile::setDetectionNetworkOutputCvg(std::string cvg) {
    config["output-cvg"]=cvg;
}

void ConfigFile::setDetectionLabels(std::string labels) {
    config["labels"]=labels;
}

void ConfigFile::setSegmentationNetwork(std::string network) {
    config["seg-network"]=network;
}

void ConfigFile::setStreamPort(int port) {
    config["stream-port"]=port;
}

void ConfigFile::setApiPort(int port) {
    config["api-port"]=port;
}

std::string ConfigFile::getInput() {
    return config["input"].get<std::string>();
}

int ConfigFile::getInputWidth() {
    return config["input-width"];
}

int ConfigFile::getInputHeight() {
    return config["input-height"];
}

std::string ConfigFile::getInputCodec() {
    return config["input-codec"].get<std::string>();
}

int ConfigFile::getInputLoop() {
    return config["input-loop"];
}

std::string ConfigFile::getDetectionNetwork() {
    return config["network"].get<std::string>();
}

std::string ConfigFile::getDetectionNetworkInputBlob() {
    return config["input-blob"].get<std::string>();
}

std::string ConfigFile::getDetectionNetworkOutputBbox() {
    return config["output-bbox"].get<std::string>();
}

std::string ConfigFile::getDetectionNetworkOutputCvg() {
    return config["output-cvg"].get<std::string>();
}

std::string ConfigFile::getDetectionLabels() {
    return config["labels"].get<std::string>();
}

std::string ConfigFile::getSegmentationNetwork() {
    return config["seg-network"].get<std::string>();
}

int ConfigFile::getStreamPort() {
    return config["stream-port"];
}

int ConfigFile::getApiPort() {
    return config["api-port"];
}

ConfigFile::ConfigFile() {

}

void ConfigFile::setEnableDraw(bool enable) {
    config["enable-draw"]=enable;
}

bool ConfigFile::getEnableDraw() {
    return config["enable-draw"];
}

void ConfigFile::setEnableProcessInfo(bool enable) {
    config["enable-process-info"]=enable;
}

bool ConfigFile::getEnableProcessInfo() {
    return config["enable-process-info"];
}

void ConfigFile::setEnableDetection(bool enable) {
    config["enable-detection"]=enable;
}

bool ConfigFile::getEnableDetection() {
    return config["enable-detection"];
}

void ConfigFile::setEnableSegmentation(bool enable) {
    config["enable-segmentation"]=enable;
}

bool ConfigFile::getEnableSegmentation() {
    return config["enable-segmentation"];
}

void ConfigFile::setEnableStreaming(bool enable) {
    config["enable-streaming"]=enable;
}

bool ConfigFile::getEnableStreaming() {
    return config["enable-streaming"];
}

void ConfigFile::setEnableTTS(bool enable) {
    config["enable-tts"]=enable;
}

bool ConfigFile::getEnableTTS() {
    return config["enable-tts"];
}

void ConfigFile::setImageDisplayType(std::string type) {
    config["image-display-type"] = type;
}

std::string ConfigFile::getImageDisplayType() {
    return  config["image-display-type"];
}

void ConfigFile::setMsgObstacle(std::string msg) {
    config["msg-obstacle"]=msg;
}

std::string ConfigFile::getMsgObstacle() {
    return config["msg-obstacle"];
}

void ConfigFile::setMsgPerson(std::string msg) {
    config["msg-person"]=msg;
}

std::string ConfigFile::getMsgPerson() {
    return config["msg-person"];
}

void ConfigFile::setMsgSign(std::string msg) {
    config["msg-sign"]=msg;
}

std::string ConfigFile::getMsgSign() {
    return config["msg-sign"];
}

void ConfigFile::setSignTexts(std::string texts) {
    config["sign-texts"]=texts;
}

std::string ConfigFile::getSignTexts() {
    return config["sign-texts"];
}

void ConfigFile::setEnableBeepAlert(bool enable) {
    config["enable-beep-alert"]=enable;
}

bool ConfigFile::getEnableBeepAlert() {
    return config["enable-beep-alert"];
}


