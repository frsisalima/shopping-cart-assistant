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
   // std::thread ttsThread;
    void speakToNative(std::string text);
public:
    TTS();
    void speak(std::string text);
    void start();
    void stop();
};


#endif //JETSON_INFERENCE_TTS_H
