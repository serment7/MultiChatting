#include "ChattingScene.h"


CChattingScene::CChattingScene()
{
}


CChattingScene::~CChattingScene()
{
}
void CChattingScene::initScene()
{
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "ChattingWindow", SERVICE_TITLE, WS_BORDER | WS_CAPTION,
		(WINDOW_WIDTH >> 1) - 300, (WINDOW_HEIGHT >> 1) - 300, 600, 600, NULL, NULL, g_hInst, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
}

void CChattingScene::release()
{
	DestroyWindow(hWnd);
}

