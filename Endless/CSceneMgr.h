#pragma once

class CScene;

class CSceneMgr
{
public:
	static CSceneMgr& Instance() { static CSceneMgr instance; return instance; }

private:
	CScene*		m_currentScene;
	SCENE_TYPE	m_currentSceneType;

public:
	void Init();
	void Update();
	void Render(ID3D11DeviceContext* _deviceContext);

	void ChangeScene(SCENE_TYPE _sceneType);

public:
	CScene* GetCurrentScene() { return m_currentScene; }
	SCENE_TYPE GetSceneType() { return m_currentSceneType; }

public:
	CSceneMgr();
	~CSceneMgr();
};

