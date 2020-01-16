#pragma once
#include "Scene.h"

class CFirstScene : public CScene
{
private:
public:
	CFirstScene();
	virtual ~CFirstScene();
	virtual void initScene();
	virtual void release();
};
