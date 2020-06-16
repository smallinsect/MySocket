# MyMFC

## windows命令

查看程序使用的端口，查看8888端口

```
netstat -aon|findstr "8888"
```

## MyServer

创建模态窗口服务器

1. winsock初始化

```
WSADATA wd;
::WSAStartup(MAKEWORD(2, 2), &wd);
```

2. 创建套接字

```
s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

3. 初始化信息

```
// 初始化信息
addr.sin_family = AF_INET;
addr.sin_port = htons(8888);
//addr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
addr.sin_addr.s_addr = INADDR_ANY;
```

4. 绑定信息

```
::bind(s, (PSOCKADDR)&addr, sizeof(addr))
```

5. 监听服务器套接字

```
::listen(s, 5)
```

```
// 将服务端的socket设为异步
::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ);
```

自定义WM_SOCKET消息

```
#define WM_SOCKET WM_USER+100

afx_msg LRESULT CMyServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CString str;
	char cs[1024] = "";
	switch (lParam) {
	case FD_ACCEPT:
	{
		int addrlen = sizeof(addr1);
		s1 = ::accept(s, (PSOCKADDR)&addr1, &addrlen);
		n = n + 1;
		str.Format(TEXT("有%d个客户端连接... %s:%d\r\n"), 
			n, ::inet_ntoa(addr1.sin_addr), ::ntohs(addr1.sin_port));
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
		break;
	}
	case FD_READ:
	{
		::recv(s1, cs, 1024, 0);
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str);
		str += cs;
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
		break;
	}
	}
	return 0;
}
```

## MyClient

创建模态窗口客户端

1. 初始化


```
WSADATA wd;
::WSAStartup(MAKEWORD(2, 2), &wd);
```

2. 创建套接字

```
// 创建套接字
s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

3. 连接服务器

```
addr.sin_family = AF_INET;
addr.sin_port = htons(8888);
addr.sin_addr.s_addr = inet_addr("127.0.0.1");

::connect(s, (PSOCKADDR)&addr, sizeof(addr))
```

4. 写数据异步

```
::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_READ);
```

自定义WM_SOCKET消息

```
#define WM_SOCKET WM_USER+100

afx_msg LRESULT CMyClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	case FD_READ:
	{
		char cs[1024] = "";
		recv(s, cs, 1024, 0);
		CString str;
		GetDlgItem(IDC_EDIT_MSG)->GetWindowText(str);
		str += "\r\n服务器说：";
		str += (LPTSTR)cs;
		GetDlgItem(IDC_EDIT_MSG)->SetWindowText(str);
		break;
	}
	}
	return 0;
}
```

# Winsock

初始化

```
WSADATA wd;
::WSAStartup(MAKEWORD(2, 2), &wd);
```

查看socket错误

```
int n = ::WSAGetLastError();
```

套接字信息

	sockaddr_in saddr = {0};//定义套接字地址结构变量
	saddr.sin_family = AF_INET;//制定地址家族为TCP/IP
	saddr.sin_port = htons(80);//制定端口号
	saddr.sin_addr.s_addr = inet_addr("0.0.0.0");//将字符串IP转换为网络字节顺序排列的IP
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
addr.sin_port = htons(port);

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

宽字符CString转char

```
CString str1;
GetDlgItem(IDC_EDIT_IP)->GetWindowText(str1);
char sz1[1024];
DWORD dwNum=WideCharToMultiByte(CP_OEMCP, NULL, str1.GetBuffer(0), -1, NULL, NULL, 0, NULL);
memset(sz1, 0, sizeof(sz1));
WideCharToMultiByte(CP_OEMCP, NULL, str1.GetBuffer(0), -1, sz1, dwNum, 0, NULL);
```

char 转CString

```
char szBuff[50] = { 0 };
CString str(szBuff);  
```



