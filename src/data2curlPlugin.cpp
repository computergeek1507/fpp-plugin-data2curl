
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <istream>
#include <ostream>

#include <iostream> 

#include <thread> 

#include <vector>

#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <thread>
#include <cmath>

#include <httpserver.hpp>
#include "common.h"
#include "settings.h"
#include "Plugin.h"
#include "Plugins.h"
#include "log.h"
#include "channeloutput/channeloutput.h"
#include "MultiSync.h"

#include "fppversion_defines.h"

#include "commands/Commands.h"

#include "CURLItem.h"

class Data2CURLPlugin : public FPPPlugin, public httpserver::http_resource {
private:
    std::vector<std::unique_ptr <CURLItem>> _CURLOutputs;
    Json::Value config;

public:

    Data2CURLPlugin() : FPPPlugin("fpp-plugin-data2curl") {
        LogInfo(VB_PLUGIN, "Initializing Data2CURL Plugin\n");
        readFiles();
    }
    virtual ~Data2CURLPlugin() 
    {
        _CURLOutputs.clear();
    }

    virtual const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req) override {
        std::string v = getTopics();
        return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(v, 200));
    }
    
#if FPP_MAJOR_VERSION < 4 || FPP_MINOR_VERSION < 1
    virtual void modifyChannelData(int ms, uint8_t *seqData) override {
#else
    virtual void modifySequenceData(int ms, uint8_t *seqData) override {
#endif
        try
        {
            sendChannelData(seqData);
        }
        catch(std::exception ex)
        {
            std::cout << ex.what();
        }
    }

    virtual void playlistCallback(const Json::Value &playlist, const std::string &action, const std::string &section, int item) {
        if (settings["Start"] == "PlaylistStart" && action == "start") {
            EnableCURLItems();
        }  
    }

    void EnableCURLItems() {
        for(auto & output: _CURLOutputs)
        {
            output->EnableOutput();
        }
    }
    

    void sendChannelData(unsigned char *data) {
        for(auto & output: _CURLOutputs)
        {
            output->SendData(data);
        }
    }
    
    void saveDataToFile()
    {
        std::ofstream outfile;
        outfile.open ("/home/fpp/media/config/fpp-plugin-data2curl");

        if(_CURLOutputs.size() ==0) {
            outfile <<  "nooutputsfound;;;1";
            outfile <<  "\n";
        }

        for(auto & out: _CURLOutputs) {
            outfile << out->GetIPAddress();
            outfile <<  ";";
            outfile << out->GetURL();
            outfile <<  ";";
            outfile << out->GetMessage();
            outfile <<  ";";
            outfile << out->GetStartChannel();
            outfile <<  "\n";
        }
        outfile.close();
    }


    void readFiles()
    {
        //read topic, payload and start channel settings from JSON setting file. 
        if (LoadJsonFromFile("/home/fpp/media/config/plugin.data2curl.json", config)) {
            for (int i = 0; i < config.size(); i++) {
                std::string const ip = config[i]["ip"].asString();
                std::string const url = config[i]["url"].asString();
                std::string const message = config[i]["message"].asString();
                unsigned int sc =  config[i]["startchannel"].asInt();
                if(!ip.empty()) {
                    std::unique_ptr<CURLItem> mqttItem = std::make_unique<CURLItem>(ip,url,message,sc);
                    _CURLOutputs.push_back(std::move(mqttItem));
                }
            }
        }
        saveDataToFile();
    }
    
    std::string getTopics()
    {
        std::string topics;
        for(auto & out: _CURLOutputs) {
            topics += out->GetIPAddress();
            topics += ",";
        }
        return topics;
    } 
};


extern "C" {
    FPPPlugin *createPlugin() {
        return new Data2CURLPlugin();
    }
}
