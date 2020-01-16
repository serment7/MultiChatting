#pragma once
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>

class CSocket
{
private:

	BOOL bActive;
	int num;
	SOCKET sock, client;
protected:
	SOCKADDR_IN addr_in;
public:
	CSocket();
	virtual ~CSocket();
	void setSocket(SOCKET);
	SOCKET getSocket();
	BOOL isActive();
	void setActive(BOOL);
	void setAddr(SOCKADDR_IN);
	SOCKADDR_IN getAddr();
	void setActive();
	int getNumber();
	void setNumber(int);
	void release();
	SOCKET getClient();
	void recvClient(SOCKET);
};