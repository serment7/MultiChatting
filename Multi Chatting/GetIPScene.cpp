#include "GetIPScene.h"


CGetIPScene::CGetIPScene()
{
}

CGetIPScene::~CGetIPScene()
{
}

void CGetIPScene::initScene()
{
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "GetIPWindow", SERVICE_TITLE, WS_BORDER | WS_CAPTION,
		(WINDOW_WIDTH >> 1) - 200, (WINDOW_HEIGHT >> 1) - 200, 400, 400, NULL, NULL, g_hInst, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
}

void CGetIPScene::release()
{
	DestroyWindow(hWnd);
}

