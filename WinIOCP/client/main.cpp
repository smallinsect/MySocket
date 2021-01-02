#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI SendThread(LPVOID lpParam);// 发送数据线程
DWORD WINAPI RecvThread(LPVOID lpParam);// 接受数据线程

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        printf("[客户端] 创建套接字失败，错误码：%d\n", GetLastError());
        return -1;
    }


    int flag = connect(ClientSocket, (sockaddr*)&addr, sizeof(addr));
    if (flag < 0) {
        cout << "error!" << endl;
        return -1;
    }
    printf("[连接服务器] 成功...\n");

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

// 发送数据线程
DWORD WINAPI SendThread(LPVOID lpParam) {
    SOCKET ClientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        scanf("%s", buffer);
        //strcpy(buffer, "hello");
        iRet = send(ClientSocket, buffer, strlen(buffer) + 1, 0);
        if (iRet <= 0) {
            printf("[服务器] 断开...\n");
            break;
        }
        printf("发送数据长度[%d]\n", iRet);
    }

    return 0;
}
// 接受数据线程
DWORD WINAPI RecvThread(LPVOID lpParam) {
    SOCKET ClientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        iRet = recv(ClientSocket, buffer, 1024, 0);
        if (iRet <= 0) {
            printf("[服务器] 断开...\n");
            break;
        }
        printf("[服务器] 数据长度[%d]\n", iRet);
        printf("[服务器] 接受数据[%s]\n", buffer);
    }
    return 0;
}