
#include <string.h>

#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

int main(){
	cout << "Hell world server ..." << endl;

	WSADATA wd;
	int err = WSAStartup(MAKEWORD(2,2), &wd);
	if(err != 0){
		cout << "WSAStartup failed with error: " << err << endl;
		return -1;
	}

	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sSocket == SOCKET_ERROR){
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if(bind( sSocket, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR){
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
	SOCKET cSocket;
	while(true){
		cSocket = listen( sSocket, 5);

		send(cSocket, buf, strlen(buf), 0);
	}

	WSACleanup();

	system("pause");
	return 0;
}