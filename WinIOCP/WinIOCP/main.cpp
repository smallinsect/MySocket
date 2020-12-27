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
//	// ÿһ���ͻ������ӵ�Socket  
//	SOCKADDR_IN m_ClientAddr;
//	// �ͻ��˵ĵ�ַ  
//	CArray<_PER_IO_CONTEXT*> m_arrayIoContext;
//	// �ͻ���������������������ݣ�  
//}PER_SOCKET_CONTEXT, *LP_PER_SOCKET_CONTEXT;

typedef struct _IO_CONTEXT {
	OVERLAPPED   overlapped;   // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)
	WSABUF       wsabuf;       // WSA���͵Ļ����������ڸ��ص�������������
	char         buffer[MAX_BUFFER_LEN];// �����WSABUF�������ַ��Ļ�����  
	IO_OPERATION opCode;       // ��ʶ�������������(��Ӧ�����ö��)
	DWORD        nBytes;
	DWORD        dwFlags;
} IO_CONTEXT, *LP_IO_CONTEXT;

char sz[] = "255.255.255.255:65535";

typedef struct _COMPLETION_KEY {
	SOCKET      Socket;
	SOCKADDR_IN Addr;
	int         AddrLen;
	char        RemoteAddr[32];// ��ַ�Ͷ˿�
} COMPLETION_KEY, *LP_COMPLETION_KEY;

DWORD WINAPI WorkerThread(LPVOID WorkThreadContext) {
	while (1) {
		LPOVERLAPPED lpOverlapped = NULL;
		DWORD dwIOBytes = 0;
		LP_COMPLETION_KEY lpComKey = NULL;

		bool bReturn = GetQueuedCompletionStatus(g_hIOCP,//��ɶ˿ھ��
			&dwIOBytes,//һ��I/O���������͵��ֽ���������ǽ��ܣ���ʾһ�ν����˶����ֽ����ݣ�����Ƿ��ͣ���ʾһ�η����˶����ֽ����ݡ�
			(PULONG_PTR)&lpComKey,//���ļ�I/O������ɺ����ڴ����֮������CK
			&lpOverlapped,//IOCP�ض��Ľṹ��
			WSA_INFINITE);//�����ߵĵȴ�ʱ��

		if (dwIOBytes == 0) {
			cout << "[�ͻ��� " << lpComKey->RemoteAddr << "] �Ͽ�����..." << endl;
			closesocket(lpComKey->Socket);
			delete lpOverlapped;
			delete lpComKey;
			continue;
		}

		// ��ͨ�׽���
		//LP_IO_CONTEXT lpIOContext = (LP_IO_CONTEXT)lpOverlapped;
		LP_IO_CONTEXT lpIOContext = CONTAINING_RECORD(lpOverlapped, IO_CONTEXT, overlapped);

		if (lpIOContext->opCode == IO_READ) {// a read operation complete

			cout << "[�ͻ���" << lpComKey->RemoteAddr << " ��������] ���ܳ���[" << dwIOBytes << "] " << lpIOContext->buffer << endl;
	
			strcat_s(lpIOContext->buffer, 1024, "���ײ˵�С����");
			lpIOContext->wsabuf.len = strlen(lpIOContext->buffer) + 1;
			lpIOContext->opCode = IO_OPERATION::IO_WRITE;
			lpIOContext->nBytes = 0;
			lpIOContext->dwFlags = 0;
			// Ͷ�ݷ��͵���Ϣ
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

			cout << "[�ͻ���"<< lpComKey->RemoteAddr << " ��������] ���ͳ���[" << dwIOBytes << "] " << lpIOContext->buffer << endl;
			// Write operation completed, so post Read operation.
			lpIOContext->wsabuf.len = MAX_BUFFER_LEN;
			lpIOContext->opCode = IO_OPERATION::IO_READ;
			lpIOContext->nBytes = 0;
			lpIOContext->dwFlags = 0;
			// Ͷ�ݽ��ܵ���Ϣ
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
		cout << "����socketʧ��..." << endl;
		return -1;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �����׽��ְ󶨵�ַ�Ͷ˿�
	if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "�˿ڰ��׽���ʧ��..." << endl;
		return -1;
	}
	// ��ʼ�����׽���
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "�����׽���ʧ��..." << endl;
		return -1;
	}
	// ��ȡCPU����
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int g_ThreadCount = sysInfo.dwNumberOfProcessors * 2;
	// ������ɶ˿�g_hIOCP
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, g_ThreadCount);

	//CreateIoCompletionPort((HANDLE)m_socket,g_hIOCP,0,0);
	// ���������̳߳����� CPU����*2
	for (int i = 0; i < g_ThreadCount; ++i) {
		HANDLE hThread;
		DWORD dwThreadId;
		hThread = CreateThread(NULL, 0, WorkerThread, 0, 0, &dwThreadId);
		CloseHandle(hThread);
	}
	cout << "[����������] �����˿�8888 ..." << endl;
	while (1) {
		LP_COMPLETION_KEY lpComKey = new COMPLETION_KEY;
		lpComKey->AddrLen = sizeof(SOCKADDR_IN);
		lpComKey->Socket = accept(listenSocket, (LPSOCKADDR)&lpComKey->Addr, &lpComKey->AddrLen);

		sprintf_s(lpComKey->RemoteAddr, "%s:%u", inet_ntoa(lpComKey->Addr.sin_addr), ntohs(lpComKey->Addr.sin_port));
		cout << "[�ͻ���" << lpComKey->RemoteAddr << "����]..." << endl;

		// �ͻ����׽��ְ���ɶ˿���
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
			// Ͷ��һ��������Ϣ
			int nRet = WSARecv(
				lpComKey->Socket,
				&lpIOContext->wsabuf,
				1,
				&lpIOContext->nBytes,
				&lpIOContext->dwFlags,
				&lpIOContext->overlapped,
				NULL);
			// Ͷ�ݽ���socketʧ��
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
