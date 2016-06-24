//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <array>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/assert.hpp>

#include <stdint.h>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

namespace
{
char defaultAddress[] = "127.0.0.1";
char defaultPort[] = "53462";

const char requestString[] = "+SEND_SELECT_HISTORY=1,%d,%d,1";
}




int main(int argc, char* argv[])
{
//	array <char, 1024> RequestBuffer;
  try
  {
	  char *currentAddress;
	  char *currentPort;
    if (argc == 3)
    {
    	currentAddress = argv[1];
    	currentPort = argv[2];
    }
    else
    {
    	currentAddress = defaultAddress;
		currentPort = defaultPort;
//      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
//      return 1;
    }

//    __cplusplus;
//    __cplusplus=201103L
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), currentAddress, currentPort);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    std::cout << "Connect OK!" << std::endl;
    using namespace std; // For strlen.

    uint32_t IndexStart = 0, IndexEnd = 0;

    std::fflush(stdin);
    std::cout << "Enter index range, start: ";
    std::cin >> IndexStart;
    std::cout << "\t end: ";
    std::cin >> IndexEnd;
//    std::cin.getline(request, max_length);
//    char request[max_length];
//    sprintf(request, "+SEND_SELECT_HISTORY=1,%d,%d,1", IndexStart, IndexEnd);
//    size_t request_length = strlen(request);
//    boost::asio::write(s, boost::asio::buffer(request, request_length));


    char tempBuf[1024];
    sprintf(tempBuf, requestString, IndexStart, IndexEnd);
    std::string tempString{"+SEND_SELECT_HISTORY=1,"};

    tempString += std::to_string(IndexStart) + "," + std::to_string(IndexEnd) + "\r\n";

    cout << tempString;
//    RequestBuffer.insert(tempBuf.begin(), tempBuf.end());
    boost::asio::write(s, boost::asio::buffer(tempBuf, strlen(tempBuf)));


    char reply[max_length];
    size_t reply_length = boost::asio::read(s,
        boost::asio::buffer(reply, 10));
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";

    s.close();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
