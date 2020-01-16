#pragma once
#include "Scene.h"
class CGetIPScene :
	public CScene
{
public:
	CGetIPScene();
	virtual ~CGetIPScene();
	virtual void initScene();
	virtual void release();
};

