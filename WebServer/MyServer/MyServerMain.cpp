//////////////////////////////////////////////////////////////////////////
//
// �������˳���
//
//////////////////////////////////////////////////////////////////////////

#include "MyWeb.h"

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����������׽���
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		puts("[sever] socket error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] socket success ...");
	//��������IP�Ͷ˿�
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	//saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//������ip��bind�Զ��󶨱���ip��ַ��
	if (bind(serverSocket, (const sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		puts("[sever] bind error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] bind success ...");

	//��������ʼ����
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("[sever] listen error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] listen success ...");

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(serverSocket, &fds);

	map<SOCKET, string> mSocketInfo;
	ostringstream ostr;
	ostr << "[server] socket:"<< serverSocket << " ip:" << inet_ntoa(saddr.sin_addr) << " port:" << saddr.sin_port;
	cout << ostr.str() << " accept ...\n";

	mSocketInfo.insert(pair<SOCKET, string>(serverSocket, ostr.str()));
	ostr.str("");//���������

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	while (true) {
		fd_set fdsTemp = fds;
		if (select(0, &fdsTemp, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < fdsTemp.fd_count; ++i) {
			if (fdsTemp.fd_array[i] == serverSocket) {
				sockaddr_in clientAddr = { 0 };
				int clientAddrLen = sizeof(sockaddr_in);
				SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
				if (clientSocket == INVALID_SOCKET) {
					puts("[sever] accept error ...");
					system("pause");
					exit(-1);
				}

				FD_SET(clientSocket, &fds);

				ostr << "[client] socket:" << clientSocket << " ip:" << inet_ntoa(clientAddr.sin_addr) << " port:" << clientAddr.sin_port;
				cout << ostr.str() << " accept ...\n";
				mSocketInfo.insert(pair<SOCKET, string>(clientSocket, ostr.str()));
				ostr.str("");

				continue;
			}
			//�������������
			if (Request(fdsTemp.fd_array[i]) > 0) {
				//��Ӧ���������
				Response(fdsTemp.fd_array[i]);
			}
			else {
				//������˳�
				SOCKET clientSocket = fdsTemp.fd_array[i];
				FD_CLR(fdsTemp.fd_array[i], &fds);
				closesocket(clientSocket);
				map<SOCKET, string>::iterator iterFind = mSocketInfo.find(clientSocket);
				if (iterFind != mSocketInfo.end()) {
					cout << iterFind->second.c_str() << " quit ...\n";
					mSocketInfo.erase(iterFind);
				}
				else {
					cout << "[client] ??? quit ...\n";
				}
			}
		}
	}

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}