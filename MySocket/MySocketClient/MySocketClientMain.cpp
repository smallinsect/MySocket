#define WIN32_LEAN_AND_MEAN

#include <string.h>

#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

int main(){
	cout << "Hell world client ..." << endl;

	WSADATA wd;
	int err = WSAStartup(MAKEWORD(2,2), &wd);
	if(err != 0){
		cout << "WSAStartup failed with error: " << err << endl;
		return -1;
	}

	SOCKET cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(cSocket == SOCKET_ERROR){
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	if(connect(cSocket, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR){
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;

	char buf[1024] = "";
	if(recv(cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR){
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;

	cout << "server : " << buf << endl;

	WSACleanup();

	system("pause");
	return 0;
}