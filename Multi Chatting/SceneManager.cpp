#include "SceneManager.h"

CSceneManager::CSceneManager()
{

}
void CSceneManager::init()
{

}
CSceneManager::~CSceneManager()
{
}
void CSceneManager::beginScene(int nScene)
{
	switch (nScene)
	{
	case ID_FIRST_SCENE:
		m_scene = new CFirstScene;
		m_scene->initScene();
		break;
	case ID_MAKE_CHATTING_SCENE:
		m_scene = new CChattingScene;
		m_scene->initScene();
		break;
	case ID_GET_IP_SCENE:
		m_scene = new CGetIPScene;
		m_scene->initScene();
		break;
	}
}

void CSceneManager::endScene()
{
    m_scene->release();
	delete m_scene;
}

void CSceneManager::changeScene(int uScene)
{
	endScene();
	beginScene(uScene);
}