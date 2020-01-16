#include"Socket.h"

CSocket::CSocket()
{

}
CSocket::~CSocket()
{

}
void CSocket::setSocket(SOCKET p_sock)
{
	sock = p_sock;
}
SOCKET CSocket::getSocket()
{
	return sock;
}
BOOL CSocket::isActive()
{
	return bActive;
}

void CSocket::setActive(BOOL p_active)
{
	bActive = p_active;
}
void CSocket::setAddr(SOCKADDR_IN p_addr)
{
	addr_in = p_addr;
}

SOCKADDR_IN CSocket::getAddr()
{
	return addr_in;
}
int CSocket::getNumber()
{
	return num;
}
void CSocket::setNumber(int p_num)
{
	num = p_num;
}
void CSocket::release()
{
	closesocket(sock);
	setActive(FALSE);
}
SOCKET CSocket::getClient()
{
	return client;
}

void CSocket::recvClient(SOCKET p_client)
{
	client = p_client;
}