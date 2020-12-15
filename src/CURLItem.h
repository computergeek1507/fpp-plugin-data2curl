#pragma once

#include <stdlib.h>
#include <list>
#include <string>
#include <atomic>

#include "common.h"
#include "log.h"

#include <curl/curl.h>

class CURLItem {
public:
    CURLItem(std::string const& ip, unsigned int port, std::string const& url, std::string const& message, 
                                    std::string const& type, std::string const& contentType, unsigned int startChannel );
    ~CURLItem();

    bool SendData( unsigned char *data);

    std::string GetIPAddress(){return _ipAddress;}
    std::string GetURL(){return _url;}
    std::string GetMessage(){return _message;}
    unsigned int GetStartChannel(){return _startChannel;}
    unsigned int GetPort(){return _port;}
    std::string GetType(){return _type;}
    std::string GetContentType(){return _contentType;}
    void EnableOutput(){_unreachable = false;}

private:
    std::string _ipAddress;
    unsigned int _port;
    std::string _url;
    std::string _message;
    std::string _contentType;
    std::string _type;
    unsigned int _startChannel;
    unsigned int _seqCount;

    uint8_t _r;
    uint8_t _g;
    uint8_t _b;
    std::atomic<bool> _unreachable;

    CURL *m_curl;

    void outputData( uint8_t r ,uint8_t g ,uint8_t b );

    void replaceValues(std::string & valueStr, uint8_t r ,uint8_t g ,uint8_t b );
    void RGBtoHSIV(float fR, float fG, float fB, float& fH, float& fSI, float& fSV,float& fI, float& fV);
};