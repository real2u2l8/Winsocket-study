#include "pch.h"

static ST_WSA_INIALIZER g_WsaInitializer; //WSA init 선언
const WORD g_wPort = 33481;
SOCKET g_listenSock;

DWORD WINAPI Th_Service(void*) { //서버 쓰레드
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

DWORD WINAPI Th_Client(void *) //클라이언트 쓰레드
{
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_un_b = { 127, 0, 0, 1 };
	remote.sin_port = htons(g_wPort);
	if (0 != connect(clientSock, (sockaddr*)&remote, sizeof(remote)))
		return -1;

	const char* pszMessage = "Hello this is client!";
	send(clientSock, pszMessage, strlen(pszMessage), 0);

	char szBuffer[100];
	int nReadSize = recv(clientSock, szBuffer, 100, 0);
	szBuffer[nReadSize] = 0;

	printf("client recived : %s\n", szBuffer);
	closesocket(clientSock);
	return 0;

}

int main(void) 
{
	HANDLE server_Thread = CreateThread(nullptr, 0, Th_Service, 0, 0, nullptr);
	Sleep(500);
	HANDLE client_Thread = CreateThread(nullptr, 0, Th_Client, 0, 0, nullptr);

	DWORD client_dwRet = WaitForSingleObject(client_Thread, INFINITE);
	closesocket(g_listenSock);
	//DWORD server_dwRet = WaitForSingleObject(server_Thread, INFINITE);	

	return 0;
}