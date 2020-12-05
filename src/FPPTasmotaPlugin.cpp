
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <istream>
#include <ostream>

#include <iostream> 

#include <filesystem>

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
#include "mqtt.h"
#include "MultiSync.h"

#include "fppversion_defines.h"

#include "commands/Commands.h"

#include "TasmotaBulb.h"

class FPPTasmotaPlugin : public FPPPlugin, public httpserver::http_resource {
private:
    std::vector<std::unique_ptr <TasmotaBulb>> _tasmotaOutputs;
    Json::Value config;

public:

    FPPTasmotaPlugin() : FPPPlugin("fpp-tasmota-plugin") {
        printf ("FPPTasmotaPlugin Starting\n");
        sendConfigFileNow();
    }
    virtual ~FPPTasmotaPlugin() 
    {
         _tasmotaOutputs.clear();
    }

    virtual const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req) override {
        std::string v = getIPs();
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
            sendBulbsOn();
        }  
    }
    
    void sendBulbsOn() {
        for(auto & output: _tasmotaOutputs)
        {
            printf ("Sending BulbOn %s\n" ,output->GetIPAddress().c_str());
            output->BulbOn();
        }
    }

    void sendChannelData(unsigned char *data) {
        for(auto & output: _tasmotaOutputs)
        {
            printf ("Sending Data %s\n" ,output->GetIPAddress().c_str());
            output->SendData(data);
        }
    }
    
    void saveDataToFile()
    {
	//Save obj setting to text file for PHP gui
        std::ofstream outfile;
        outfile.open ("/home/fpp/media/config/fpp-tasmota-plugin");
        
        for(auto & out: _zcppOutputs)
        {
            outfile << out->GetIPAddress();
            outfile <<  ",";
            outfile << out->GetStartChannel();
            outfile <<  ",";
            outfile << out->GetChannelCount();
            outfile <<  "\n";
        }
        outfile.close();
    }


    void readFiles()
    {
	//read start channel settings from JSON setting file. 
	//TODO: write php web GUI to populate the JSON file
        if (LoadJsonFromFile("/home/fpp/media/config/fpp-tasmota-plugin.json", config)) {
            for(auto & out: _tasmotaOutputs)
            {
		//TODO: change JSON to parameter/value list
                printf ("Reading Start Channel %s\n" ,out->GetIPAddress().c_str());
                if (config.isMember(out->GetIPAddress()))
                {
                    out->SetStartChannel(config[out->GetIPAddress()].asInt());
                    printf ("Setting Start Channel %d\n",out->GetStartChannel() );
                }
                else
                {
                    printf ("No Start Channel Setting Found\n" );
                }
            }
        }
        
        saveDataToFile();
    }
    
    std::string getIPs()
    {
        std::string ips;
        for(auto & out: _tasmotaOutputs)
        {
            ips += out->GetIPAddress();
            ips += ",";
        }
        printf ("IP Adresses %s\n" ,ips.c_str());
        return ips;
    } 
};


extern "C" {
    FPPPlugin *createPlugin() {
        return new FPPTasmotaPlugin();
    }
}
