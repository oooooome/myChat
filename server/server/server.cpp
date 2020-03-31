#include <iostream>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WORD w_req = MAKEWORD(2, 2);
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "初始化套接字库失败！" << endl;
    }
    
    int len_1 = 0;
    int len_2 = 0;
    char send_buf[100];
    char recv_buf_1[100];
    char recv_buf_2[100];
    char index;
    SOCKET s_server;
    SOCKET s_accept_1;
    SOCKET s_accept_2;
    SOCKADDR_IN server_addr;
    SOCKADDR_IN accept_addr_1;
    SOCKADDR_IN accept_addr_2;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(30396);
    s_server = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(s_server, (SOCKADDR*)& server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        cout << "套接字绑定失败" << endl;
        WSACleanup();
    }
    if (listen(s_server, SOMAXCONN) < 0)
    {
        cout << "套接字绑定失败" << endl;
        WSACleanup();
    }
    cout << "服务器开启监听" << endl;
    len_1 = sizeof(SOCKADDR);
    s_accept_1 = accept(s_server, (SOCKADDR*)& accept_addr_1, &len_1);
    if (s_accept_1 == SOCKET_ERROR)
    {
        cout << "用户1连接失败" << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        index = '0';
        send(s_accept_1, &index, 1, 0);
    }
    len_2 = sizeof(SOCKADDR);
    s_accept_2 = accept(s_server, (SOCKADDR*)&accept_addr_2, &len_2);
    if (s_accept_2 == SOCKET_ERROR) 
    {
        cout << "用户2连接失败" << endl;
        WSACleanup();
        return 0;
    }
    else
    {
        index = '1';
        send(s_accept_2, &index, 1, 0);
    }
    cout << "连接建立，准备接受数据" << endl;
    while (1)
    {
        if (recv(s_accept_1, recv_buf_1, 100, 0) < 0)
        {
            cout << "接受用户1消息失败" << endl;
            break;
        }
        else
        {
            cout << "用户1: " << recv_buf_1 << endl;
            send(s_accept_2, recv_buf_1, 100, 0);
        }
        if (recv(s_accept_2, recv_buf_2, 100, 0) < 0)
        {
            cout << "接受用户2消息失败" << endl;
            break;
        }
        else
        {
            cout << "用户2: " << recv_buf_2 << endl;
            send(s_accept_1, recv_buf_2, 100, 0);
        }
    }
    closesocket(s_server);
    closesocket(s_accept_1);
    closesocket(s_accept_2);
    WSACleanup();
    return 0;
}
