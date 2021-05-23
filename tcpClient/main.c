#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)
#define SERVER_IP "127.0.0.1" 

void main()
{
			
	int port = 59000;						// Listening port # on the server

	// Initialize WinSock

	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		printf("Can't start Winsock, Err  #%d", wsResult);
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("Can't create socket, Err # %d", WSAGetLastError());
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr= inet_addr(SERVER_IP);

	// Connect to server
	int connResult = connect(sock, (SOCKADDR*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		printf("Can't connect to server # %d", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	char userInput[100];

	do
	{
		// Prompt the user for some text
		printf( "> ");
		scanf("%s", userInput);
		if (sizeof(userInput) > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput, sizeof(userInput), 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					printf("SERVER> %s \n", buf);
				}
			}
			else
			{
				printf("send failed with error: %d\n", WSAGetLastError());
			}
		}

	} while (sizeof(userInput)> 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}