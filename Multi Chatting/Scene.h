#pragma once
#pragma comment(lib,"Comctl32.lib")

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<shellapi.h>
#include<CommCtrl.h>
#include"Constants.h"

extern HINSTANCE g_hInst;

enum id_button{
	ID_BUTTON_CREATE=1,
	ID_BUTTON_JOIN,
	ID_BUTTON_RETURNFIRST,
	ID_BUTTON_EXIT,
	ID_EDIT, ID_CHATEDIT, ID_USERLIST, ID_BUTTON_UPLOAD, ID_BUTTON_SAVE
};

enum id_scene{
	ID_FIRST_SCENE=1, ID_MAKE_CHATTING_SCENE, ID_GET_IP_SCENE,
	
};

class CScene
{

protected:
	HWND hWnd;
public:
	CScene();
	virtual ~CScene();
	virtual void initScene() = 0;
	virtual void release() = 0;
};