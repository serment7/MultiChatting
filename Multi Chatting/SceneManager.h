#pragma once
#include"StartScene.h"
#include"GetIPScene.h"
#include"ChattingScene.h"


class CSceneManager
{
private:
	CScene * m_scene;
public:
	CSceneManager();
	~CSceneManager();
	void beginScene(int);
	void endScene();
	void init();
	void changeScene(int);
};
