# VS项目配置路径

1. 输出目录
   * $(SolutionDir)../bin/$(Platform)/$(Configuration)/
2. 中检目录
   * $(SolutionDir)../bintemp/$(Platform)/$(ProjectName)/$(Configuration)/

# WinIOCP

## 创建完成端口

```c++
//功能：创建完成端口和关联完成端口
 HANDLE WINAPI CreateIoCompletionPort(
        __in   HANDLE FileHandle,             // 已经打开的文件句柄或者空句柄，一般是客户端的句柄
        __in   HANDLE ExistingCompletionPort, // 已经存在的IOCP句柄
        __in   ULONG_PTR CompletionKey,       // 完成键，包含了指定I/O完成包的指定文件
        __in   DWORD NumberOfConcurrentThreads// 真正并发同时执行最大线程数，一般推介是CPU核心数*2
     );
```

```c++
//创建完成端口句柄
HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)；
```

## socket绑定完成端口

```c++
typedef struct{
    SOCKET socket;              //客户端socket
    SOCKADDR_STORAGE ClientAddr;//客户端地址
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
```

```c++
//与socket进行关联
CreateIoCompletionPort((HANDLE)(PerHandleData->socket), completionPort, DWORD)PerHandleData, 0);
```

## 获取完成端口状态

```c++
//功能：获取队列完成状态
/*
返回值：
调用成功，则返回非零数值，相关数据存于lpNumberOfBytes、lpCompletionKey、lpoverlapped变量中。失败则返回零值。
*/
BOOL GetQueuedCompletionStatus(
    HANDLE   CompletionPort,     //完成端口句柄
    LPDWORD   lpNumberOfBytes,    //一次I/O操作所传送的字节数
    PULONG_PTR   lpCompletionKey, //当文件I/O操作完成后，用于存放与之关联的CK
    LPOVERLAPPED   *lpOverlapped, //IOCP特定的结构体
    DWORD   dwMilliseconds);      //调用者的等待时间
```

## 用于IOCP的特点函数

```c++
//用于IOCP的特定函数
typedef struct _OVERLAPPEDPLUS{
    OVERLAPPED ol; //一个固定的用于处理网络消息事件返回值的结构体变量
    SOCKET s, sclient;
    int OpCode;  //用来区分本次消息的操作类型（在完成端口的操作里面， 是以消息通知系统，读数据/写数据，都是要发这样的 消息结构体过去的）
    WSABUF wbuf; //读写缓冲区结构体变量 
    DWORD dwBytes, dwFlags;//一些在读写时用到的标志性变量 
}OVERLAPPEDPLUS;
```

## 投递一个队列完成状态

```c++
//功能：投递一个队列完成状态
BOOL PostQueuedCompletionStatus( 
　　HANDLE CompletlonPort, //指定想向其发送一个完成数据包的完成端口对象
　　DW0RD dwNumberOfBytesTrlansferred, //指定—个值,直接传递给GetQueuedCompletionStatus 函数中对应的参数 
　　DWORD dwCompletlonKey, //指定—个值,直接传递给GetQueuedCompletionStatus函数中对应的参数
　　LPOVERLAPPED lpoverlapped, ); //指定—个值,直接传递给GetQueuedCompletionStatus函数中对应的参数
```

## 实例代码

### client.cpp

```c++
#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

void main()
{
    WSADATA wsaData;  
    WSAStartup(MAKEWORD(2,2), &wsaData);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port   = htons(6000);
    server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int flag;
    flag = connect(client, (sockaddr*)&server, sizeof(server));
    if(flag < 0){
        cout<<"error!"<<endl;
        return;
    }
    while(1){
        cout<<"sent hello!!!!"<<endl;
        char buffer[1024];
        strcpy(buffer,"hello");
        send(client, buffer, 1024, 0);

        memset(buffer, NULL, sizeof(buffer));

        cout<<"recv: "<<endl;
        int rev = recv(client, buffer, 1024, 0);
        if(rev == 0)
            cout<<"recv nothing!"<<endl;
        cout<<buffer<<endl;
        Sleep(10000);
    }

    closesocket(client);
    WSACleanup();
}
```

### server.cpp

```c++
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"kernel32.lib")

HANDLE g_hIOCP;

enum IO_OPERATION{IO_READ,IO_WRITE};

struct IO_DATA{
    OVERLAPPED                  Overlapped;
    WSABUF                      wsabuf;
    int                         nBytes;
    IO_OPERATION                opCode;
    SOCKET                      client;
};

char buffer[1024];

DWORD WINAPI WorkerThread (LPVOID WorkThreadContext) {
    IO_DATA *lpIOContext = NULL; 
    DWORD nBytes = 0;
    DWORD dwFlags = 0; 
    int nRet = 0;

    DWORD dwIoSize = 0; 
    void * lpCompletionKey = NULL;
    LPOVERLAPPED lpOverlapped = NULL;

    while(1){
        GetQueuedCompletionStatus(g_hIOCP, &dwIoSize,(LPDWORD)&lpCompletionKey,(LPOVERLAPPED *)&lpOverlapped, INFINITE);

        lpIOContext = (IO_DATA *)lpOverlapped;
        if(dwIoSize == 0)
        {
            cout << "Client disconnect" << endl;
            closesocket(lpIOContext->client);
            delete lpIOContext;
            continue;
        }

        if(lpIOContext->opCode == IO_READ) // a read operation complete
        {
            ZeroMemory(&lpIOContext->Overlapped, sizeof(lpIOContext->Overlapped));
            lpIOContext->wsabuf.buf = buffer;
            lpIOContext->wsabuf.len = strlen(buffer)+1;
            lpIOContext->opCode = IO_WRITE;
            lpIOContext->nBytes = strlen(buffer)+1;
            dwFlags = 0;
            nBytes = strlen(buffer)+1;
            nRet = WSASend(
                lpIOContext->client,
                &lpIOContext->wsabuf, 1, &nBytes,
                dwFlags,
                &(lpIOContext->Overlapped), NULL);
            if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {
                cout << "WASSend Failed::Reason Code::"<< WSAGetLastError() << endl;
                closesocket(lpIOContext->client);
                delete lpIOContext;
                continue;
            }
            memset(buffer, NULL, sizeof(buffer));
        }
        else if(lpIOContext->opCode == IO_WRITE) //a write operation complete
        {
            // Write operation completed, so post Read operation.
            lpIOContext->opCode = IO_READ; 
            nBytes = 1024;
            dwFlags = 0;
            lpIOContext->wsabuf.buf = buffer;
            lpIOContext->wsabuf.len = nBytes;
            lpIOContext->nBytes = nBytes;
            ZeroMemory(&lpIOContext->Overlapped, sizeof(lpIOContext->Overlapped));

            nRet = WSARecv(
                lpIOContext->client,
                &lpIOContext->wsabuf, 1, &nBytes,
                &dwFlags,
                &lpIOContext->Overlapped, NULL);
            if( nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) ) {
                cout << "WASRecv Failed::Reason Code1::"<< WSAGetLastError() << endl;
                closesocket(lpIOContext->client);
                delete lpIOContext;
                continue;
            } 
            cout<<lpIOContext->wsabuf.buf<<endl;
        }
    }
    return 0;
}
void main ()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET m_socket = WSASocket(AF_INET,SOCK_STREAM, IPPROTO_TCP, NULL,0,WSA_FLAG_OVERLAPPED);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(6000);
    server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    bind(m_socket ,(sockaddr*)&server,sizeof(server));

    listen(m_socket, 8);

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int g_ThreadCount = sysInfo.dwNumberOfProcessors * 2;

    g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,g_ThreadCount);

    //CreateIoCompletionPort((HANDLE)m_socket,g_hIOCP,0,0);

    for( int i=0;i < g_ThreadCount; ++i){
        HANDLE  hThread;
        DWORD   dwThreadId;
        hThread = CreateThread(NULL, 0, WorkerThread, 0, 0, &dwThreadId);
        CloseHandle(hThread);
    }

    while(1)
    {
        SOCKET client = accept( m_socket, NULL, NULL );
        cout << "Client connected." << endl;


        if (CreateIoCompletionPort((HANDLE)client, g_hIOCP, 0, 0) == NULL){
            cout << "Binding Client Socket to IO Completion Port Failed::Reason Code::"<< GetLastError() << endl;
            closesocket(client);
        }
        else { //post a recv request
            IO_DATA * data = new IO_DATA;
            memset(buffer, NULL ,1024);
            memset(&data->Overlapped, 0 , sizeof(data->Overlapped));
            data->opCode = IO_READ;
            data->nBytes = 0;
            data->wsabuf.buf  = buffer;
            data->wsabuf.len  = sizeof(buffer);
            data->client = client;
            DWORD nBytes= 1024 ,dwFlags=0;
            int nRet = WSARecv(client,&data->wsabuf, 1, &nBytes,
                &dwFlags,
                &data->Overlapped, NULL);
            if(nRet == SOCKET_ERROR  && (ERROR_IO_PENDING != WSAGetLastError())){
                cout << "WASRecv Failed::Reason Code::"<< WSAGetLastError() << endl;
                closesocket(client);
                delete data;
            }
            cout<<data->wsabuf.buf<<endl;
        }
    }
    closesocket(m_socket);
    WSACleanup();
}
```

# 获取CPU核数

	// 获取CPU核数
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int g_ThreadCount = sysInfo.dwNumberOfProcessors * 2;
	 //创建完成端口g_hIOCP
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, g_ThreadCount);
	
	CreateIoCompletionPort((HANDLE)m_socket,g_hIOCP,0,0);
	 //创建工作线程池数量 CPU核数*2
	for (int i = 0; i < g_ThreadCount; ++i) {
		HANDLE hThread;
		DWORD dwThreadId;
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)&listenComKey, 0, &dwThreadId);
		CloseHandle(hThread);
	}

# 微软帮助文档

https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex

## 信号量函数

```c++
#include <signal.h>

void sighandler(int signum) {
	printf("退出信号%d\n", signum);
	exit(-1);
}

// 增加中断信号量处理
signal(SIGINT, sighandler);
```
## 宏定义

| DATE     | 当前日期，一个以 "MMM DD YYYY" 格式表示的字符常量。 |
| -------- | :-------------------------------------------------- |
|  TIME    | 当前时间，一个以 "HH:MM:SS" 格式表示的字符常量。    |
|  FILE    | 这会包含当前文件名，一个字符串常量。                |
|  LINE    | 这会包含当前行号，一个十进制常量。                  |
|  STDC    | 当编译器以 ANSI 标准编译时，则定义为 1。            |

```c++
printf("File :%s\n", __FILE__ );
printf("Date :%s\n", __DATE__ );
printf("Time :%s\n", __TIME__ );
printf("Line :%d\n", __LINE__ );
printf("ANSI :%d\n", __STDC__ );
```

## 临界区

```c++
适用范围：它只能同步一个进程中的线程，不能跨进程同步。一般用它来做单个进程内的代码快同步,效率比较高。
相关结构：
CRITICAL_SECTION _critical
相关方法：
// 初始化临界区
InitializeCriticalSection(&_critical);
// 删除释放临界区资源
DeleteCriticalSection(&_critical);
// 进入临界区
EnterCriticalSection(&_critical);
// 离开临界区
LeaveCriticalSection(& _critical);

```












