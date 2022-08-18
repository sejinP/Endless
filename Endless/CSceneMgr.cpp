#include "global.h"
#include "CSceneMgr.h"

#include "CStartScene.h"
#include "CStage1Scene.h"
#include "CStage2Scene.h"
#include "CStage3Scene.h"

CSceneMgr::CSceneMgr()
	: m_currentScene(nullptr)
	, m_currentSceneType(SCENE_TYPE::END)
{

}

CSceneMgr::~CSceneMgr()
{
	if (m_currentScene)
	{
		// ¾À Exit() ÇÊ¼ö
		m_currentScene->Exit();
		delete m_currentScene;
	}
}

void CSceneMgr::Init()
{
	// ÇöÀç ¾À »ý¼º.
	m_currentScene = new CStartScene;
	m_currentScene->Enter();
	m_currentSceneType = SCENE_TYPE::START;
}

void CSceneMgr::Update()
{
	m_currentScene->Update();
}

void CSceneMgr::Render(ID3D11DeviceContext* _deviceContext)
{
	m_currentScene->Render(_deviceContext);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _sceneType)
{
	m_currentScene->Exit();
	delete m_currentScene;

	switch (_sceneType)
	{
	case SCENE_TYPE::START:
	{
		m_currentScene = new CStartScene;
		m_currentScene->Enter();
		m_currentSceneType = SCENE_TYPE::START;
	}
		break;
	case SCENE_TYPE::STAGE1:
	{
		m_currentScene = new CStage1Scene;
		m_currentScene->Enter();
		m_currentSceneType = SCENE_TYPE::STAGE1;
	}
		break;
	case SCENE_TYPE::STAGE2:
	{
		m_currentScene = new CStage2Scene;
		m_currentScene->Enter();
		m_currentSceneType = SCENE_TYPE::STAGE2;
	}
		break;
	case SCENE_TYPE::STAGE3:
	{
		m_currentScene = new CStage3Scene;
		m_currentScene->Enter();
		m_currentSceneType = SCENE_TYPE::STAGE3;
	}
		break;
	case SCENE_TYPE::END:
		break;
	default:
		break;
	}
}
