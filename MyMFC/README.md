# MyMFC

## MyServer

创建模态窗口服务器



## MyClient

创建模态窗口客户端





# Winsock

套接字信息

	sockaddr_in saddr = {0};//定义套接字地址结构变量
	saddr.sin_family = AF_INET;//制定地址家族为TCP/IP
	saddr.sin_port = htons(80);//制定端口号
	saddr.sin_addr.s_addr = inet_addr("0.0.0.0");//将字符串IP转换为网络字节顺序排列的IP
字节转换函数

```

```

Windows套接字

```
CSocket::CSocket();
1.创建套接字
CSocket *sock = new CSocket();
2.绑定地址信息
BOOL Bind(const SOCKADDR *lpSockAddr, int nSockAddrLen);

sock.Bind((SOCKADDR*)addr, sizeof(addr));
3.监听客户端的连接请求
BOOL Listen(int nConnectionBacklog = 5);// 5表示只能监听5个套接字发送请求

4.连接服务器
BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
5.数据交换
//发送数据
virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
//接受数据
virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
6.关闭套接字对象
virtual void Close();
```

需要配置

```
创建VC应用程序时，需要指定Windows Socket，否则必须手动添加该类头文件afxsock.h。
```

引入动态库

```
WS2_32.DLL

WSADATA wd;
WSAStartup(MAKEWORD(2, 2), &wd)
```



```
客户端
```



UDP传输

```
//发送函数
int sendto(
	SOCKET s,//套接字句柄
	const char FAR* buf,//数据缓冲
	int len,//数据的长度
	int flags,//一般设置为0
	const struct sockaddr FAR* to,//目标地址结构信息
	int tolen//目标地址结构大小
);
//接受函数
int recvfrom(
	SOCKET s,
	char FAR* buf,
	int len,
	int flags,
	struct sockaddr FAR* from,
	int FAR* fromlen
);
```

将套接字设为异步模式

```
#define WM_SOCKET WM_USER+100//自定义消息

// 创建套接字
s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 将IDC_EDIT_MSG控件设为读取socket消息。
::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_READ);
```

socket头文件

```
#include <afxsock.h>
```





## MFC函数使用

控件不能使用

```
GetDlgItem(IDC_TEXT)->EndbleWindow(false);//禁用消息显示框
GetDlgItem(IDC_SENDTEXT)->EndbleWindow(false);//禁用发送消息编辑框
GetDlgItem(IDC_SEND)->EndbleWindow(false);//禁用发送消息按钮
```

获取控件信息

```
CString str, str1;
int port;
GetDlgItem(IDC_ADDR)->GetWindowText(str);//获取服务器地址
GetDlgItem(IDC_PORT)->GetWindowText(str);//获取端口号

port = atoi(str1.GetBuffer(1));//将端口字符转换整数
//赋值
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr(str.GetBuffer(1));
addr.sin_port = ntohs(port);

```

给控件设置

```
GetDlgItem(IDC_TEXT)->SetWindowText(str);//控件赋值
```

服务器

```
::bind(s, (sockaddr*)&addr, sizeof(addr));
::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT|FD_READ);
::listen(s, 5);
```