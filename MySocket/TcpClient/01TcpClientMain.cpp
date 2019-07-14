/*************************************************************
**socket简单的客户端
**
**
**
**
**
**************************************************************/

//#define WIN32_LEAN_AND_MEAN
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <string.h>
//#include <windows.h>
//#include <winsock2.h>
//
//#pragma comment(lib, "ws2_32.lib")
//
//#include <iostream>
//using namespace std;
//
//int main() {
//
//	WSADATA wd;
//	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
//	if (ret != 0) {
//		cout << "WSAStartup error ..." << endl;
//		return -1;
//	}
//	cout << "WSAStartup success ..." << endl;
//
//	SOCKET _cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (_cSocket == SOCKET_ERROR) {
//		cout << "socket error ..." << endl;
//		return -1;
//	}
//	cout << "socket success ..." << endl;
//
//	sockaddr_in caddr;
//	caddr.sin_family = AF_INET;
//	caddr.sin_port = htons(8000);
//	caddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
//
//	if (connect(_cSocket, (sockaddr *)&caddr, sizeof(caddr)) == SOCKET_ERROR) {
//		cout << "connect error ..." << endl;
//		return -1;
//	}
//	cout << "connect success ..." << endl;
//
//	char buf[1024];
//	memset(buf, 0, sizeof(buf));
//	if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//		cout << "recv error ..." << endl;
//		return -1;
//	}
//	cout << "recv success ..." << endl;
//	
//	cout << "server msg : " << buf << endl;
//
//	cout << "input msg >>";
//	cin >> buf;
//	send(_cSocket, buf, strlen(buf), 0);
//
//	WSACleanup();
//	system("pause");
//	return 0;
//}