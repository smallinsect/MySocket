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
			Login login = {"°®°×²Ë", "Ð¡À¥³æ"};
			DataHeader dh = { CMD_LOGIN, sizeof(Login)};
			send(cSocket, (const char *)&dh, sizeof(DataHeader), 0);
			send(cSocket, (const char *)&login, sizeof(Login), 0);

			DataHeader retHeader = {};
			LoginResult loginRet = {};
			recv(cSocket, (char *)&retHeader, sizeof(DataHeader), 0);
			recv(cSocket, (char *)&loginRet, sizeof(LoginResult), 0);

			cout << "LoginResult: " << loginRet.result << endl;
		}else if(strcmp(cmdBuf, "logout") == 0){
			Loginout logout = {"°®°×²ËµÄÐ¡À¥³æ"};
			DataHeader dh = { CMD_LOGINOUT, sizeof(Loginout)};
			
			send(cSocket, (const char *)&dh, sizeof(DataHeader), 0);
			send(cSocket, (const char *)&logout, sizeof(Loginout), 0);

			DataHeader retHeader = {};
			LoginoutResult logoutRet = {};
			recv(cSocket, (char *)&retHeader, sizeof(DataHeader), 0);
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