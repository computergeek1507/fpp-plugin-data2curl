#pragma once

#include <stdlib.h>
#include <list>
#include <string>

#include <curl/curl.h>

class TasmotaBulb {
public:
    TasmotaBulb(std::string const& ip, unsigned int startChannel );
    ~TasmotaBulb();

    bool BulbOn();

    std::string GetIPAddress(){return _ipAddress;}
    bool SendData( unsigned char *data);

    unsigned int GetStartChannel(){return _startChannel;}

private:

    unsigned int _startChannel;
    std::string _ipAddress;
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;

    CURL                  *m_curl;

    void outputData( unsigned char *data);
    void sendBulbOn();
};