#include"Service.h"
TCHAR sendBuffer[256];
TCHAR receivedBuffer[256];
HWND g_hSend, g_hRecv;
TCHAR chatBuffer[10000];

HINSTANCE g_hInst;
CSceneManager * sceneManager;
CSocketManager * socketManager;
extern CRITICAL_SECTION g_cs;

extern TCHAR userName[USERNAME_MAX];

void CService::checkBuffer()
{
	if (strlen(sendBuffer))
	{
		if (!strcmp(sendBuffer, QUIT_CODE))
		{			
			TCHAR str[35];
			wsprintf(str, "%s이(가) 퇴장했습니다", userName);
			m_socketManager->sendMessage(str);
			m_socketManager->release();
			PostQuitMessage(0);
			return;
		}
		else
		{
			TCHAR str[BUFFER_MAX];
			wsprintf(str, "%s : %s", userName, sendBuffer);
			m_socketManager->sendMessage(str);
			strcat(chatBuffer, str);
			strcat(chatBuffer, TEXT("\r\n"));
			SetWindowText(g_hRecv, chatBuffer);
		}
	}
	m_socketManager->recvMessage(receivedBuffer);
	if (strlen(receivedBuffer))
	{
			strcat(chatBuffer, receivedBuffer);
			strcat(chatBuffer, "\r\n");
			SetWindowText(g_hRecv, chatBuffer);
	}
	ZeroMemory(&sendBuffer, BUFFER_MAX - 1);
	ZeroMemory(&receivedBuffer, BUFFER_MAX - 1);
}

CService::CService()
{

}

CService::~CService()
{

}

void CService::init()
{
	InitializeCriticalSection(&g_cs);
	WNDCLASSEX wcx{ 0, };
	wcx.cbClsExtra = 0;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.cbWndExtra = 0;
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hIconSm = NULL;
	wcx.hInstance = g_hInst;
	wcx.lpfnWndProc = StartProc;
	wcx.lpszClassName = "StartWindow";
	wcx.lpszMenuName = NULL;
	wcx.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wcx);

	wcx.lpszClassName = "GetIPWindow";
	wcx.lpfnWndProc = GetIPProc;
	RegisterClassEx(&wcx);

	wcx.lpszClassName = "ChattingWindow";
	wcx.lpfnWndProc = ChattingProc;
	RegisterClassEx(&wcx);

	wcx.lpszClassName = "EditWindow";
	wcx.lpfnWndProc = EditProc;
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RegisterClassEx(&wcx);

	m_sceneManager = new CSceneManager;
	sceneManager = m_sceneManager;
	m_sceneManager->init();
	m_sceneManager->beginScene(ID_FIRST_SCENE);

	m_socketManager = new CSocketManager;
	socketManager = m_socketManager;
	m_socketManager->init();
}

void CService::release()
{
	m_sceneManager->endScene();

	delete sceneManager;
	delete m_socketManager;
	DeleteCriticalSection(&g_cs);
}

void CService::run()
{
	m_socketManager->run();
	checkBuffer();
}


LRESULT CALLBACK StartProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCreate, hJoin, hExit,hName;
	static TCHAR str[256] = { 0, };
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		strcpy(userName,"User");
		hName = CreateWindow("edit", userName, WS_VISIBLE | WS_CHILD | WS_BORDER, (rt.right >> 1) - 75, (rt.bottom >> 1) - 120, 150, 20,
			hWnd, (HMENU)0, g_hInst, NULL);
		SendMessage(hName, EM_LIMITTEXT, (WPARAM)USERNAME_MAX-1, 0);
		hCreate = CreateWindow("button", "채팅 만들기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, (rt.right >> 1) - 75, (rt.bottom >> 1) - 80, 150, 30, hWnd, (HMENU)ID_BUTTON_CREATE, g_hInst, sceneManager);
		hJoin = CreateWindow("button","채팅 참여하기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, (rt.right >> 1) - 75, (rt.bottom >> 1) - 30, 150, 30, hWnd, (HMENU)ID_BUTTON_JOIN, g_hInst, NULL);
		hExit = CreateWindow("button", "종료하기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, (rt.right >> 1) - 75, (rt.bottom >> 1) + 20, 150, 30, hWnd, (HMENU)ID_BUTTON_EXIT, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_CREATE:
			GetWindowText(hName, userName, USERNAME_MAX - 1);
			socketManager->makeChatting();
			sceneManager->changeScene(ID_MAKE_CHATTING_SCENE);
			break;
		case ID_BUTTON_JOIN:
			GetWindowText(hName, userName, USERNAME_MAX - 1);
			sceneManager->changeScene(ID_GET_IP_SCENE);
			break;
		case ID_BUTTON_EXIT:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_SYSKEYDOWN:
		switch (wParam)
		{
		case VK_F4:
			PostQuitMessage(0);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;


	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK GetIPProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND hIPEdit, hJoin, hReturn;
	INITCOMMONCONTROLSEX icex;
	static DWORD dwAddress;
	static TCHAR str[256];

	RECT rt;
	switch (uMsg)
	{
	case WM_CREATE:
		icex.dwICC = ICC_INTERNET_CLASSES;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&icex);
		GetClientRect(hWnd, &rt);
		hIPEdit = CreateWindow(WC_IPADDRESS, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
			(rt.right >> 1) - 75, (rt.bottom >> 1) - 90, 150, 30, hWnd, (HMENU)NULL, g_hInst, NULL);
		hJoin = CreateWindow("button", "참여하기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON,
			(rt.right >> 1) - 75, (rt.bottom >> 1) - 30, 150, 30, hWnd, (HMENU)ID_BUTTON_JOIN, g_hInst, NULL);
		hReturn = CreateWindow("button", "처음으로", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, (rt.right >> 1) - 75, (rt.bottom >> 1) + 30, 150, 30, hWnd, (HMENU)ID_BUTTON_RETURNFIRST, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_JOIN:
			SendMessage(hIPEdit, IPM_GETADDRESS, 0, (LPARAM)&dwAddress);
			socketManager->joinChatting(dwAddress);
			sceneManager->changeScene(ID_MAKE_CHATTING_SCENE);
			break;
		case ID_BUTTON_RETURNFIRST:
			sceneManager->changeScene(ID_FIRST_SCENE);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChattingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditWindow, hChatEdit, hUserList;
	static HWND hChangeName, hFileUpload, hReturn, hExit;
	static TCHAR chLimit[10];
	static TCHAR strIP[30] = "IP : ";
	RECT rt;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		hEditWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "EditWindow", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
			20, rt.bottom - 160, 350, 140, hWnd, (HMENU)NULL, g_hInst, NULL);
		hChatEdit = CreateWindowEx(0L, "edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL |ES_MULTILINE| ES_READONLY, 20, 40, 350, 330,
			hWnd, (HMENU)ID_CHATEDIT, g_hInst, NULL);
		SendMessage(hUserList, LB_ADDSTRING, 0, (LPARAM)userName);
		/*hFileUpload = CreateWindow("button", "파일 업로드", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, 400, rt.bottom - 140, 150, 30, hWnd, (HMENU)ID_BUTTON_UPLOAD, g_hInst, NULL);*/
		hReturn = CreateWindow("button", "채팅 나가기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, 400, rt.bottom - 100, 150, 30, hWnd, (HMENU)ID_BUTTON_RETURNFIRST, g_hInst, NULL);
		hExit = CreateWindow("button", "종료하기", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON
			, 400, rt.bottom - 60, 150, 30, hWnd, (HMENU)ID_BUTTON_EXIT, g_hInst, NULL);
		g_hRecv = hChatEdit;
		strcat(strIP, socketManager->getIP());
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 20, 20, strIP,strlen(strIP));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_SAVE:
			break;
		case ID_BUTTON_UPLOAD:
			break;
		case ID_BUTTON_RETURNFIRST:
			socketManager->release();
			socketManager->init();
			sceneManager->changeScene(ID_FIRST_SCENE);
			break;
		case ID_BUTTON_EXIT:
			strcpy(sendBuffer, QUIT_CODE);
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
TCHAR strLimit[10];
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	static BOOL bEditEnabled;
	static TCHAR strText[BUFFER_MAX];
	
	static TCHAR strFile[BUFFER_MAX];
	static int nLenText;

	switch (uMsg)
	{
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		GetClientRect(hWnd, &rt);
		hEdit = CreateWindowEx(0L, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL|ES_MULTILINE , 0, 0, rt.right, rt.bottom - 20,
			hWnd, (HMENU)ID_EDIT, g_hInst, NULL);
		g_hSend = hEdit;
		bEditEnabled = FALSE;
		SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)BUFFER_MAX, 0);
		nLenText = 0;
		wsprintf(strLimit, "(%d/%d)", nLenText, BUFFER_MAX - 1);
		rt.top = rt.bottom - 20;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDIT:
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
				GetWindowText(hEdit, strText, BUFFER_MAX);
				nLenText = lstrlen(strText);
				wsprintf(strLimit, "(%d/%d)", nLenText, BUFFER_MAX - 1);
				InvalidateRect(hWnd, &rt, TRUE);
				break;
			}
			break;
		}
		return 0;
	case WM_PARENTNOTIFY:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextAlign(hdc, TA_RIGHT | TA_BOTTOM);
		TextOut(hdc, rt.right, rt.bottom, strLimit, lstrlen(strLimit));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SYSKEYDOWN:
		switch (wParam)
		{
		case VK_F4:
			PostQuitMessage(0);
		}
		return 0;
	case WM_DROPFILES:
		DragQueryFile((HDROP)wParam, 0, strFile, 256);
		SetWindowText(GetParent(hWnd), strFile);
		DragFinish((HDROP)wParam);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}