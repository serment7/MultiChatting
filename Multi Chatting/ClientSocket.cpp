#include "ClientSocket.h"


CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::init(DWORD p_ip,WORD p_PORT)
{
}

void CClientSocket::release()
{
	setActive(FALSE);
	closesocket(getSocket());
}