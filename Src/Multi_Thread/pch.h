#ifndef PCH_H
#define PCH_H
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

struct ST_WSA_INIALIZER
{
	ST_WSA_INIALIZER(void)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	~ST_WSA_INIALIZER(void)
	{
		WSACleanup();
	}
};