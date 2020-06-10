# MySocket
用于学习socket编程，记录的代码

网络报文的数据结构格式

网络数据报文的格式定义

select模型处理多个客户端

# MyServer
一个简单的web服务器

# MyServerFile
用于获取本地文件路径和文件，发送给浏览器的服务器。

#Web消息请求和响应的规则
https://www.runoob.com/http/http-header-fields.html



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
s = ::socket(AF_INET, SOCK_STREAM, 0);
::WSAAsyncSelect(s, this->m_hWnd, WM_SOCKET, FD_READ);
```

MFC函数使用

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









DemoTcp01Client01 简单模板客户端
DemoTcp01Client02

DemoTcp01Server01 简单模板服务端
DemoTcp01Server02

DemoTcp02Client01 select模型的客户端
DemoTcp02Server01 select模型的服务端

Demo01UdpClient
    简单模板客户端
Demo01UdpServer
    简单模板服务端
Demo02UdpClient
    广播接收数据
Demo02UdpServer
    广播发送数据


缓冲区溢出、无法发送、网络阻塞

粘包 少包

应用层
运输层
    为两台主机上的应用程序处理端对端
网络层
    处理分组在网络中的活动
链路层
    处理与电缆

```
getpeername(skt, (sockaddr *)&addr, &addrLen);
sockaddr_in addr;
int addrLen = sizeof(addr);
if (getsockname(s, (sockaddr *)&addr, &addrLen) != 0) {
    printf("[server] getsockname error ...\n");
    return -2;
}
printf("[server] getsockname success ...\n");
printf("ip: %s\n", inet_ntoa(addr.sin_addr));
printf("port: %d\n", ntohs(addr.sin_port));
addrinfo hints, *result = NULL;
ZeroMemory(&hints, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_flags = AI_PASSIVE;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_socktype = SOCK_STREAM;

iResult = getaddrinfo(NULL, "8080", &hints, &result);
if (iResult != 0) {
    printf("[server] getaddrinfo error %d ...\n", iResult);
    system("pause");
    return -1;
}
printf("[server] getaddrinfo success ...\n");

SOCKET sockServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
if (sockServer == INVALID_SOCKET) {
    printf("[server] socket error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

if (bind(sockServer,  result->ai_addr,  result->ai_addrlen) == SOCKET_ERROR) {
    printf("[server] bind error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";

freeaddrinfo(result);

SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if (sockServer == INVALID_SOCKET) {
    cout << "[server] socket error ...\n";
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

sockaddr_in saddrServer = {0};
saddrServer.sin_family = AF_INET;
saddrServer.sin_port = htons(8080);
//saddrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

if (bind(sockServer, (sockaddr *)&saddrServer, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    cout << "[server] bind error ...\n";
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";
```

查看所有服务器的端口
tasklist /svc | find "MyServer."

查看程序端口
netstat -ano | find "14960"
5.查看端口是否被监听
netstat –ntl

查看远程端口是否监听
telnet 10.0.144.59 8080
telnet ip地址 端口号
Telnet不是内部或外部命令，控制面板->程序和功能->启动或关闭windows功能->找到telnet客户端，打上勾就可以了。

socket的bind，如果没有设置ip，自动绑定本机ip。

tcp使用场景
    对数据安全性要求高的时候
        登录数据的传输
        文件传输
    http协议
        传输层协议-tcp
udp使用场景
    效率高-实时性要求比较高
        视频聊天
        通话
    有实力的大公司
        使用udp
        在应用层自定义协议，做数据校验

广播
    服务器 只发数据 广播地址 发送到固定端口
        创建套接字-socket
        fd绑定服务器IP和端口
        初始化客户端ip和端口信息
            sockaddr_in cli;
            cli.sin_family = af_inet;
            cli.sin_port = htons(8080);
            inet_pton(af_net, "xxx.xxx.123.255", &cli.adr);
        发送数据
            sendto(fd, buf, len, 0, (sockaddr *)&cli, sizeof(cli))
        设置广播权限
            int flag = 1;
            setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
    客户端 收数据
        创建套接字
        显示绑定ip和端口
            bind()
        接受数据-server数据
            recvfrom(fd,);
    适用范围
        只使用局域网

广播地址：xxx.xxx.123.255
网关:xxx.xxx.123.1
整个广播地址：255.255.255.255

组播
    使用范围：
        局域网
        internet
    结构体
        struct ip_mreqn{
            //组播组的IP地址
            struct in_addr imr_multiaddr;
            //本地某一网络设备接口的IP地址
            struct in_addr imr_interface;
            int imr_ifindex;//网卡编号
        };
        struct in_addr{
            in_addr_t s_addr;
        };
    server
        只发送数据
        需要使用组播地址 发送到客户端对应的端口上
        添加组播权限 setsockopt
    client
        绑定固定端口
        加入到组播地址 setsockopt
组播地址
224.0.0.0~224.0.0.255
    预留的组播地址（永久组地址），地址224.0.0.0保留
    不做分配，其他地址供路由协议使用；
224.0.1.0~224.0.1.255
    公用组播地址，可以用于Internet；欲使用需申请。
224.0.2.0~238.255.255.255
    用户可用的组播地址（临时组地址），全网范围内有效；
239.0.0.0~239.255.255.255
    本地管理组播地址，仅在特定的本地范围内有效。
服务器端操作
客户端操作

if_nametoindex获取网卡编号

1、tcp状态转换
2、select FD_ZERO(fd_set*); FD_SET(fd, fd_set*); FD_ISSET(fd, fd_set*); FD_CLR(fd_set*);
    fd_set-1024
    select(maxfd+1, &reads, &write, &expt, timeout);
        timeout=NULL永久阻塞
        struct timeval val = {1,0};阻塞1秒
        reads-传入传出参数
    思路
        //监听
        //将待检测的数据初始化到fd_set集合中
        while(true){//循环的委托内核检测
            select();
            //判断fd是否是监听的
            //已经连接的客户端发送来的数据
            for(){
                IS_ISSET();
            }
        }


epoll三个函数
该函数生成一个epoll专用的文件描述符
    int epoll_create(int size);
        size：epoll上能关注的最大描述符数
用于控制某个epoll文件描述符事件，可以注册、修改、删除
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    参数
        epfd:epoll_create生成的epoll专用描述符
        op:
            EPOLL_CTL_ADD--注册
            EPOLL_CTL_MOD--修改
            EPOLL_CTL_DEL--删除
        fd:关联的文件描述符
        event:告诉内核要监听什么事件
等待IO事件发生-可以设置阻塞的函数
    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
    参数：
        epfd:要检测的句柄
        events:用于回传待处理事件的数组
        maxevents:告诉内核这个events的大小
        timeout:为超时时间
            -1:永久阻塞
            0:立即返回
            >0:

typedef union epoll_data{
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;
struct epoll_event{
    uint32_t events;
    epoll_data_t data;
};
events:
-EPOLLIN-读
-EPOLLOUT-写
-EPOLLERR-异常

epoll三种工作模式
1、水平触发模式
2、边沿触发模式
3、边沿费阻塞触发

本地套接字
    1、文件格式：
        管道：p
        套接字：s
        伪文件
    2、服务器端
        创建套接字
            socket(AF_LOCAL, SOCK_STREAM, 0);
        绑定-struct sockaddr_un serv
            serv.sun_family = AF_LOCAL;
            strcpy(serv.sun_paht, "server.socket");--现在不存在的文件
            bind(lfd, (struct sockaddr*)&serv, len);--绑定成功套接字文件被创建
        设置监听
            listen();
        等待接收连接请求
            struct sockaddr_un client;
            int len = sizeof(client);
            int cfd = accept(ldf, &client, &len);
        通信
            send 
            recv
        断开连接
            close(cfd);
            close(lfd);
    客户端
        创建套接字 
            int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        绑定一个套接字文件
            struct sockaddr_un client;
            client.sun_family = AF_LOCAL;
            strcpy(client.sun_path, "client.socket");
            bind(fd, (struct sockaddr*)&client, len);
        连接服务器
            struct sockaddr_un serv
            serv.sun_family = AF_LOCAL;
            strcpy(serv.sun_paht, "server.socket");--现在不存在的文件
            bind(lfd, (struct sockaddr*)&serv, len);--绑定成功套接字文件被创建
            connect(fd, &serv, sizeof(serv));
        通信
            recv 
            send 
        关闭

            close
=======
## Linux的Tcp修改缓存

内核设置的套接字缓存

```
/proc/sys/net/core/rmem_default，net.core.rmem_default，套接字接收缓存默认值 (bit)
/proc/sys/net/core/wmem_default，net.core.wmem_default，套接字发送缓存默认值 (bit)
/proc/sys/net/core/rmem_max，net.core.rmem_max，套接字接收缓存最大值 (bit)
/proc/sys/net/core/wmem_max，net.core.wmem_max，发送缓存最大值 (bit)
tcp缓存
/proc/sys/net/ipv4/tcp_rmem：net.ipv4.tcp_rmem，接收缓存设置，依次代表最小值、默认值和最大值(bit)
4096    87380    4194304
/proc/sys/net/ipv4/tcp_wmem：net.ipv4.tcp_wmem，发送缓存设置，依次代表最小值、默认值和最大值(bit)
/proc/sys/net/ipv4/tcp_mem:
94500000    915000000    927000000
对应net.ipv4.tcp_mem，tcp整体缓存设置，对所有tcp内存使用状况的控制，单位是页，依次代表TCP整体内存的无压力值、压力模式开启阀值、最大使用值，用于控制新缓存的分配是否成功
tcp或者udp的设置会覆盖内核设置。其中只有tcp_mem是用于tcp整体内存的控制，其他都是针对单个连接的。
```
修改配置

```
sysctl -w net.core.rmem_max=8388608
...
sysctl -w net.ipv4.tcp_mem='8388608 8388608 8388608'
```

估算TCP缓存大小

```
以tcp接收缓存为例（实际上发送窗口=对方的接收窗口），tcp接收缓存有2部分组成：接收窗口及应用缓存，应用缓存用于应用的延时读及一些调度信息。linux使用net.ipv4.tcp_adv_win_scale（对应文件/proc/sys/net/ipv4/tcp_adv_win_scale）指出应用缓存的比例。
if tcp_adv_win_scale > 0: 应用缓存 = buffer / (2^tcp_adv_win_scale)，tcp_adv_win_scale默认值为2，表示缓存的四分之一用于应用缓存，可用接收窗口占四分之三。
if tcp_adv_win_scale <= 0: 应用缓存 = buffer - buffer/2^(-tcp_adv_win_scale)，即接收窗口=buffer/2^(-tcp_adv_win_scale)，如果tcp_adv_win_scale=-2，接收窗口占接收缓存的四分之一。
那如果能估算出接收窗口就能算出套接字缓存的大小。如何算接收窗口呢？
BDP(bandwidth-delay product，带宽时延积) = bandwith(bits/sec) * delay(sec)，代表网络传输能力，为了充分利用网络，最大接收窗口应该等于BDP。delay = RTT/2。
receive_win = bandwith * RTT / 2
buffer = rec_win/(3/4) (上面知道tcp_adv_win_scale=2时表示接收窗口占buffer的3/4
以我们的机房为例，同机房的带宽为30Gbit/s，两台机器ping可获得RTT大概为0.1ms，那BDP=(30Gb/1000) * 0.1 / 2 = 1.5Mb，buffer = 1.5Mb * 4 / 3 = 2Mb
```

TCP缓存的综合考虑

```
如第三节我们真的能设置tcp最大缓存为2Mb吗？通常一台机器会部署多个服务，一个服务内部也往往会建立多个tcp连接。但系统内存是有限的，如果有4000个连接，满负荷工作，达到最大窗口。那么tcp整体消耗内存=4000 * 2Mb = 1GB。
并发连接越多，默认套接字缓存越大，则tcp占用内存越大。当套接字缓存和系统内存一定时，会影响并发连接数。对于高并发连接场景，系统资源不足，缩小缓存限制；并发连接少时，可以适当放大缓存限制。
linux自身引入了自动调整接收缓存的功能，来使吞吐量最大，(缓存最大值还是受限于tcp_rmem[2])。配置项如下。
net.ipv4.tcp_moderate_rcvbuf = 1 (/proc/sys/net/ipv4/tcp_moderate_rcvbuf)
```

**系统TCP连接内存大小限制**

- TCP的每一个连接请求，读写都需要占用系统内存资源，可根据系统配置，对TCP连接数，内存大小，限制调优。

------

**查看系统内存资源**

```
记录内存 详情：cat /proc/meminfo
命令：free -m
```

------

**TCP连接内存限制 内核调优**

**修改文件：/etc/sysctl.conf**

**生效命令：sysctl -p /etc/sysctl.conf**

**1、限制TCP总连接内存使用**

```
# 内核分给TCP的内存大小范围，单位为page。
# 第一个数字表示，当 tcp 使用的 page 少于 196608 时，kernel 不对其进行任何的干预
# 第二个数字表示，当 tcp 使用了超过 262144 的 pages 时，kernel 会进入 “memory pressure” 压力模式
# 第三个数字表示，当 tcp 使用的 pages 超过 393216 时就会报：Out of socket memory。
net.ipv4.tcp_mem = 196608 262144 393216
```

注：最大范围可根据系统内存大小进行设置。
注：内核分配给TCP连接的内存，单位是Page，1 Page = 4096 Bytes。
注：查看分配给TCP连接得内存：getconf PAGESIZE

**2、限制TCP单连接读写内存使用**

```
# TCP连接读缓存大小。最小内存 缺省内存 最大内存（单位为：Byte字节）
net.ipv4.tcp_rmem = 4096 4096 1073741824

# TCP连接写缓存大小。最小内存 缺省内存 最大内存（单位为：Byte字节）
net.ipv4.tcp_wmem = 4096 4096 1073741824
```

注:4.0G TCP内存能容纳的连接数读写相加： 4000M/8 = 500K = 50万并发
注:TCP请求读写缓存可跟据程序平均请求资源大小设置。

**如何永久性修改TCP缓冲区的大小：在 \**/etc/sysctl.conf\** 修改如下内容：**

```
net.ipv4.tcp_mem = 6177504 8236672 16777216
net.ipv4.tcp_rmem = 4096 873800 16777216
net.ipv4.tcp_wmem = 4096 873800 16777216
```

**然后执行 \**/sbin/sysctl –p\** 使之生效，修改完后再查看**

```
king@ubuntu:~$ sudo /sbin/sysctl -p
[sudo] king 的密码： 
net.ipv4.tcp_mem = 6177504 8236672 16777216
net.ipv4.tcp_rmem = 4096 873800 16777216
net.ipv4.tcp_wmem = 4096 873800 16777216
```
>>>>>>> 6cc589b471d0bc01e7474f50e6d940b86ee1f341

