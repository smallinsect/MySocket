/***************************************************
网络报文的数据结构格式
***************************************************/
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>

#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

enum CMD{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_ERROR
};
struct DataHeader{
	short cmd;
	short dataLength;
};
struct Login : public DataHeader{
	Login(){
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
		memset(userName, 0, sizeof(userName));
		memset(passWord, 0, sizeof(passWord));
	}
	char userName[32];
	char passWord[32];
};
struct LoginResult : public DataHeader{
	LoginResult(){
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		result = 1;
	}
	int result;
};
struct Loginout : public DataHeader{
	Loginout(){
		cmd = CMD_LOGINOUT;
		dataLength = sizeof(Loginout);
		memset(userName, 0, sizeof(userName));
	}
	char userName[32];
};
struct LoginoutResult : public DataHeader{
	LoginoutResult(){
		cmd = CMD_LOGINOUT_RESULT;
		dataLength = sizeof(LoginoutResult);
		result = 2;
	}
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
			break;
		}
		cout << "recv success ..." << endl;
		cout << "client cmd : " << header.cmd << endl;
		switch(header.cmd){
		case CMD_LOGIN:
			{
				Login login;
				recv(cSocket, (char *)&login+sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
				cout << "recv cmd : " << login.cmd << endl;
				cout << "recv len : " << login.dataLength << endl;
				cout << "userName : " << login.userName << endl;
				cout << "passWord : " << login.passWord << endl;
				LoginResult ret;
				send(cSocket, (const char *)&ret, sizeof(LoginResult), 0);
			}
			break;
		case CMD_LOGINOUT:
			{
				Loginout loginout;
				recv(cSocket, (char *)&loginout+sizeof(DataHeader), sizeof(Loginout)-sizeof(DataHeader), 0);
				cout << "recv cmd : " << loginout.cmd << endl;
				cout << "recv len : " << loginout.dataLength << endl;
				cout << "userName : " << loginout.userName << endl;
				LoginoutResult ret;
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
	cout << "client exit ..." << endl;
	closesocket(sSocket);
	WSACleanup();

	system("pause");
	return 0;
}