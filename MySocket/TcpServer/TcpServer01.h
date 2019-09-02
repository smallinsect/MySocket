#pragma once

#include "AllHeader.h"

class TcpServer01
{
private:
	SOCKET skt;
	sockaddr_in addr;
public:
	void init(const char *IP, u_short port);
	void onRun();
	bool isRun();
	void readData(char *buf);
	void writeData(char *buf);
	void closeSocket();
public:
	TcpServer01();
	~TcpServer01();
};

