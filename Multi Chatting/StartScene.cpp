#include "StartScene.h"

CFirstScene::CFirstScene()
{
}


CFirstScene::~CFirstScene()
{
}


void CFirstScene::initScene()
{
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "StartWindow", SERVICE_TITLE, WS_CAPTION|WS_BORDER,
		(WINDOW_WIDTH >> 1) - 200, (WINDOW_HEIGHT >> 1) - 200, 400, 400, NULL, NULL, g_hInst, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
}

void CFirstScene::release()
{
	DestroyWindow(hWnd);
}