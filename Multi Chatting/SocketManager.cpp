#include "SocketManager.h"

extern HWND g_hRecv;
int g_nTreadCount;
BOOL bAccept;
BOOL bRun;
TCHAR recvBuffer[256];
CRITICAL_SECTION g_cs;
//STCLIENT stClient;
BOOL bReceived;
TCHAR userName[USERNAME_MAX];
int nCode;
extern HWND g_hUserList;
BOOL bNameChange;

CSocketManager::CSocketManager()
{
}
CSocketManager::~CSocketManager()
{
}
void CSocketManager::init()
{
	WSAStartup(wVersionRequested, &wsaData);
	int count = 0;
	nCode = -1;
	g_nTreadCount = 0;
	bAccept = FALSE;
	bReceived = TRUE;
	while (count < CLIENT_MAX)
	{
		client[count] = nullptr;
		++count;
	}
	listenSocket = nullptr;
}
void CSocketManager::release()
{
	int count = 0;
	EnterCriticalSection(&g_cs);
	while (count < CLIENT_MAX)
	{
		disconnect(count);
		++count;
	}
	if (listenSocket != nullptr)
	{
		listenSocket->release();
		listenSocket = nullptr;
	}
	LeaveCriticalSection(&g_cs);
	WSACleanup();
}
void CSocketManager::sendMessage(TCHAR * buf)
{
	
	EnterCriticalSection(&g_cs);
	
	if (listenSocket->getNumber() == SERVER_NUMBER)
	{
		send(listenSocket->getSocket(), buf, strlen(buf), 0);
	}
	else
	{
		int count = 0;
		while (count<CLIENT_MAX)
		{
			if (client[count]&&client[count]->getNumber()!=nCode)
				send(client[count]->getSocket(), buf, strlen(buf), 0);
			++count;
		}
		nCode = -1;
	}
	LeaveCriticalSection(&g_cs);
}
void CSocketManager::recvMessage(TCHAR * recvbuf)
{
	EnterCriticalSection(&g_cs);
	if (!bReceived)
	{
		strcpy(recvbuf, recvBuffer);
		bReceived = TRUE;
		if (listenSocket->getNumber() == SERVER_NUMBER)
		{
			if (!strcmp(recvbuf, QUIT_CODE))
			{
				if (listenSocket != nullptr)
				{
					listenSocket->setActive(FALSE);
				}
				PostQuitMessage(0);
			}
			else if (strcmp(recvBuffer,""))
			{
				LeaveCriticalSection(&g_cs);
				return;
			}
		}
		else
		{
			sendMessage(recvbuf);
		}

	}
	else
	{
		recvbuf = nullptr;
	}
	LeaveCriticalSection(&g_cs);
	
}
void CSocketManager::makeChatting()
{
	bAccept = FALSE;
	listenSocket = new CListenSocket;
	listenSocket->init(INADDR_ANY, PORT);
	listenSocket->listenSocket(CLIENT_MAX);
	listenSocket->setNumber(-1);
	CreateThread(nullptr, 0, acceptThread, (LPVOID)listenSocket, 0, nullptr);
}

void CSocketManager::aceeptClient()
{
	EnterCriticalSection(&g_cs);
	if (bAccept)
	{
		int count = 0;
		while ((client[count]!=nullptr)&&(count < CLIENT_MAX))
		{
			++count;
		}
		client[count] = new CClientSocket;
		client[count]->setSocket(listenSocket->getClient());
		client[count]->setActive(TRUE);
		client[count]->setNumber(count);
		CreateThread(nullptr, 0, clientThread, (LPVOID)client[count], 0, nullptr);
		++g_nTreadCount;
		bAccept = FALSE;
	}
	LeaveCriticalSection(&g_cs);

}

void CSocketManager::disconnect(int bannum)
{
	if (client[bannum])
		client[bannum]->setActive(FALSE);
}
void CSocketManager::joinChatting(DWORD ip)
{
	TCHAR str[35];
	listenSocket = new CListenSocket;
	listenSocket->init(ip,PORT);
	listenSocket->setNumber(SERVER_NUMBER);
	listenSocket->setActive(TRUE);
	listenSocket->connectSocket();
	client [0] = new CClientSocket;
	client[0]->setSocket(listenSocket->getSocket());
	CreateThread(nullptr, 0, clientThread, (LPVOID)client[0] , 0, nullptr);
	++g_nTreadCount;
	wsprintf(str, "%s이(가) 접속했습니다", userName);
	send(listenSocket->getSocket(), str, strlen(str),0);
}

void CSocketManager::run()
{
	aceeptClient();
}

TCHAR * CSocketManager::getIP()
{
	IN_ADDR addr;
	HOSTENT * host;
	TCHAR tempHostName[256];
	gethostname(tempHostName, 255);
	host = gethostbyname(tempHostName);
	addr.s_addr = *((u_long*)host->h_addr_list[0]);
	return inet_ntoa(addr);
}

DWORD WINAPI acceptThread(LPVOID p_listenSocket)
{
	int cbAddr = sizeof(SOCKADDR);
	SOCKET acceptClient;
	CListenSocket * listenSocket = (CListenSocket*)p_listenSocket;
	while (bRun)
	{
		EnterCriticalSection(&g_cs);
		if (!bAccept&&(g_nTreadCount < 7))
		{
			LeaveCriticalSection(&g_cs);
			acceptClient=accept(listenSocket->getSocket(), (SOCKADDR*)&listenSocket->getAddr(), &cbAddr);
			EnterCriticalSection(&g_cs);
			if (acceptClient != INVALID_SOCKET)
			{
				listenSocket->recvClient(acceptClient);
				bAccept = TRUE;
			}
		}
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}

DWORD WINAPI clientThread(LPVOID p_client)
{
	EnterCriticalSection(&g_cs);
	CSocket * client = (CSocket*)p_client;

	while (client->isActive())
	{
		LeaveCriticalSection(&g_cs);
		EnterCriticalSection(&g_cs);
		if (bReceived) //메세지를 받을 준비가 되었거나 처리된 상태라면
		{
			LeaveCriticalSection(&g_cs);
			ZeroMemory(recvBuffer, sizeof(TCHAR)*BUFFER_MAX-1);
			recv(client->getSocket(), recvBuffer, BUFFER_MAX - 1, 0);

			//메세지를 받으면 처리되지 않은 메세지가 있음을 알린다
			EnterCriticalSection(&g_cs);
			nCode = client->getNumber();
			bReceived = FALSE;

			//클라이언트 종료 메세지라면 쓰레드를 종료한다
			if (!strcmp(recvBuffer, QUIT_CODE))
			{
				--g_nTreadCount;
				break;
			}
		}
	}
	LeaveCriticalSection(&g_cs);

	client->release();
	delete client;
	client = nullptr;
	return 0;
}
