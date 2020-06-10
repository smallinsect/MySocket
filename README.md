# MySocket
用于学习socket编程，记录的代码

网络报文的数据结构格式

网络数据报文的格式定义

select模型处理多个客户端


#MyServer
一个简单的web服务器

#MyServerFile
用于获取本地文件路径和文件，发送给浏览器的服务器。

#Web消息请求和响应的规则
https://www.runoob.com/http/http-header-fields.html



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

getpeername(skt, (sockaddr *)&addr, &addrLen);
//鏍规嵁socket鑾峰彇ip鍜岀��鍙� getsockname鍑芥暟
sockaddr_in addr;
int addrLen = sizeof(addr);
if (getsockname(s, (sockaddr *)&addr, &addrLen) != 0) {
    printf("[server] getsockname error ...\n");
    return -2;
}
printf("[server] getsockname success ...\n");
printf("ip: %s\n", inet_ntoa(addr.sin_addr));
printf("port: %d\n", ntohs(addr.sin_port));

//鑾峰彇鏈�鏈篿p鍜屽崗璁� getaddrinfo
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

//鍒涘缓濂楁帴瀛�
SOCKET sockServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
if (sockServer == INVALID_SOCKET) {
    printf("[server] socket error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

//濂楁帴瀛楃粦瀹氬崗璁�銆佺��鍙ｃ€両P鍦板潃
if (bind(sockServer,  result->ai_addr,  result->ai_addrlen) == SOCKET_ERROR) {
    printf("[server] bind error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";

freeaddrinfo(result);//閲婃斁璧勬簮

//鏅�閫氭柟娉曞垱寤烘湇鍔＄��
//鍒涘缓濂楁帴瀛�
SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if (sockServer == INVALID_SOCKET) {
    cout << "[server] socket error ...\n";
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

sockaddr_in saddrServer = {0};
saddrServer.sin_family = AF_INET;//鍗忚�甀PV4
saddrServer.sin_port = htons(8080);//绔�鍙�8080
//saddrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP鍦板潃127.0.0.1

//濂楁帴瀛楃粦瀹氬崗璁�銆佺��鍙ｃ€両P鍦板潃
if (bind(sockServer, (sockaddr *)&saddrServer, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    cout << "[server] bind error ...\n";
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";

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

