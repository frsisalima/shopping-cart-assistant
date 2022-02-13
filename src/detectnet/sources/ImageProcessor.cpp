//
// Created by Fabricio Sisalima on 12/2/22.
//

#include <regex>
#include "../headers/ImageProcessor.h"

ImageProcessor::ImageProcessor(ConfigFile* configFile, commandLine &cmdLine) {
    this->configFile=configFile;
    this->cmdLine=cmdLine;
    ocr= new Ocr();
    ocr->init();
    tts= new TTS();
    tts->start();
    streamer = new Streamer();
}

void ImageProcessor::process() {
    initInput();
    initNetworks();
    streamer->start(configFile->getStreamPort());
    const uint32_t overlayFlags =  detectNet::OverlayFlags::OVERLAY_NONE;

    while(enableProcess ) {
        uchar3* image = NULL;
        if( !input->Capture(&image, 2000)){
            if( !input->IsStreaming() ){
                initInput();
            }
            LogError("detectnet:  failed to capture video frame\n");
            continue;
        }
        auto start = std::chrono::system_clock::now();

        cv::Mat cv_image(cv::Size(input->GetWidth(), input->GetHeight()), CV_8UC3, image);

        detectNet::Detection* detections = NULL;
        if(configFile->getEnableDetection() && net!= NULL) {
            const int numDetections = net->Detect(image, input->GetWidth(), input->GetHeight(), &detections, overlayFlags);
            objectDetection(cv_image, detections, numDetections, net);
        }

        bool obstacles = false;
        if(configFile->getEnableSegmentation() && segmentNet!=NULL){
           processSegmentation(image,input,"overlay|mask");
            cv::Mat cv_image_seg_mask(cv::Size(input->GetWidth(), input->GetHeight()), CV_8UC3, imgMask);
            if(configFile->getImageDisplayType()=="mask"){
                cv_image= cv_image_seg_mask;
            }else{
                if(configFile->getImageDisplayType()=="overlay"){
                    cv_image=Mat(cv::Size(input->GetWidth(), input->GetHeight()), CV_8UC3, imgOverlay);
                }
            }

            if(secureArea.width==0 && secureArea.height==0){
                Point p1= Point((cv_image_seg_mask.cols/2)-125,cv_image_seg_mask.rows-200);
                Point p4= Point((cv_image_seg_mask.cols/2)-250,cv_image_seg_mask.rows-200);
                secureArea =Rect(p1.x,p1.y,250,200);
                secureAreaPerson =Rect(p4.x,p4.y,500,200);
            }
            obstacles = detectObstacle(cv_image_seg_mask, secureArea);
            if(obstacles){
                countObstacles = countObstacles + 1;
                if(countObstacles > framesLimiteToDetermineObstacle){
                    countObstacles = 0;
                    if(configFile->getEnableBeepAlert()){
                        system("(speaker-test -t sine -f 450)& pid=$!; sleep 0.2s; kill -9 $pid");
                    }else{
                        if(configFile->getEnableTTS()){
                            tts->speak(configFile->getMsgObstacle());
                        }
                    }

                }
            }else{
                countObstacles = 0;
            }
        }

        if(configFile->getEnableStreaming()){
            drawPolygon(cv_image,obstacles);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;
            string text="";

            if(configFile->getEnableProcessInfo()){
                text = status + " || "+"CPU="+ std::to_string(cvRound(1.0 / diff.count()))+"FPS" ;
                if(configFile->getEnableDetection() && net!=NULL){
                    text.append(" DETNET="+ std::to_string(cvRound(net->GetNetworkFPS())) +"FPS" );
                }
                if(configFile->getEnableSegmentation() && segmentNet!=NULL){
                    text.append(" SEGNET="+ std::to_string(cvRound(segmentNet->GetNetworkFPS()))+"FPS" );
                }
            }
            streamer->send(cv_image,text);
        }
    }
    streamer->stop();
    tts->stop();
    SAFE_DELETE(input);
    SAFE_DELETE(net);
    SAFE_DELETE(segmentNet);
    LogVerbose("detectnet:  shutdown complete.\n");
}

bool ImageProcessor::allocBuffers( int width, int height, uint32_t flags )
{
    // check if the buffers were already allocated for this size
    //if( imgOverlay != NULL && width == overlaySize.x && height == overlaySize.y )
    if(( imgMask != NULL && width == maskSize.x && height == maskSize.y )
       || ( imgOverlay != NULL && width == overlaySize.x && height == overlaySize.y )
            )
        return true;

    // free previous buffers if they exit
    CUDA_FREE_HOST(imgMask);
    CUDA_FREE_HOST(imgOverlay);
    CUDA_FREE_HOST(imgComposite);

    // allocate overlay image
    overlaySize = make_int2(width, height);

    if( flags & segNet::VISUALIZE_OVERLAY )
    {
        if( !cudaAllocMapped(&imgOverlay, overlaySize) )
        {
            LogError("segnet:  failed to allocate CUDA memory for overlay image (%ux%u)\n", width, height);
            return false;
        }
        outputSize = overlaySize;
    }

    // allocate mask image (half the size, unless it's the only output)
    if( flags & segNet::VISUALIZE_MASK )
    {
        // maskSize = (flags & segNet::VISUALIZE_OVERLAY) ? make_int2(width/2, height/2) : overlaySize;
        maskSize = overlaySize;
        if( !cudaAllocMapped(&imgMask, maskSize) )
        {
            LogError("segnet:  failed to allocate CUDA memory for mask image\n");
            return false;
        }

        outputSize = maskSize;
    }

    // allocate composite image if both overlay and mask are used
    if( (flags & segNet::VISUALIZE_OVERLAY) && (flags & segNet::VISUALIZE_MASK) )
    {
        compositeSize = make_int2(overlaySize.x + maskSize.x, overlaySize.y);

        if( !cudaAllocMapped(&imgComposite, compositeSize) )
        {
            LogError("segnet:  failed to allocate CUDA memory for composite image\n");
            return false;
        }
        outputSize = compositeSize;
    }

    return true;
}

void ImageProcessor::processSegmentation(uchar3* imgInput,videoSource* input, const char* visualizationFlag){
    const segNet::FilterMode filterMode = segNet::FILTER_LINEAR;
    const uint32_t visualizationFlags = segNet::VisualizationFlagsFromStr(visualizationFlag);
    const char* ignoreClass =  "void";

    allocBuffers(input->GetWidth(), input->GetHeight(), visualizationFlags);
    segmentNet->Process(imgInput, input->GetWidth(), input->GetHeight(), ignoreClass);
    if( visualizationFlags & segNet::VISUALIZE_OVERLAY )
        segmentNet->Overlay(imgOverlay, overlaySize.x, overlaySize.y, filterMode);
    if( visualizationFlags & segNet::VISUALIZE_MASK )
        segmentNet->Mask(imgMask, maskSize.x, maskSize.y, filterMode);
}

void ImageProcessor::drawPolygon(Mat &image, bool obstacles){
    if(!configFile->getEnableDraw() || !configFile->getEnableSegmentation()) return;
    if(secureArea.x ==0 && secureArea.y == 0) return;
    Point p1= Point(secureArea.x,secureArea.y);
    Point p2= Point(secureArea.x+secureArea.width,secureArea.y);
    Point p3= Point(secureAreaPerson.x+secureAreaPerson.width,secureAreaPerson.y+secureAreaPerson.height);
    Point p4= Point(secureAreaPerson.x,secureAreaPerson.y+secureAreaPerson.height);
    int lineType = LINE_8;

    vector< Point> contour;
    contour.push_back(p1);
    contour.push_back(p2);
    contour.push_back(p3);
    contour.push_back(p4);

    Mat contours = Mat(contour);
    const Point *pts = (const cv::Point*) contours.data;
    int npts = contours.rows;
    polylines(image, &pts, &npts, 1, true,obstacles?Scalar(0, 0, 255):Scalar(0, 255, 0),8,lineType);
}

bool ImageProcessor::detectObstacle(Mat image,Rect rect){
    Mat roi = image(rect);
    Mat src_gray;
    int thresh = 3;
    Mat canny_output;
    vector<vector<Point> > contours;
    cvtColor( roi, src_gray, COLOR_BGR2GRAY );
    Canny( src_gray, canny_output, thresh, thresh*2 );
    findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    int obstacles = 0;
    for( size_t i = 0; i < contours.size(); i++ ){
        approxPolyDP( contours[i], contours_poly[i], 50, true );
        boundRect[i] = cv::boundingRect( contours_poly[i] );
        if(boundRect[i].height>15 && boundRect[i].width>15 ){
            obstacles=obstacles+1;
        }
    }
    return  obstacles > 0;
}

void ImageProcessor::processOcr(Mat &image, Rect rect){
    if(rect.width<0 ||rect.height<0 || rect.x+rect.width>image.cols || rect.y+rect.height>image.rows)return;
    Mat imgSign = image(rect);
    cv::cvtColor( imgSign, imgSign, CV_BGR2GRAY );
    imgSign = imgSign > 200;
    bitwise_not(imgSign, imgSign);
    string textOcr= ocr->getText(imgSign);
    if(textOcr.length()>3 && configFile->getSignTexts().find(textOcr)!= string::npos){
        if(configFile->getEnableTTS() && textOcr!="" && textOcr!=" "){
            tts->speak( std::regex_replace(configFile->getMsgSign(),std::regex("\\$name"),textOcr));
        }
    }

    if(configFile->getEnableDraw()){
        cv::putText(image, textOcr, cv::Point(rect.x, rect.y-10), cv::FONT_HERSHEY_SIMPLEX, 1,cv::Scalar(0, 255, 0), 2);
    }
    // cv::rectangle(image, rect, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
    // cv::cvtColor( imgSign, imgSign, CV_GRAY2BGR );
    // imgSign.copyTo(image(cv::Rect(0,0,imgSign.cols, imgSign.rows)));
}

void ImageProcessor::objectDetection(Mat &image, detectNet::Detection* detections, int numDetections, detectNet* net) {
    int nearPerson =0;
    for( int n=0; n < numDetections; n++ )
    {
        const int boxWidth = detections[n].Right - detections[n].Left;
        const int boxHeight =  detections[n].Bottom -  detections[n].Top;
        Rect rect = cv::Rect(detections[n].Left, detections[n].Top, boxWidth, boxHeight);

        std::string classDetected =  net->GetClassDesc(detections[n].ClassID);
        if(classDetected == "Person" || classDetected == "person" || classDetected == "-m-01g317"){
            Scalar color = cv::Scalar(50, 255, 0);
            if(rect.height > 600 && rect.width  >300){
                nearPerson= nearPerson+ 1;
                color = cv::Scalar(0, 0, 255);
            }
            if(configFile->getEnableDraw()){
                rectangle(image, rect, color, 5, cv::LINE_AA);
            }
        }
        if(classDetected == "sign"){
            if(configFile->getEnableDraw()){
                rectangle(image, rect, cv::Scalar(50, 200, 50), 5, cv::LINE_AA);
            }
            processOcr(image,rect);
        }
    }
    if(nearPerson>0){
        countObstaclesPerson = countObstaclesPerson + 1;
        if(countObstaclesPerson > framesLimiteToDetermineObstaclePerson){
            countObstaclesPerson = 0;
            if(configFile->getEnableTTS()){
                tts->speak(configFile->getMsgPerson());
            }
        }
    }else{
        countObstaclesPerson = 0;
    }
}

void ImageProcessor::initNetworks() {
    std::thread tInit([this](){
        status ="INITIALIZING NETWORK DETECTION";
        cmdLine.AddArg(("--network="+ configFile->getDetectionNetwork()).c_str());
        cmdLine.AddArg(("--labels="+ configFile->getDetectionLabels()).c_str());
        cmdLine.AddArg(("--input-blob="+ configFile->getDetectionNetworkInputBlob()).c_str());
        cmdLine.AddArg(("--output-cvg="+ configFile->getDetectionNetworkOutputCvg()).c_str());
        cmdLine.AddArg(("--output-bbox="+ configFile->getDetectionNetworkOutputBbox()).c_str());

        net = detectNet::Create(cmdLine);
        status ="INITIALIZING NETWORK SEGMENTATION";
        segmentNet = segNet::Create(segNet::NetworkTypeFromStr(configFile->getSegmentationNetwork().c_str()));
        segmentNet->SetOverlayAlpha(150.0f);
        status ="PROCESSING";
    });
    tInit.detach();
}

void ImageProcessor::initInput() {
    cmdLine.AddArg(("--input="+configFile->getInput()).c_str());
    cmdLine.AddArg(("--input-codec="+ configFile->getInputCodec()).c_str());
    cmdLine.AddArg(("--input-height="+ to_string(configFile->getInputHeight())).c_str());
    cmdLine.AddArg(("--input-width="+ to_string(configFile->getInputWidth())).c_str());
    cmdLine.AddArg(("--input-loop="+ to_string(configFile->getInputLoop())).c_str());
    input = videoSource::Create(cmdLine, ARG_POSITION(0));
}

ImageProcessor::ImageProcessor() {

}

void ImageProcessor::stopProcess() {
    enableProcess =false;
}

