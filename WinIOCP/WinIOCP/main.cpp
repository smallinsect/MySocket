#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <winsock2.h>
#include <windows.h>
#include <Mswsock.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <list>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"Mswsock.lib")

#define MAX_BUFFER_LEN 1024

enum IO_TPYE {
	IO_ACCPET = 0,
	IO_READ = 1,
	IO_WRITE = 2
};

typedef struct _IO_CONTEXT {
	OVERLAPPED   Overlapped;   // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
	CHAR         Buffer[MAX_BUFFER_LEN];// 这个是WSABUF里具体存字符的缓冲区  
	IO_TPYE      OpCode;       // 标识网络操作的类型(对应上面的枚举)
	SOCKET       AcceptSocket;
} IO_CONTEXT, * LP_IO_CONTEXT;

char sz[] = "255.255.255.255:65535";

typedef struct _COM_KEY {
	SOCKET Socket;// 接受套接字
	CHAR   RemoteAddr[32];// 地址和端口
} COM_KEY, * LP_COM_KEY;

typedef struct _DATA {
	SOCKET ListenSocket;// 监听套接字
	CHAR   RemoteAddr[32];// 地址和端口
	HANDLE HIOCP;// 完成端口句柄
} DATA, * LP_DATA;

// 增加临界区互斥锁
CRITICAL_SECTION _critical;
list<LP_COM_KEY> g_listComKey;

void AddComKey(LP_COM_KEY lpComKey) {
	EnterCriticalSection(&_critical);
	g_listComKey.push_back(lpComKey);
	LeaveCriticalSection(&_critical);
}

// 初始化
BOOL WinSockInit() {
	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

// 关联完成端口
BOOL AssociationCompletionPort(LP_DATA lpData, LP_COM_KEY lpComKey) {
	// 套接字关联完成端口
	if (CreateIoCompletionPort((HANDLE)lpComKey->Socket, lpData->HIOCP, (ULONG_PTR)lpComKey, 0) == NULL) {
		cout << "套接字绑定完成端口失败..." << WSAGetLastError() << endl;
		return FALSE;
	}
	return TRUE;
}

// 投递连接请求
BOOL PostAccept(LP_DATA lpData) {
	DWORD nBytesRecv = 0;
	LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
	lpIOContext->OpCode = IO_ACCPET;
	int len = sizeof(SOCKADDR_IN);

	lpIOContext->AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	// 投递一个客户端连接
	BOOL nRet = AcceptEx(
		lpData->ListenSocket,
		lpIOContext->AcceptSocket,
		lpIOContext->Buffer,
		0,
		len + 16,
		len + 16,
		&nBytesRecv,
		&lpIOContext->Overlapped);
	// 投递接受socket失败
	if (!nRet && ERROR_IO_PENDING != WSAGetLastError()) {
		cout << "AcceptEx 失败 " << WSAGetLastError() << endl;
		return FALSE;
	}
	return TRUE;
}

// 投递发送数据
BOOL PostSend(PCCH buffer, ULONG dwSizeBytes) {
	EnterCriticalSection(&_critical);
	for (list<LP_COM_KEY>::iterator it = g_listComKey.begin(); it != g_listComKey.end(); ++it) {
		LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
		lpIOContext->OpCode = IO_WRITE;
		strcat(lpIOContext->Buffer, buffer);
		WSABUF wsabuf = { 0 };
		wsabuf.buf = lpIOContext->Buffer;
		wsabuf.len = dwSizeBytes;
		// 投递发送的消息
		DWORD nBytesSend = 0, dwFlags = 0;
		int nRet = WSASend(
			(*it)->Socket,
			&wsabuf,
			1,
			&nBytesSend,
			dwFlags,
			&lpIOContext->Overlapped,
			NULL);
	}
	LeaveCriticalSection(&_critical);
	return TRUE;
}

// 投递接受数据
BOOL PostRecv(LP_COM_KEY lpComKey) {
	DWORD nBytesRecv = 0, dwFlags = 0;
	LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
	lpIOContext->OpCode = IO_READ;

	WSABUF wsabuf = { 0 };
	wsabuf.buf = lpIOContext->Buffer;
	wsabuf.len = MAX_BUFFER_LEN;
	// 投递一个接受消息
	int nRet = WSARecv(
		lpComKey->Socket,
		&wsabuf,
		1,
		&nBytesRecv,
		&dwFlags,
		&lpIOContext->Overlapped,
		NULL);
	// 投递接受数据失败失败
	if (nRet == SOCKET_ERROR && ERROR_IO_PENDING != WSAGetLastError()) {
		printf("[客户端%s] 投递WSARecv失败... 错误码：%d\n", lpComKey->RemoteAddr, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}


DWORD WINAPI WorkerThread(LPVOID lpParam) {
	LP_DATA lpData = (LP_DATA)lpParam;

	LP_IO_CONTEXT lpIOContext;
	DWORD dwTrans;
	LP_COM_KEY lpComKey;

	//InterlockedIncrement(&threadNums);

	while (TRUE) {
		lpIOContext = NULL;
		dwTrans = 0;
		lpComKey = NULL;

		BOOL bIORet = GetQueuedCompletionStatus(
			lpData->HIOCP,//完成端口句柄
			&dwTrans,//一次I/O操作所传送的字节数，如果是接受：表示一次接受了多少字节数据，如果是发送：表示一次发送了多少字节数据。
			(PULONG_PTR)&lpComKey,//当文件I/O操作完成后，用于存放与之关联的CK
			(LPOVERLAPPED*)&lpIOContext,//IOCP特定的结构体
			WSA_INFINITE);//调用者的等待时间

		if (dwTrans == 0 && (lpIOContext->OpCode == IO_READ || lpIOContext->OpCode == IO_WRITE)) {
			printf("[客户端 %s] 断开连接...\n", lpComKey->RemoteAddr);
			closesocket(lpComKey->Socket);
			// list中移除套接字

			delete lpComKey;
			delete lpIOContext;
			continue;
		}

		if (bIORet && lpIOContext && lpComKey) {
			if (lpIOContext->OpCode == IO_READ) {
				printf("[接受 客户端%s 数据] 接受数据长度[%d] 接受的数据[%s]\n", lpComKey->RemoteAddr, dwTrans, lpIOContext->Buffer);
				PostRecv(lpComKey);
				CHAR msg[1024];
				sprintf(msg, "[%s] %s", lpComKey->RemoteAddr, lpIOContext->Buffer);
				int len = strlen(msg) + 1;
				PostSend(msg, len);
			}
			else if (lpIOContext->OpCode == IO_WRITE) {
				printf("[发送 客户端%s 数据] 发送数据长度[%d]\n", lpComKey->RemoteAddr, dwTrans);
			}
			else if (lpIOContext->OpCode == IO_ACCPET) {
				QueueUserWorkItem(WorkerThread, lpParam, WT_EXECUTELONGFUNCTION);

				PSOCKADDR_IN addrClient = NULL;
				PSOCKADDR_IN addrLocal = NULL;
				int nClientLen = sizeof(SOCKADDR_IN), nLocalLen = sizeof(SOCKADDR_IN);

				GetAcceptExSockaddrs(lpIOContext->Buffer, 0,
					sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
					(LPSOCKADDR*)&addrLocal, &nLocalLen,
					(LPSOCKADDR*)&addrClient, &nClientLen);

				LP_COM_KEY lpComKeyT = new COM_KEY();
				lpComKeyT->Socket = lpIOContext->AcceptSocket;
				sprintf(lpComKeyT->RemoteAddr, "%s:%d", inet_ntoa(addrClient->sin_addr), ntohs(addrClient->sin_port));
				printf("[客户端%s]连接成功\n", lpComKeyT->RemoteAddr);

				AddComKey(lpComKeyT);

				AssociationCompletionPort(lpData, lpComKeyT);
				PostRecv(lpComKeyT);

				PostAccept(lpData);
			}
		}
		else if (!lpComKey && !lpIOContext) {
			// exit the thread
		}
		if (lpIOContext) {
			delete lpIOContext;// 释放内存
		}
	}
	//InterlockedDecrement(&threadNums);
	printf("线程退出...\n");
	return 0;
}

void sighandler(int signum) {
	printf("退出信号%d\n", signum);
	exit(-1);
}

int main() {
	// 增加中断信号量处理
	signal(SIGINT, sighandler);
	// 初始化临界区
	InitializeCriticalSection(&_critical);

	WinSockInit();

	// 创建完成端口
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hIOCP == NULL) {
		cout << "创建完成端口失败..." << endl;
		return -1;
	}
	// 创建套接字
	SOCKET ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET) {
		printf("[服务器] 创建套接字失败，错误码：%d...\n", WSAGetLastError());
		return -1;
	}

	SOCKADDR_IN Addr = { 0 };
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(8888);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 监听套接字绑定地址和端口
	if (bind(ListenSocket, (LPSOCKADDR)&Addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		printf("[服务器] 套接字绑定地址和端口失败，错误码：%d...\n", WSAGetLastError());
		return -1;
	}
	// 开始监听套接字
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("[服务器] 监听套接字失败，错误码：%d...\n", WSAGetLastError());
		return -1;
	}

	LP_DATA lpData = new DATA();
	lpData->HIOCP = hIOCP;
	lpData->ListenSocket = ListenSocket;
	sprintf(lpData->RemoteAddr, "%s:%d", inet_ntoa(Addr.sin_addr), ntohs(Addr.sin_port));
	printf("[服务器%s] 开启...\n", lpData->RemoteAddr);
	LP_COM_KEY lpComKey = new COM_KEY();
	lpComKey->Socket = ListenSocket;
	sprintf(lpComKey->RemoteAddr, "%s:%d", inet_ntoa(Addr.sin_addr), ntohs(Addr.sin_port));

	QueueUserWorkItem(WorkerThread, (LPVOID)lpData, WT_EXECUTELONGFUNCTION);
	// 套接字关联完成端口
	AssociationCompletionPort(lpData, lpComKey);
	// 投递接受客户端
	PostAccept(lpData);

	while (TRUE) {
		Sleep(1000);
	}
	closesocket(ListenSocket);
	WSACleanup();
	delete lpData;
	delete lpComKey;
	// 删除释放临界区资源
	DeleteCriticalSection(&_critical);

	printf("Line :%d\n", __LINE__);
	return 0;
}
