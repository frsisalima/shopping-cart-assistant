#include <iostream>
#include "../../utils/commandLine.h"
#include "headers/ConfigFile.h"
#include "headers/JetsonEndpoint.h"
#include "headers/ImageProcessor.h"

ConfigFile loadConfigFile(commandLine cmdLine){
    std::string filenameConfig = "config.json";
    if(cmdLine.GetFlag("config"))
        filenameConfig = cmdLine.GetString("config");

    ConfigFile configFile(filenameConfig);
    configFile.load();
    return  configFile;
}

int main(int argc, char** argv )
{
    commandLine cmdLine(argc, argv, "headless");

    ConfigFile configFile = loadConfigFile(cmdLine);
    ImageProcessor imageProcessor(&configFile,cmdLine);

    JetsonEndpoint jetsonEndpoint(imageProcessor, &configFile);
    jetsonEndpoint.init(2);

   std::thread threadProcess([&imageProcessor](){
        imageProcessor.process();
    });
    std::thread threadApi([&jetsonEndpoint](){
        jetsonEndpoint.start();
    });

    threadApi.join();
    threadProcess.join();
    jetsonEndpoint.stop();

	return 0;
}