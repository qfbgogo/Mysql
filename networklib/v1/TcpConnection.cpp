 ///
 /// @file    TcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-05 17:02:41
 ///

#include "TcpConnection.h"
#include <string.h>
#include <stdio.h>


namespace wd
{

TcpConnection::TcpConnection(int sockfd)
: sockfd_(sockfd)
, sockIO_(sockfd)
, localAddr_(wd::Socket::getLocalAddr(sockfd))
, peerAddr_(wd::Socket::getPeerAddr(sockfd))
, isShutdownWrite_(false)
{
	sockfd_.nonblock();
}


TcpConnection::~TcpConnection()
{
	if(!isShutdownWrite_)
	{
		isShutdownWrite_ = true;
		shutdown();
	}
	printf("~TcpConnection()\n");
}

std::string TcpConnection::receive()
{
	char buf[65536];
	memset(buf, 0, sizeof(buf));
	size_t ret = sockIO_.readline(buf, sizeof(buf));
	if(ret == 0)
	{
		return std::string();
	}
	else
		return std::string(buf);
}

void TcpConnection::send(const std::string & msg)
{
	sockIO_.writen(msg.c_str(), msg.size());
}


void TcpConnection::shutdown()
{
	if(!isShutdownWrite_)
		sockfd_.shutdownWrite();
	isShutdownWrite_ = true;
}

std::string TcpConnection::toString()
{
	char str[100];
	snprintf(str, sizeof(str), "%s:%d -> %s:%d",
			 localAddr_.ip().c_str(),
			 localAddr_.port(),
			 peerAddr_.ip().c_str(),
			 peerAddr_.port());
	return std::string(str);
}
}// end of namespace wd
