#pragma once

struct DataPackage {
	int iAge;
	char szName[32];
};

//交互命令
enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
//消息头
struct DataHeader {
	int cmd;//消息指令
	int dataLength;//消息长度
};
//登录消息
struct Login : public DataHeader {
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
	}
	char szName[64];//用户名
	char szPass[64];//密码
};
//登录返回消息
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		iResult = 1;
	}
	int iResult;
};
//登出消息
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
	}
	char szName[64];
};
//登出返回消息
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		iResult = 2;
	}
	int iResult;
};