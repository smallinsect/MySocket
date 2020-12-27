#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"kernel32.lib")

#define MAX_BUFFER_LEN 1024

HANDLE g_hIOCP;

enum IO_OPERATION { IO_READ, IO_WRITE };

//typedef struct _PER_SOCKET_CONTEXT {
//	SOCKET      m_Socket;
//	// 每一个客户端连接的Socket  
//	SOCKADDR_IN m_ClientAddr;
//	// 客户端的地址  
//	CArray<_PER_IO_CONTEXT*> m_arrayIoContext;
//	// 客户端网络操作的上下文数据，  
//}PER_SOCKET_CONTEXT, *LP_PER_SOCKET_CONTEXT;

typedef struct _IO_CONTEXT {
	OVERLAPPED   overlapped;   // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
	WSABUF       wsabuf;       // WSA类型的缓冲区，用于给重叠操作传参数的
	char         buffer[MAX_BUFFER_LEN];// 这个是WSABUF里具体存字符的缓冲区  
	IO_OPERATION opCode;       // 标识网络操作的类型(对应上面的枚举)
	DWORD        nBytes;
	DWORD        dwFlags;
} IO_CONTEXT, *LP_IO_CONTEXT;

char sz[] = "255.255.255.255:65535";

typedef struct _COMPLETION_KEY {
	SOCKET      Socket;
	SOCKADDR_IN Addr;
	int         AddrLen;
	char        RemoteAddr[32];// 地址和端口
} COMPLETION_KEY, *LP_COMPLETION_KEY;

DWORD WINAPI WorkerThread(LPVOID WorkThreadContext) {
	while (1) {
		LPOVERLAPPED lpOverlapped = NULL;
		DWORD dwIOBytes = 0;
		LP_COMPLETION_KEY lpComKey = NULL;

		bool bReturn = GetQueuedCompletionStatus(g_hIOCP,//完成端口句柄
			&dwIOBytes,//一次I/O操作所传送的字节数，如果是接受：表示一次接受了多少字节数据，如果是发送：表示一次发送了多少字节数据。
			(PULONG_PTR)&lpComKey,//当文件I/O操作完成后，用于存放与之关联的CK
			&lpOverlapped,//IOCP特定的结构体
			WSA_INFINITE);//调用者的等待时间

		if (dwIOBytes == 0) {
			cout << "[客户端 " << lpComKey->RemoteAddr << "] 断开连接..." << endl;
			closesocket(lpComKey->Socket);
			delete lpOverlapped;
			delete lpComKey;
			continue;
		}

		// 普通套接字
		//LP_IO_CONTEXT lpIOContext = (LP_IO_CONTEXT)lpOverlapped;
		LP_IO_CONTEXT lpIOContext = CONTAINING_RECORD(lpOverlapped, IO_CONTEXT, overlapped);

		if (lpIOContext->opCode == IO_READ) {// a read operation complete

			cout << "[客户端" << lpComKey->RemoteAddr << " 接受数据] 接受长度[" << dwIOBytes << "] " << lpIOContext->buffer << endl;
	
			strcat_s(lpIOContext->buffer, 1024, "爱白菜的小昆虫");
			lpIOContext->wsabuf.len = strlen(lpIOContext->buffer) + 1;
			lpIOContext->opCode = IO_OPERATION::IO_WRITE;
			lpIOContext->nBytes = 0;
			lpIOContext->dwFlags = 0;
			// 投递发送的消息
			int nRet = WSASend(
				lpComKey->Socket,
				&lpIOContext->wsabuf,
				1,
				&lpIOContext->nBytes,
				lpIOContext->dwFlags,
				&lpIOContext->overlapped,
				NULL);
			if (nRet == SOCKET_ERROR && ERROR_IO_PENDING != WSAGetLastError()) {
				cout << "WASSend Failed::Reason Code::" << WSAGetLastError() << endl;
				closesocket(lpComKey->Socket);
				delete lpIOContext;
				delete lpComKey;
				continue;
			}
			cout << "lpIOContext->nBytes " << lpIOContext->nBytes << endl;
		}
		else if (lpIOContext->opCode == IO_WRITE) { //a write operation complete

			cout << "[客户端"<< lpComKey->RemoteAddr << " 发送数据] 发送长度[" << dwIOBytes << "] " << lpIOContext->buffer << endl;
			// Write operation completed, so post Read operation.
			lpIOContext->wsabuf.len = MAX_BUFFER_LEN;
			lpIOContext->opCode = IO_OPERATION::IO_READ;
			lpIOContext->nBytes = 0;
			lpIOContext->dwFlags = 0;
			// 投递接受的消息
			int nRet = WSARecv(
				lpComKey->Socket,
				&lpIOContext->wsabuf,
				1,
				&lpIOContext->nBytes,
				&lpIOContext->dwFlags,
				&lpIOContext->overlapped,
				NULL);
			if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
				cout << "WASRecv Failed::Reason Code1::" << WSAGetLastError() << endl;
				closesocket(lpComKey->Socket);
				delete lpIOContext;
				delete lpComKey;
				continue;
			}
			cout << "lpIOContext->nBytes " << lpIOContext->nBytes << endl;
		}
	}
	return 0;
}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET) {
		cout << "创建socket失败..." << endl;
		return -1;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 监听套接字绑定地址和端口
	if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "端口绑定套接字失败..." << endl;
		return -1;
	}
	// 开始监听套接字
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "监听套接字失败..." << endl;
		return -1;
	}
	// 获取CPU核数
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int g_ThreadCount = sysInfo.dwNumberOfProcessors * 2;
	// 创建完成端口g_hIOCP
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, g_ThreadCount);

	//CreateIoCompletionPort((HANDLE)m_socket,g_hIOCP,0,0);
	// 创建工作线程池数量 CPU核数*2
	for (int i = 0; i < g_ThreadCount; ++i) {
		HANDLE hThread;
		DWORD dwThreadId;
		hThread = CreateThread(NULL, 0, WorkerThread, 0, 0, &dwThreadId);
		CloseHandle(hThread);
	}
	cout << "[服务器启动] 监听端口8888 ..." << endl;
	while (1) {
		LP_COMPLETION_KEY lpComKey = new COMPLETION_KEY;
		lpComKey->AddrLen = sizeof(SOCKADDR_IN);
		lpComKey->Socket = accept(listenSocket, (LPSOCKADDR)&lpComKey->Addr, &lpComKey->AddrLen);

		sprintf_s(lpComKey->RemoteAddr, "%s:%u", inet_ntoa(lpComKey->Addr.sin_addr), ntohs(lpComKey->Addr.sin_port));
		cout << "[客户端" << lpComKey->RemoteAddr << "连接]..." << endl;

		// 客户端套接字绑定完成端口中
		if (CreateIoCompletionPort((HANDLE)lpComKey->Socket, g_hIOCP, (ULONG_PTR)lpComKey, 0) == NULL) {
			cout << "Binding Client Socket to IO Completion Port Failed::Reason Code::" << GetLastError() << endl;
			closesocket(lpComKey->Socket);
			delete lpComKey;
		}
		else { //post a recv request
			LP_IO_CONTEXT lpIOContext = new IO_CONTEXT;
			ZeroMemory(&lpIOContext->overlapped, sizeof(lpIOContext->overlapped));
			lpIOContext->wsabuf.buf = lpIOContext->buffer;
			lpIOContext->wsabuf.len = MAX_BUFFER_LEN;
			lpIOContext->opCode = IO_READ;
			lpIOContext->nBytes = 0;
			lpIOContext->dwFlags = 0;
			// 投递一个接受消息
			int nRet = WSARecv(
				lpComKey->Socket,
				&lpIOContext->wsabuf,
				1,
				&lpIOContext->nBytes,
				&lpIOContext->dwFlags,
				&lpIOContext->overlapped,
				NULL);
			// 投递接受socket失败
			if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
				cout << "WASRecv Failed::Reason Code::" << WSAGetLastError() << endl;
				closesocket(lpComKey->Socket);
				delete lpIOContext;
				delete lpComKey;
			}
		}
	}
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}
