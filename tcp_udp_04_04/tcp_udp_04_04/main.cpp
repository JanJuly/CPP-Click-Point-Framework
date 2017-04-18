#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <WS2tcpip.h>
#include "entity.h"
#include <winsock.h>

#define PORT "20000"

using namespace std;
int main()
{
	int nResult;
	WSADATA wsData;

	struct addrinfo info;
	struct addrinfo *pResult;

	nResult = WSAStartup(MAKEWORD(2, 2), &wsData);

	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;
	info.ai_flags = AI_PASSIVE;

	nResult = getaddrinfo(NULL, PORT, &info, &pResult);

	SOCKET Listener;
	Listener = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);

	nResult = bind(Listener, pResult->ai_addr, (int)pResult->ai_addrlen);

	nResult = listen(Listener, SOMAXCONN);

	SOCKET Client;

	Client = accept(Listener, NULL, NULL);


	

	return 0;

}