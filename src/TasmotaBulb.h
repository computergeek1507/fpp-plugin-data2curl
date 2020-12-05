#pragma once

#include <stdlib.h>
#include <list>
#include <string>

class TasmotaBulb {
public:
    TasmotaBulb(std::string const& ip, unsigned int startChannel );
    ~TasmotaBulb();

    bool BulbOn();

    std::string GetIPAddress(){return _ipAddress;}
    bool SendData( unsigned char *data);

    unsigned int GetStartChannel(){return _startChannel;}
    //void SetStartChannel(unsigned int startChannel){ _startChannel = startChannel;}


private:

    unsigned int _startChannel;
    std::string _ipAddress;

    void outputData( unsigned char *data);
    void sendBulbOn();
};