#define WIN32_LEAN_AND_MEAN

#include <string.h>

#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

enum CMD{
	CMD_LOGIN,
	CMD_LOGINOUT,
	CMD_ERROR
};
struct DataHeader{
	short cmd;
	short dataLength;
};
struct Login{
	char userName[32];
	char passWord[32];
};
struct LoginResult{
	int result;
};
struct Loginout{
	char userName[32];
};
struct LoginoutResult{
	int result;
};


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

	char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
	SOCKET cSocket;
	SOCKADDR_IN caddr = {};
	int caddrlen = sizeof(caddr);
	cSocket = accept( sSocket, (SOCKADDR *)&caddr, &caddrlen);
	if(cSocket == INVALID_SOCKET){
		cout << "accept error ..." << endl;
		return -1;
	}
	cout << "accopet success ..." << endl;
	cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
	cout << "client port : " << caddr.sin_port << endl;

	while(true){
		DataHeader header = {};
		int nLen = recv(cSocket, (char *)&header, sizeof(DataHeader), 0);
		if(nLen == SOCKET_ERROR){
			cout << "recv error ..." << endl;
			return -1;
		}
		cout << "recv success ..." << endl;
		cout << "client cmd : " << header.cmd << endl;
		switch(header.cmd){
		case CMD_LOGIN:
			{
				Login login = {};
				recv(cSocket, (char *)&login, sizeof(Login), 0);
				cout << "userName : " << login.userName << endl;
				cout << "passWord : " << login.passWord << endl;
				LoginResult ret = {1};
				send(cSocket, (const char *)&header, sizeof(DataHeader), 0);
				send(cSocket, (const char *)&ret, sizeof(LoginResult), 0);
			}
			break;
		case CMD_LOGINOUT:
			{
				Loginout loginout = {};
				recv(cSocket, (char *)&loginout, sizeof(Loginout), 0);
				cout << "userName : " << loginout.userName << endl;
				LoginoutResult ret = {2};
				send(cSocket, (const char *)&header, sizeof(DataHeader), 0);
				send(cSocket, (const char *)&ret, sizeof(LoginoutResult), 0);
			}
			break;
		default:
			{
				header.cmd = CMD_ERROR;
				header.dataLength = 0;
				send(cSocket, (char *)&header, sizeof(DataHeader), 0);
			}
			break;
		}
	}

	WSACleanup();

	system("pause");
	return 0;
}