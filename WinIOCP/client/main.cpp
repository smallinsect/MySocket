#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int flag = connect(client, (sockaddr*)&addr, sizeof(addr));
    if (flag < 0) {
        cout << "error!" << endl;
        return -1;
    }
    while (1) {
        cout << "sent hello!!!!" << endl;
        char buffer[1024];
        cin >> buffer;
        //strcpy(buffer, "hello");
        send(client, buffer, 1024, 0);

        memset(buffer, 0, sizeof(buffer));

        cout << "recv: " << endl;
        int rev = recv(client, buffer, 1024, 0);
        if (rev == 0)
            cout << "recv nothing!" << endl;
        cout << buffer << endl;
        Sleep(1000);
    }

    closesocket(client);
    WSACleanup();

    return 0;
}