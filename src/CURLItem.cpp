#include "CURLItem.h"

#include <stdlib.h>
#include <cstdint>
#include <thread>
#include <cmath>

#include <iostream>
#include <istream>
#include <ostream>

CURLItem::CURLItem(std::string const& ip, unsigned int port, std::string const& url, std::string const& message, 
                                std::string const& type, std::string const& contentType, unsigned int startChannel) :
    _startChannel(startChannel),
    _ipAddress(ip),
    _port(port),
    _url(url),
    _message(message),
    _type(type),
    _contentType(contentType),
    _r(0),
    _g(0),
    _b(0),
    _unreachable(false),
    m_curl(NULL)
{
    m_curl = curl_easy_init();
}

CURLItem::~CURLItem()
{
    if (m_curl)
        curl_easy_cleanup(m_curl);
}

bool CURLItem::SendData( unsigned char *data)
{
    try
    {
        if(_unreachable)
            return false;

        uint8_t r = data[_startChannel - 1];
        uint8_t g = data[_startChannel];
        uint8_t b = data[_startChannel + 1];

        if(r == _r && g == _g && b == _b) {
            return true;
        }
        _r = r;
        _g = g;
        _b = b;

        std::thread t(&CURLItem::outputData, this, r, g, b );
        t.detach();
        //outputData(r, g, b );
        return true;
    }
    catch(std::exception ex)
    {
        _unreachable = true;
        LogInfo(VB_PLUGIN, "Error %s \n",ex.what());
    }
    return false;
}

void CURLItem::outputData( uint8_t r ,uint8_t g ,uint8_t b )
{
    std::string newURL = _url;
    std::string newMessage = _message;
    if(!newURL.empty()){
        replaceValues(newURL, r, g, b);
    }

    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, _type.c_str());
    struct curl_slist *hs = NULL;
    std::string const content = "Content-Type: " + _contentType;
    hs = curl_slist_append(hs, content.c_str());
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, hs);

    if(!newMessage.empty()){
        replaceValues(newMessage, r, g, b);
        LogInfo(VB_PLUGIN, "Data '%s'\n",newMessage.c_str());
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, newMessage.c_str());
    }

    std::string const repURL = "http://" + _ipAddress + ":" + std::to_string(_port) + newURL;
    LogInfo(VB_PLUGIN, "URL '%s'\n",repURL.c_str());
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());

    CURLcode status = curl_easy_perform(m_curl);
    if (status != CURLE_OK) {
        _unreachable = true;
        LogInfo(VB_PLUGIN, "failed to send curl command\n");
    }
}

void CURLItem::replaceValues(std::string & valueStr, uint8_t r ,uint8_t g ,uint8_t b )
{
    replaceAll(valueStr, "%R%" , std::to_string(r));
    replaceAll(valueStr, "%G%" , std::to_string(g));
    replaceAll(valueStr, "%B%" , std::to_string(b));

    int rScale = (r * 100)/255;
    int gScale = (g * 100)/255;
    int bScale = (b * 100)/255;

    replaceAll(valueStr, "%RS%" , std::to_string(rScale));
    replaceAll(valueStr, "%GS%" , std::to_string(gScale));
    replaceAll(valueStr, "%BS%" , std::to_string(bScale));

    if(r > 254)
        replaceAll(valueStr, "%SW%" , "ON");
    else
        replaceAll(valueStr, "%SW%" , "OFF");

    float h,s,i;

    RGBtoHSI(r/255,g/255,b/255,h,s,i);
    
    int ih = (h);
    int is = (s*100);
    int ii = (i*100);
    
    replaceAll(valueStr, "%H%" , std::to_string(ih));
    replaceAll(valueStr, "%S%" , std::to_string(is));
    replaceAll(valueStr, "%I%" , std::to_string(ii));
}

void CURLItem::RGBtoHSI(float fR, float fG, float fB, float& fH, float& fS, float& fI) {
    float M  = std::max(std::max(fR, fG), fB);
    float m = std::min(std::min(fR, fG), fB);
    float c = M-m;
  
    fI = (1.0/3.0)*(fR+fG+fB);
  
    if(c==0) {
        fH = 0.0;
        fS = 0.0;
    }
    else {
        if(M==fR) {
            fH = fmod(((fG-fB)/c), 6.0);
        }
        else if(M==fG) {
            fH = (fB-fR)/c + 2.0;
        }
        else if(M==fB) {
            fH = (fR-fG)/c + 4.0;
        }
        fH *=60.0;
        if(fI!=0) {
            fS = 1.0 - (m/fI);
        }
        else {
            fS = 0.0;
        }
    }

    if(fH < 0.0)
        fH += 360.0;
}

