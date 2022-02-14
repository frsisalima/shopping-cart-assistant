//
// Created by Fabricio Sisalima on 7/1/22.
//
#include <algorithm>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <string>
#include <mutex>
#include <vector>
#include <memory>
#include "jetson-utils/json.hpp"
#include "ConfigFile.h"
#include "ImageProcessor.h"

#ifndef JETSON_INFERENCE_JETSON_ENDPOINT_H
#define JETSON_INFERENCE_JETSON_ENDPOINT_H

using namespace nlohmann;

class JetsonEndpoint
{
public:
    JetsonEndpoint();
    JetsonEndpoint(ImageProcessor* imageProcessor, ConfigFile* configFile);

    void init(size_t thr = 2);
    void start();
    void stop();

private:
    void setupRoutes();
    void recordConfig(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void getConfig(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
    Pistache::Rest::Router router;
    ConfigFile* configFile;
    ImageProcessor* imageProcessor;
};


#endif //JETSON_INFERENCE_JETSON_ENDPOINT_H
