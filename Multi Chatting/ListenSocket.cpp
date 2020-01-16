#include "ListenSocket.h"


CListenSocket::CListenSocket()
{
}


CListenSocket::~CListenSocket()
{
}

void CListenSocket::init(DWORD ip, WORD port)
{
	SOCKET tempSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	ZeroMemory(&addr_in, sizeof(SOCKADDR_IN));
	addr_in.sin_addr.s_addr = htonl(ip);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	setSocket(tempSock);
	setNumber(8);
}
void CListenSocket::release()
{
	setActive(FALSE);
	closesocket(getSocket());
}
void CListenSocket::listenSocket(int nQueue)
{
	bind(getSocket(), (SOCKADDR*)&addr_in, sizeof(SOCKADDR));
	setActive(TRUE);
	listen(getSocket(), nQueue);
}
void CListenSocket::connectSocket()
{
	connect(getSocket(), (SOCKADDR*)&addr_in, sizeof(SOCKADDR));
}
