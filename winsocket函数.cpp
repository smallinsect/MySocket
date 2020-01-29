


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


