#pragma once
#include "Socket.h"
class CClientSocket :
	public CSocket
{
private:
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void init(DWORD, WORD);
	virtual void release();
};