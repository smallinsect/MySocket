/*************************************************************
**socket客户端
**
**客户端与服务端的数据交互
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
//struct Message {
//	int iCmd;//指令
//	char szMsg[1024];//消息
//};
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
//	while (true) {
//		cout << "input cmd >>";
//		cin >> buf;
//		char msgBuf[1024] = "";
//		if (strcmp(buf, "SignIn") == 0) {
//			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//				cout << "send error ..." << endl;
//				return -1;
//			}
//			cout << "send success ..." << endl;
//			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
//				cout << "recv error ..." << endl;
//			}
//			cout << "recv success ..." << endl;
//			cout << "server : " << msgBuf << endl;
//		} else if (strcmp(buf, "getName") == 0) {
//			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//				cout << "send error ..." << endl;
//				return -1;
//			}
//			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
//				cout << "recv error ..." << endl;
//			}
//			cout << "recv success ..." << endl;
//			cout << "server : " << msgBuf << endl;
//		} else if (strcmp(buf, "getAge") == 0) {
//			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//				cout << "send error ..." << endl;
//				return -1;
//			}
//			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
//				cout << "recv error ..." << endl;
//			}
//			cout << "recv success ..." << endl;
//			cout << "server : " << msgBuf << endl;
//		} else if (strcmp(buf, "SignOut") == 0) {
//			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//				cout << "send error ..." << endl;
//				return -1;
//			}
//			cout << "send success ..." << endl;
//			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
//				cout << "recv error ..." << endl;
//			}
//			cout << "recv success ..." << endl;
//			cout << "server : " << msgBuf << endl;
//		} else if (strcmp(buf, "Quit") == 0) {
//			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
//				cout << "send error ..." << endl;
//				return -1;
//			}
//			cout << "send success ..." << endl;
//			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
//				cout << "recv error ..." << endl;
//			}
//			cout << "recv success ..." << endl;
//			cout << "server : " << msgBuf << endl;
//			break;
//		} else {
//			cout << "iCmd error ..." << endl;
//		}
//	}
//
//
//	WSACleanup();
//	system("pause");
//	return 0;
//}