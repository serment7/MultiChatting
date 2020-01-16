#pragma once
#include"Constants.h"
#include"ListenSocket.h"
#include"ClientSocket.h"

const BYTE nMajor = 2, nMinor = 2;
const WORD wVersionRequested = MAKEWORD(nMinor, nMajor);

class CSocketManager
{
private:
	WSADATA wsaData;
	CListenSocket * listenSocket;
	CClientSocket * client[CLIENT_MAX];
public:
	CSocketManager();
	~CSocketManager();
	void init();
	void release();
	void makeChatting();
	void aceeptClient();
	void joinChatting(DWORD);
	void sendMessage(TCHAR *);
	void disconnect(int);
	void run();
	TCHAR * getIP();
	void recvMessage(TCHAR*);
};
DWORD WINAPI acceptThread(LPVOID);
DWORD WINAPI clientThread(LPVOID);