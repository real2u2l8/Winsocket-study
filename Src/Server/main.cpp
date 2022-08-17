/*
*	멀티쓰레드 작업 해주어야지 출력도 됨. 
*/
#include "pch.h"
static ST_WSA_INIALIZER g_WsaInitializer; //WSA init 선언

const WORD g_wPort = 33481;
SOCKET g_listenSock;
int main(void)
{
	g_listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(g_wPort);
	bind(g_listenSock, (sockaddr*)&service, sizeof(service));
	listen(g_listenSock, 10); //뒤에 10은 backlog queue 갯수 < 조정해야됨.
	
	sockaddr_in remoteInfo = { 0, };
	int nInfoLen = (int)sizeof(remoteInfo);
	SOCKET connectionSock;
	while (INVALID_SOCKET != (connectionSock = accept(g_listenSock, 
																		(sockaddr*)&remoteInfo, &nInfoLen)))
	{
		char szBuffer[1500 + 1];
		int nReadSize = recv(connectionSock, szBuffer, 1500, 0);
		szBuffer[nReadSize] = 0;

		printf("server recived : %s\n", szBuffer);

		send(connectionSock, szBuffer, nReadSize, 0);
		shutdown(connectionSock, SD_BOTH);
		closesocket(connectionSock);
	}
	return 0;
}
	