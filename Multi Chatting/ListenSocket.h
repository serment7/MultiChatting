#pragma once
#include "Socket.h"
class CListenSocket :
	public CSocket
{
private:
public:
	CListenSocket();
	virtual ~CListenSocket();
	virtual void init(DWORD, WORD);
	virtual void release();
	void listenSocket(int);
	void connectSocket();
};
