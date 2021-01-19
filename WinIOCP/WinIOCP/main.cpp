#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <winsock2.h>
#include <windows.h>
#include <Mswsock.h>
#include <stdio.h>
#include <string.h>
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
	IO_READ   = 1,
	IO_WRITE  = 2
};

//typedef struct _PER_SOCKET_CONTEXT {
//	SOCKET      m_Socket;
//	// ÿһ���ͻ������ӵ�Socket  
//	SOCKADDR_IN m_ClientAddr;
//	// �ͻ��˵ĵ�ַ  
//	CArray<_PER_IO_CONTEXT*> m_arrayIoContext;
//	// �ͻ���������������������ݣ�  
//}PER_SOCKET_CONTEXT, *LP_PER_SOCKET_CONTEXT;

typedef struct _IO_CONTEXT {
	OVERLAPPED   Overlapped;   // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)
	CHAR         Buffer[MAX_BUFFER_LEN];// �����WSABUF�������ַ��Ļ�����  
	IO_TPYE      OpCode;       // ��ʶ�������������(��Ӧ�����ö��)
	SOCKET       AcceptSocket;
} IO_CONTEXT, *LP_IO_CONTEXT;

char sz[] = "255.255.255.255:65535";

typedef struct _COM_KEY {
	SOCKET Socket;// �����׽���
	CHAR   RemoteAddr[32];// ��ַ�Ͷ˿�
} COM_KEY, *LP_COM_KEY;

typedef struct _DATA {
	SOCKET ListenSocket;// �����׽���
	CHAR   RemoteAddr[32];// ��ַ�Ͷ˿�
	HANDLE HIOCP;// ��ɶ˿ھ��
} DATA, *LP_DATA;

// �����ٽ���������
//list<LP_IO_CONTEXT> g_listIOContext;
//list<LP_COM_KEY> g_listComKey;


// ��ʼ��
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

// ������ɶ˿�
BOOL AssociationCompletionPort(LP_DATA lpData, LP_COM_KEY lpComKey) {
	// �׽��ֹ�����ɶ˿�
	if (CreateIoCompletionPort((HANDLE)lpComKey->Socket, lpData->HIOCP, (ULONG_PTR)lpComKey, 0) == NULL) {
		cout << "�׽��ְ���ɶ˿�ʧ��..." << WSAGetLastError() << endl;
		return FALSE;
	}
	//g_listComKey.push_back(lpComKey);
	return TRUE;
}

// Ͷ����������
BOOL PostAccept(LP_DATA lpData) {
	DWORD nBytesRecv = 0, dwFlags = 0;
	LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
	lpIOContext->OpCode = IO_ACCPET;
	int len = sizeof(SOCKADDR_IN);

	lpIOContext->AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	// Ͷ��һ���ͻ�������
	BOOL nRet = AcceptEx(
		lpData->ListenSocket,
		lpIOContext->AcceptSocket,
		lpIOContext->Buffer,
		0,
		len + 16,
		len + 16,
		&nBytesRecv,
		&lpIOContext->Overlapped);
	// Ͷ�ݽ���socketʧ��
	if (!nRet && ERROR_IO_PENDING != WSAGetLastError()) {
		cout << "AcceptEx ʧ�� " << WSAGetLastError() << endl;
		return FALSE;
	}
	return TRUE;
}

// Ͷ�ݷ�������
BOOL PostSend(LP_COM_KEY lpComKey, PCCH buffer, ULONG dwSizeBytes) {
	DWORD nBytesSend = 0, dwFlags = 0;
	LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
	lpIOContext->OpCode = IO_WRITE;

	WSABUF wsabuf = { 0 };
	wsabuf.buf = lpIOContext->Buffer;
	wsabuf.len = dwSizeBytes;
	// Ͷ�ݷ��͵���Ϣ
	int nRet = WSASend(
		lpComKey->Socket,
		&wsabuf,
		1,
		&nBytesSend,
		dwFlags,
		&lpIOContext->Overlapped,
		NULL);
	// Ͷ�ݽ�������ʧ��ʧ��
	if (nRet == SOCKET_ERROR && ERROR_IO_PENDING != WSAGetLastError()) {
		printf("[�ͻ���%s] Ͷ��WSASendʧ��... �����룺%d\n", lpComKey->RemoteAddr, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

// Ͷ�ݽ�������
BOOL PostRecv(LP_COM_KEY lpComKey) {
	DWORD nBytesRecv = 0, dwFlags = 0;
	LP_IO_CONTEXT lpIOContext = new IO_CONTEXT();
	lpIOContext->OpCode = IO_READ;

	WSABUF wsabuf = { 0 };
	wsabuf.buf = lpIOContext->Buffer;
	wsabuf.len = MAX_BUFFER_LEN;
	// Ͷ��һ��������Ϣ
	int nRet = WSARecv(
		lpComKey->Socket,
		&wsabuf,
		1,
		&nBytesRecv,
		&dwFlags,
		&lpIOContext->Overlapped,
		NULL);
	// Ͷ�ݽ�������ʧ��ʧ��
	if (nRet == SOCKET_ERROR && ERROR_IO_PENDING != WSAGetLastError()) {
		printf("[�ͻ���%s] Ͷ��WSARecvʧ��... �����룺%d\n", lpComKey->RemoteAddr, WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}


DWORD WINAPI WorkerThread(LPVOID lpParam) {
	LP_DATA lpData = (LP_DATA)lpParam;

	LP_IO_CONTEXT lpIOContext;
	DWORD dwTrans;
	LP_COM_KEY lpComKey;
	BOOL bIORet;

	//InterlockedIncrement(&threadNums);

	while (TRUE) {
		lpIOContext = NULL;
		dwTrans = 0;
		lpComKey = NULL;

		bIORet = GetQueuedCompletionStatus(
			lpData->HIOCP,//��ɶ˿ھ��
			&dwTrans,//һ��I/O���������͵��ֽ���������ǽ��ܣ���ʾһ�ν����˶����ֽ����ݣ�����Ƿ��ͣ���ʾһ�η����˶����ֽ����ݡ�
			(PULONG_PTR)&lpComKey,//���ļ�I/O������ɺ����ڴ����֮������CK
			(LPOVERLAPPED*)&lpIOContext,//IOCP�ض��Ľṹ��
			WSA_INFINITE);//�����ߵĵȴ�ʱ��

		if (dwTrans == 0 && (lpIOContext->OpCode == IO_READ || lpIOContext->OpCode == IO_WRITE)) {
			printf("[�ͻ��� %s] �Ͽ�����...\n", lpComKey->RemoteAddr);
			closesocket(lpComKey->Socket);
			delete lpComKey;
			delete lpIOContext;
			continue;
		}

		// ��ͨ�׽���
		//LP_IO_CONTEXT lpIOContext = (LP_IO_CONTEXT)lpOverlapped;
		//LP_IO_CONTEXT lpIOContext = CONTAINING_RECORD(lpOverlapped, IO_CONTEXT, Overlapped);
		if (bIORet && lpIOContext && lpComKey) {
			if (lpIOContext->OpCode == IO_READ) {
				printf("[���� �ͻ���%s ����] �������ݳ���[%d] ���ܵ�����[%s]\n", lpComKey->RemoteAddr, dwTrans, lpIOContext->Buffer);
				PostSend(lpComKey, "�������յ���Ϣ", 15);
			}
			else if (lpIOContext->OpCode == IO_WRITE) {
				printf("[���� �ͻ���%s ����] �������ݳ���[%d]\n", lpComKey->RemoteAddr, dwTrans);
				PostRecv(lpComKey);
			}
			else if (lpIOContext->OpCode == IO_ACCPET) {
				QueueUserWorkItem(WorkerThread, lpParam, WT_EXECUTELONGFUNCTION);
				printf("[�ͻ�������]...\n");

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
				printf("[�ͻ���%s]���ӳɹ�\n", lpComKeyT->RemoteAddr);


				AssociationCompletionPort(lpData, lpComKeyT);
				PostRecv(lpComKeyT);

				PostAccept(lpData);
			}
		}
		else if (!lpComKey && !lpIOContext) {
			// exit the thread
		}
	}
	//InterlockedDecrement(&threadNums);
	printf("�߳��˳�...\n");
	return 0;
}

int main() {
	// �����ж��ź�������
	// ..............

	WinSockInit();

	// ������ɶ˿�
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hIOCP == NULL) {
		cout << "������ɶ˿�ʧ��..." << endl;
		return -1;
	}
	// �����׽���
	SOCKET ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET) {
		printf("[������] �����׽���ʧ�ܣ������룺%d...\n", WSAGetLastError());
		return -1;
	}

	SOCKADDR_IN Addr = {0};
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(8888);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �����׽��ְ󶨵�ַ�Ͷ˿�
	if (bind(ListenSocket, (LPSOCKADDR)&Addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		printf("[������] �׽��ְ󶨵�ַ�Ͷ˿�ʧ�ܣ������룺%d...\n", WSAGetLastError());
		return -1;
	}
	// ��ʼ�����׽���
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("[������] �����׽���ʧ�ܣ������룺%d...\n", WSAGetLastError());
		return -1;
	}

	LP_DATA lpData = new DATA();
	lpData->HIOCP = hIOCP;
	lpData->ListenSocket = ListenSocket;
	sprintf(lpData->RemoteAddr, "%s:%d", inet_ntoa(Addr.sin_addr), ntohs(Addr.sin_port));
	printf("[������%s] ����...\n", lpData->RemoteAddr);
	LP_COM_KEY lpComKey = new COM_KEY();
	lpComKey->Socket = ListenSocket;
	sprintf(lpComKey->RemoteAddr, "%s:%d", inet_ntoa(Addr.sin_addr), ntohs(Addr.sin_port));

	QueueUserWorkItem(WorkerThread, (LPVOID)lpData, WT_EXECUTELONGFUNCTION);
	// �׽��ֹ�����ɶ˿�
	AssociationCompletionPort(lpData, lpComKey);
	// Ͷ�ݽ��ܿͻ���
	PostAccept(lpData);

	while (TRUE) {
		Sleep(1000);
	}
	closesocket(ListenSocket);
	WSACleanup();
	delete lpData;
	delete lpComKey;
	return 0;
}
