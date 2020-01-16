#pragma once
#include "Scene.h"
class CChattingScene :
	public CScene
{
public:
	CChattingScene();
	virtual ~CChattingScene();
	virtual void initScene();
	virtual void release();
};

