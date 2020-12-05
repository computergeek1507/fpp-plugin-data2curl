#include "TasmotaBulb.h"

#include <stdlib.h>
#include <cstdint>

#include <iostream>
#include <istream>
#include <ostream>

TasmotaBulb::TasmotaBulb(std::string const& ip, unsigned int startChannel) :
    _startChannel(startChannel),
    _ipAddress(ip),
    _r(255),
    _g(255),
    _b(255),
    m_curl(NULL)
{
    m_curl = curl_easy_init();
}

TasmotaBulb::~TasmotaBulb()
{
    if (m_curl)
        curl_easy_cleanup(m_curl);
}

bool TasmotaBulb::BulbOn()
{
    try
    {
        sendBulbOn();
        return true;
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }
    return false;
}

bool TasmotaBulb::SendData( unsigned char *data)
{
    try
    {
        outputData(data );
        return true;
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }
    return false;
}


void TasmotaBulb::sendBulbOn()
{
    std::string repURL = "http://" + _ipAddress + "/cm?cmnd=Power%20On";
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());

    CURLcode status = curl_easy_perform(m_curl);
    if (status != CURLE_OK) {
        std::cout << "failed to send on command";
    }
}

void TasmotaBulb::outputData( unsigned char *data)
{
    uint8_t r = data[_startChannel - 1];
    uint8_t g = data[_startChannel];
    uint8_t b = data[_startChannel + 1];

    if(r == _r && g == _g && b == _b) {
        std::cout << "data is the same, skipping";
        return;
    }
    _r = r;
    _g = g;
    _b = b;

    std::string repURL = "http://" + _ipAddress + "/cm?cmnd=Color%20" + std::to_string(r)
    + "," + std::to_string(g) + ","  + std::to_string(b);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());

    CURLcode status = curl_easy_perform(m_curl);
    if (status != CURLE_OK) {
        std::cout << "failed to send color command";
    }
}
