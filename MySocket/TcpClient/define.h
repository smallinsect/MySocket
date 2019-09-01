#pragma once

struct DataPackage {
	int iAge;
	char szName[32];
};

//��������
enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
//��Ϣͷ
struct DataHeader {
	int cmd;//��Ϣָ��
	int dataLength;//��Ϣ����
};
//��¼��Ϣ
struct Login : public DataHeader {
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
	}
	char szName[64];//�û���
	char szPass[64];//����
};
//��¼������Ϣ
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		iResult = 1;
	}
	int iResult;
};
//�ǳ���Ϣ
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
	}
	char szName[64];
};
//�ǳ�������Ϣ
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		iResult = 2;
	}
	int iResult;
};