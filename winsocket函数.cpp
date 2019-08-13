


//根据socket获取ip和端口 getsockname函数
sockaddr_in addr;
int addrLen = sizeof(addr);
if (getsockname(s, (sockaddr *)&addr, &addrLen) != 0) {
    printf("[server] getsockname error ...\n");
    return -2;
}
printf("[server] getsockname success ...\n");
printf("ip: %s\n", inet_ntoa(addr.sin_addr));
printf("port: %d\n", ntohs(addr.sin_port));

//获取本机ip和协议 getaddrinfo
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

//创建套接字
SOCKET sockServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
if (sockServer == INVALID_SOCKET) {
    printf("[server] socket error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

//套接字绑定协议、端口、IP地址
if (bind(sockServer,  result->ai_addr,  result->ai_addrlen) == SOCKET_ERROR) {
    printf("[server] bind error ...\n");
    freeaddrinfo(result);
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";

freeaddrinfo(result);//释放资源

//普通方法创建服务端
//创建套接字
SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if (sockServer == INVALID_SOCKET) {
    cout << "[server] socket error ...\n";
    system("pause");
    return -1;
}
cout << "[server] socket success ...\n";

sockaddr_in saddrServer = {0};
saddrServer.sin_family = AF_INET;//协议IPV4
saddrServer.sin_port = htons(8080);//端口8080
//saddrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP地址127.0.0.1

//套接字绑定协议、端口、IP地址
if (bind(sockServer, (sockaddr *)&saddrServer, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    cout << "[server] bind error ...\n";
    system("pause");
    return -1;
}
cout << "[server] bind success ...\n";


