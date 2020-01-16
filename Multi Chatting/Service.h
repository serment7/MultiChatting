#pragma once

#include"SceneManager.h"
#include"SocketManager.h"

extern 

class CService
{
private:
	CSocketManager * m_socketManager;
	CSceneManager * m_sceneManager;
public:
	CService();
	~CService();
	void run();
	void init();
	void release();
	void checkBuffer();
};
LRESULT CALLBACK StartProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChattingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetIPProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK editboxSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);