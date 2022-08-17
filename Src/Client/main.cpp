#include "pch.h"
static ST_WSA_INIALIZER g_WsaInitializer;

const WORD g_wPort = 33481;

int main(void)
{
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_un_b = { 127, 0, 0, 1 };
	remote.sin_port = htons(g_wPort);
	if (0 != connect(clientSock, (sockaddr*)&remote, sizeof(remote)))
		return -1;

	const char* pszMessage = "Hello world";
	send(clientSock, pszMessage, strlen(pszMessage), 0);

	char szBuffer[100];
	int nReadSize = recv(clientSock, szBuffer, 100, 0);
	szBuffer[nReadSize] = 0;

	printf("client recived : %s\n", szBuffer);
	closesocket(clientSock);
	return 0;

}