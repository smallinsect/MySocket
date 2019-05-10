#define WIN32_LEAN_AND_MEAN

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

	if(listen(sSocket, 5) == SOCKET_ERROR){
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

	char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
	SOCKET cSocket;
	SOCKADDR_IN caddr = {};
	int caddrlen = sizeof(caddr);
	while(true){
		cSocket = accept( sSocket, (SOCKADDR *)&caddr, &caddrlen);
		if(cSocket == INVALID_SOCKET){
			cout << "accept error ..." << endl;
			break;
		}
		cout << "accopet success ..." << endl;
		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;

		send(cSocket, buf, strlen(buf), 0);
	}

	WSACleanup();

	system("pause");
	return 0;
}