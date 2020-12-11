#pragma once

#include <stdlib.h>
#include <list>
#include <string>

#include "common.h"
#include "log.h"

#include <curl/curl.h>

class CURLItem {
public:
    CURLItem(std::string const& ip, std::string const& url, std::string const& message, unsigned int startChannel );
    ~CURLItem();

    bool SendData( unsigned char *data);

    std::string GetIPAddress(){return _ipAddress;}
    std::string GetURL(){return _url;}
    std::string GetMessage(){return _message;}
    unsigned int GetStartChannel(){return _startChannel;}
    void EnableOutput(){_unreachable = false;}

private:

    unsigned int _startChannel;
    std::string _ipAddress;
    std::string _url;
    std::string _message;
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;
    bool _unreachable;

    CURL *m_curl;

    void outputData( uint8_t r ,uint8_t g ,uint8_t b );

    void replaceValues(std::string & valueStr, uint8_t r ,uint8_t g ,uint8_t b );
    void RGBtoHSI(float fR, float fG, float fB, float& fH, float& fS, float& fI);
};