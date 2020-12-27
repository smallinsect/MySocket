#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI SendThread(LPVOID lpParam);// 发送数据线程
DWORD WINAPI RecvThread(LPVOID lpParam);// 接受数据线程

int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int flag = connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
    if (flag < 0) {
        cout << "error!" << endl;
        return -1;
    }
    cout << "[连接服务器] 成功..." << endl;

    DWORD dwThreadIdRecv = 0;
    DWORD dwThreadIdSend = 0;
    HANDLE hRecv = CreateThread(NULL, 0, RecvThread, (LPVOID)clientSocket, 0, &dwThreadIdRecv);
    HANDLE hSend = CreateThread(NULL, 0, SendThread, (LPVOID)clientSocket, 0, &dwThreadIdSend);

    while (1) {

        Sleep(1000);
    }

    closesocket(clientSocket);
    WSACleanup();

    system("pause");
    return 0;
}

// 发送数据线程
DWORD WINAPI SendThread(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        cin >> buffer;
        //strcpy(buffer, "hello");
        iRet = send(clientSocket, buffer, strlen(buffer) + 1, 0);
        if (iRet <= 0) {
            cout << "[服务器] 断开连接 ..." << endl;
            break;
        }
        cout << "发送字节数：" << iRet << endl;
    }

    return 0;
}
// 接受数据线程
DWORD WINAPI RecvThread(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;

    int iRet;
    char buffer[1024];
    while (true) {
        iRet = recv(clientSocket, buffer, 1024, 0);
        if (iRet <= 0) {
            cout << "服务器断开连接..." << endl;
            break;
        }
        cout << "接受数据长度：[" << iRet << "] [服务器] " << buffer << endl;
    }
    return 0;
}