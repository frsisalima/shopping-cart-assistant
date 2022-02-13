//
// Created by Fabricio Sisalima on 7/1/22.
//

#include "../headers/JetsonEndpoint.h"
#include "../headers/ConfigFile.h"

JetsonEndpoint::JetsonEndpoint(ImageProcessor &imageProcessor,ConfigFile* configFile)
        :httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(configFile->getApiPort()))))
{
    this->configFile = configFile;
    this->imageProcessor = imageProcessor;
}

void JetsonEndpoint::init(size_t thr)
{
    auto opts = Pistache::Http::Endpoint::options().threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void JetsonEndpoint::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void JetsonEndpoint::stop()
{
    httpEndpoint->shutdown();
}

void JetsonEndpoint::setupRoutes()
{
    using namespace Pistache::Rest;
    Routes::Post(router, "/config", Routes::bind(&JetsonEndpoint::recordConfig, this));
    Routes::Get(router, "/config", Routes::bind(&JetsonEndpoint::getConfig, this));
}

void JetsonEndpoint::recordConfig(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    configFile->config= json::parse(request.body());
    configFile->save();
    response.send(Pistache::Http::Code::Ok, to_string(configFile->config),MIME(Application, Json));
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));

}

void JetsonEndpoint::getConfig(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    response.send(Pistache::Http::Code::Ok, to_string(configFile->config),MIME(Application, Json));
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
}

