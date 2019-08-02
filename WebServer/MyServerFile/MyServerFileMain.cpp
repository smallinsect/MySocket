
#include "MyServerWeb.h"


int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2,2), &wd);

	//创建套接字
	SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockServer == INVALID_SOCKET) {
		cout << "[server] socket error ...\n";
		system("pause");
		return -1;
	}
	cout << "[server] socket success ...\n";

	sockaddr_in saddrServer = {0};
	saddrServer.sin_family = AF_INET;//协议IPV4
	saddrServer.sin_port = htons(8080);//端口8080
	//saddrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP地址127.0.0.1

	//套接字绑定协议、端口、IP地址
	if (bind(sockServer, (sockaddr *)&saddrServer, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "[server] bind error ...\n";
		system("pause");
		return -1;
	}
	cout << "[server] bind success ...\n";

	//开始监听
	if (listen(sockServer, SOMAXCONN) == SOCKET_ERROR) {
		cout << "[server] listen error ...\n";
		system("pause");
		return -1;
	}
	cout << "[server] listen success ...\n";

	fd_set fdsSockets;
	FD_ZERO(&fdsSockets);
	FD_SET(sockServer, &fdsSockets);

	ostringstream ostr;
	ostr << "[server] socket:" << sockServer << " ip:" << inet_ntoa(saddrServer.sin_addr) << " port:" << saddrServer.sin_port;
	cout << ostr.str() << endl;

	map<SOCKET, string> mSockInfo;//socket信息
	mSockInfo.insert(pair<SOCKET, string>(sockServer, ostr.str()));
	ostr.str("");//清空

	timeval tv = { 0, 3000 };
	while (true) {
		fd_set fdsTemp = fdsSockets;

		int iret = select(0, &fdsTemp, NULL, NULL, &tv);
		if (iret == 0) {
			continue;
		}
		if (iret < 0) {
			cout << "[server] select error ...\n";
			break;
		}
		for (u_int i = 0; i < fdsTemp.fd_count; ++i) {
			if (sockServer == fdsTemp.fd_array[i]) {//服务器接收请求
				sockaddr_in saddrClient = {0};
				int saddrClientLen = sizeof(sockaddr_in);
				SOCKET sockClient = accept(sockServer, (sockaddr *)&saddrClient, &saddrClientLen);
				if (sockClient == INVALID_SOCKET) {
					cout << "[server] accept error ...\n";
					continue;
				}
				cout << "[server] accept success ...\n";
				ostr << "[client] socket:" << sockClient << " ip:" << inet_ntoa(saddrClient.sin_addr) << " port:" << saddrClient.sin_port;
				cout << ostr.str() << endl;

				FD_SET(sockClient, &fdsSockets);

				mSockInfo.insert(pair<SOCKET, string>(sockClient, ostr.str()));
				ostr.str("");
				continue;
			}
			if (request(fdsTemp.fd_array[i]) > 0) {//浏览器请求
				response(fdsTemp.fd_array[i]);//服务器响应
			}
			else {//清理socket
				SOCKET sockTemp = fdsTemp.fd_array[i];
				map<SOCKET, string>::iterator itr = mSockInfo.find(sockTemp);
				if (itr != mSockInfo.end()) {
					cout << itr->second.c_str() << " quit ...\n";
					mSockInfo.erase(itr);
				}
				else {
					cout << "socket:" << sockTemp << " ??? quit ...\n";
				}

				FD_CLR(sockTemp, &fdsSockets);
				closesocket(sockTemp);
			}
		}
	}

	FD_ZERO(&fdsSockets);
	closesocket(sockServer);

	system("pause");
	return 0;
}
