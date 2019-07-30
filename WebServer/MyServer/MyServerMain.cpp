//////////////////////////////////////////////////////////////////////////
//
// �������˳���
//
//////////////////////////////////////////////////////////////////////////

#include "MyWeb.h"

int main(int argc, char *argv[]) {
	puts("hello world");

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����������׽���
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		puts("socket error ...");
		system("pause");
		exit(-1);
	}
	puts("socket success ...");
	//��������IP�Ͷ˿�
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	//saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//������ip��bind�Զ��󶨱���ip��ַ��
	if (bind(serverSocket, (const sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		puts("bind error ...");
		system("pause");
		exit(-1);
	}
	puts("bind success ...");

	//��������ʼ����
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("listen error ...");
		system("pause");
		exit(-1);
	}
	puts("listen success ...");

	while (true) {
		sockaddr_in clientAddr = { 0 };
		int clientAddrLen = sizeof(sockaddr_in);
		SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
		if (clientSocket == INVALID_SOCKET) {
			puts("accept error ...");
			system("pause");
			exit(-1);
		}
		puts("accept success ...");
		printf("client ip: %s\n", inet_ntoa(clientAddr.sin_addr));
		printf("client port:%d\n", clientAddr.sin_port);

		while (true) {
			//�������������
			if (Request(clientSocket) != 0) {
				break;
			}
			//��Ӧ���������
			Response(clientSocket);
		}
		closesocket(clientSocket);
		printf("client quit ...\n");
	}

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}