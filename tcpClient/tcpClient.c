#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")      //引入动态链接库
#pragma warning(disable:4996) 
#define SERVER_IP "127.0.0.1"        //客户端IP
//#define SERVER_IP "10.2.0.1" 

int main1()
{
    WORD ws_version = MAKEWORD(2, 2);   //指定Winsock version
    WSADATA wsaData;                    //WSA 函数的参数

    /*初始化winsock*/
    WSAStartup(ws_version, &wsaData);
    char buf[1000];
    while (1)
    {
        /*socket*/
        SOCKET s_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        SOCKADDR_IN addr_server;
        addr_server.sin_family = AF_INET;   //协议
        addr_server.sin_port = htons(5050); //端口
        addr_server.sin_addr.s_addr = inet_addr(SERVER_IP);

        char buf[100];
        int send_status, recv_status;

        /*Connect*/
        int cnct_status = connect(s_client, (SOCKADDR*)&addr_server, sizeof(SOCKADDR));
        if (cnct_status == 0)//连接成功
        {
            printf("\nConnecting... done\n");
            recv_status = recv(s_client, buf, 100, 0);
            printf("Received: ");
            buf[recv_status] = 0x00;
            printf(buf);
        

            scanf("%s", buf);
            //向服务端发送消息
            send_status = send(s_client, buf, 10, 0);
            if (send_status == SOCKET_ERROR)//发送失败
            {
                printf("send error!\n");
            }
            else
            {
                printf("Sending  - %s\n", buf);
                //接受服务端消息
                recv_status = recv(s_client, buf, 100, 0);
                buf[recv_status] = 0x00;//在字符串结尾填上 结束符 0x00 == /0 参考回答
                printf("Received - %s\n", buf);
            }
        }
        else
        {
            printf("Test:fail to connect server! \n");
        }
        closesocket(s_client);
    }

    WSACleanup();

    return 0;
}