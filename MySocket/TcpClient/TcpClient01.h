#pragma once

#include "AllHeader.h"

class TcpClient01
{
private:
	SOCKET skt;

public:
	//≥ı ºªØsocket
	void init();
	void connectServer(const char *IP, u_short port);
	void onRun();
	bool isRun();
	void readData();
	void writeData();

	void closeSocket();

public:
	TcpClient01();
	~TcpClient01();
};

