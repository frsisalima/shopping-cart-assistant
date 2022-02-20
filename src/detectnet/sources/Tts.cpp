//
// Created by Fabricio Sisalima on 20/1/22.
//

#include <thread>
#include <unistd.h>
#include "../headers/Tts.h"

void TTS::speak(std::string text){
    if(lastTextToSpeak==text){
        similarTextCont=similarTextCont+1;
        if(similarTextCont==similarTextMaxCont){
            this->textToSpeak = text;
            this->lastTextToSpeak = text;
            this->similarTextCont=0;
        }
    }else{
        this->textToSpeak = text;
        this->lastTextToSpeak = text;
        this->similarTextCont=0;
    }
}

void TTS::start() {
    {
        std::thread t([this](){
             while (!endTTS){
                 if(!textToSpeak.empty()){
                     std::cout << "SPEACKING TEXT= " << textToSpeak << "\n";
                     speakToNative(textToSpeak);
                     textToSpeak="";
                 }
                 usleep(10000);
             }
        });
        t.detach();
    }
}

void TTS::stop() {
    endTTS=true;
    //ttsThread.join();
}

void TTS::speakToNative(std::string text) {
    std::string command="python3 /usr/local/bin/tts.py '";
    command.append(text);
    command.append("'");
    FILE* in = popen(command.c_str(), "r");
    pclose(in);
}

TTS::TTS() {

}
