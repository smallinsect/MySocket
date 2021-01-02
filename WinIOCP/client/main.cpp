#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI SendThread(LPVOID lpParam);// ���������߳�
DWORD WINAPI RecvThread(LPVOID lpParam);// ���������߳�

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        printf("[�ͻ���] �����׽���ʧ�ܣ������룺%d\n", GetLastError());
        return -1;
    }


    int flag = connect(ClientSocket, (sockaddr*)&addr, sizeof(addr));
    if (flag < 0) {
        cout << "error!" << endl;
        return -1;
    }
    printf("[���ӷ�����] �ɹ�...\n");

    DWORD dwThreadIdRecv = 0;
    DWORD dwThreadIdSend = 0;
    HANDLE hRecv = CreateThread(NULL, 0, RecvThread, (LPVOID)ClientSocket, 0, &dwThreadIdRecv);
    HANDLE hSend = CreateThread(NULL, 0, SendThread, (LPVOID)ClientSocket, 0, &dwThreadIdSend);

    while (1) {

        Sleep(1000);
    }

    closesocket(ClientSocket);
    WSACleanup();

    system("pause");
    return 0;
}

// ���������߳�
DWORD WINAPI SendThread(LPVOID lpParam) {
    SOCKET ClientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        scanf("%s", buffer);
        //strcpy(buffer, "hello");
        iRet = send(ClientSocket, buffer, strlen(buffer) + 1, 0);
        if (iRet <= 0) {
            printf("[������] �Ͽ�...\n");
            break;
        }
        printf("�������ݳ���[%d]\n", iRet);
    }

    return 0;
}
// ���������߳�
DWORD WINAPI RecvThread(LPVOID lpParam) {
    SOCKET ClientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        iRet = recv(ClientSocket, buffer, 1024, 0);
        if (iRet <= 0) {
            printf("[������] �Ͽ�...\n");
            break;
        }
        printf("[������] ���ݳ���[%d]\n", iRet);
        printf("[������] ��������[%s]\n", buffer);
    }
    return 0;
}