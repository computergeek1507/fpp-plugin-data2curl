
#include "TasmotaBulb.h"


#include <stdlib.h>
#include <list>
#include <string>
#include <cstdint>


#include <iostream> 
#include <istream>
#include <ostream>

#include <fstream>
#include <filesystem>

TasmotaBulb::TasmotaBulb() : _startChannel(1)
{
}

TasmotaBulb::~TasmotaBulb()
{

}

bool TasmotaBulb::BulbOn()
{
	try
	{
		if(_ipAddress.empty())
			return false;

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
		if(_ipAddress.empty())
			return false;

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
