#include "TasmotaBulb.h"


#include <stdlib.h>
#include <cstdint>


#include <iostream> 
#include <istream>
#include <ostream>


TasmotaBulb::TasmotaBulb(std::string const& ip, unsigned int startChannel) :
_startChannel(startChannel),
_ipAddress(ip)
{
}

TasmotaBulb::~TasmotaBulb()
{

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

}

void TasmotaBulb::outputData( unsigned char *data)
{
	
}
