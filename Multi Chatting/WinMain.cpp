#pragma once
#include"Service.h"

CService* service;

extern HINSTANCE g_hInst;
extern HWND g_hSend;
extern HWND g_hRecv;

extern TCHAR * chatBuffer;
extern TCHAR sendBuffer[256];
extern TCHAR recvBuffer[256];
extern int g_nTreadCount;
extern BOOL bRun;
extern TCHAR strLimit[10];
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	bRun = TRUE;
	g_nTreadCount = 0;

	g_hInst = hInstance;
	MSG msg = { 0, };
	service = new CService;
	service->init();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
		{	
			static BOOL bShiftDown=FALSE;
			if (msg.message == WM_SYSKEYDOWN)
			{
				if (GetKeyState(VK_F4) & 0x8000)
					strcpy(sendBuffer, QUIT_CODE);
			}
			if (msg.message == WM_KEYDOWN)
			{
				if (msg.hwnd == g_hSend)
				{
					switch (msg.wParam)
					{
					case VK_SHIFT:
						bShiftDown = TRUE;
						continue;
					case VK_RETURN:
						if (bShiftDown)
						{
						}
						else
						{
							GetWindowText(g_hSend, sendBuffer, BUFFER_MAX - 1);
							SetWindowText(g_hSend, TEXT(""));
							wsprintf(strLimit, "(%d/%d)", 0, BUFFER_MAX - 1);
							InvalidateRect(GetParent(g_hSend), NULL, TRUE);
							continue;
						}
					}
				}
			}
			else if(msg.message == WM_KEYUP)
			{
				switch (msg.wParam)
				{
				case VK_SHIFT:
					bShiftDown = FALSE;
					continue;
				}
			}
			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		}
		else{
			service->run();
			Sleep(20);
		}
	}
	bRun = FALSE;
	service->release();
	delete service;
	return msg.wParam;
}