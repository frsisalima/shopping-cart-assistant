//
// Created by Fabricio Sisalima on 8/1/22.
//

#include <regex>
#include "../headers/Ocr.h"

void Ocr::init()
{
    ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "spa", tesseract::OEM_LSTM_ONLY);
    ocr->SetPageSegMode(tesseract::PSM_AUTO);
    ocr->SetVariable("tessedit_char_whitelist","ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    ocr->SetVariable("debug_file", "NUL");
}

void Ocr::end(){
    ocr->End();
}

string Ocr::getText(Mat image){
    ocr->SetImage(image.data, image.cols, image.rows, 1, image.step);
    string text=string(ocr->GetUTF8Text());
    string temp = "";
    for (int i = 0; i < text.size(); ++i) {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')|| (text[i] >= '0' && text[i] <= '9')|| (text[i] == ' ')) {
            temp = temp + text[i];
        }
    }
    text = temp;
    return  text;
}