
#ifndef __COMMON_H__
#define __COMMON_H__

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define FD_SETSIZE 1024

#include <conio.h>
#include <string.h>
#include <process.h>

#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")



#include <vector>
#include <iostream>
using namespace std;

#define RECV_BUF_SIZE 10240

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_DATA,
	CMD_DATA_RESULT,
	CMD_ERROR,
	CMD_NEW_USER_JOIN
};
struct DataHeader {
	DataHeader() {
		cmd = CMD_ERROR;
		dataLength = sizeof(DataHeader);
	}
	short cmd;
	short dataLength;
};
struct Login : public DataHeader {
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
		memset(userName, 0, sizeof(userName));
		memset(passWord, 0, sizeof(passWord));
	}
	char userName[32];
	char passWord[32];
};
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		result = 1;
	}
	int result;
};
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
		memset(userName, 0, sizeof(userName));
	}
	char userName[32];
};
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		result = 2;
	}
	int result;
};
struct NewUserJoin : public DataHeader {

};
struct Data : DataHeader {
	Data() {
		cmd = CMD_DATA;
		dataLength = sizeof(Data);
		memset(szData, 0, 1024);
	}
	char szData[1024];
};
struct DataResult : public DataHeader {
	DataResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		result = CMD_LOGOUT_RESULT;
	}
	int result;
	char szData[1024];
};


#endif
