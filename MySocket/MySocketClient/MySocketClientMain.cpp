
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
	
	connect(cSocket, (SOCKADDR*)&saddr, sizeof(saddr));

	WSACleanup();

	system("pause");
	return 0;
}