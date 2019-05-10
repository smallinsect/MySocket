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

	while(true){
		char cmdBuf[1024] = "";
		cout << "input cmd >>";
		cin >> cmdBuf;
		if(strcmp(cmdBuf, "exit") == 0){
			cout << "exit client ..." << endl;
			break;
		}else if(strcmp(cmdBuf, "login") == 0){
			Login login;
			strcpy(login.userName, "爱白菜");
			strcpy(login.passWord, "小昆虫");
			send(cSocket, (const char *)&login, sizeof(Login), 0);

			LoginResult loginRet;
			recv(cSocket, (char *)&loginRet, sizeof(LoginResult), 0);

			cout << "LoginResult: " << loginRet.result << endl;
		}else if(strcmp(cmdBuf, "logout") == 0){
			Loginout logout;
			strcpy(logout.userName, "爱白菜的小昆虫");
			send(cSocket, (const char *)&logout, sizeof(Loginout), 0);

			LoginoutResult logoutRet;
			recv(cSocket, (char *)&logoutRet, sizeof(LoginResult), 0);

			cout << "LogoutResult: " << logoutRet.result << endl;
		}else{
			cout << "cmd error" << endl;
		}
	}

	WSACleanup();

	system("pause");
	return 0;
}