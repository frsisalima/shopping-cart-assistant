//
// Created by Fabricio Sisalima on 20/1/22.
//
#include <iostream>

#ifndef JETSON_INFERENCE_TTS_H
#define JETSON_INFERENCE_TTS_H


class TTS {
private:
    bool endTTS=false;
    std::string textToSpeak="";
    std::string lastTextToSpeak="";
    int similarTextCont=0;
    int similarTextMaxCont=120;
    void speakToNative(std::string text);
public:
    TTS();
    void speak(std::string text);
    void start();
    void stop();
};


#endif //JETSON_INFERENCE_TTS_H
