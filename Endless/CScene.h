#pragma once

class CObject;
class CUI;
class CPlayer;
class CCamera;
class CBullet;
class CLight;
class CDepthTexture;

class CScene
{
private:
	vector<CObject*>	m_objects[(size_t)OBJECT_TYPE::END];
	vector<CUI*>		m_uis;
	queue<CBullet*>		m_bulletPool;

	CPlayer*			m_player;
	CCamera*			m_camera;
	CLight*				m_light;
	CDepthTexture*		m_depthTexture;
	CUI*				m_menuPanel;
	GAME_STATE			m_gameState;

public:
	virtual void Update();
	virtual void Render(ID3D11DeviceContext* _deviceContext);

	void PushObject(CObject* _object, OBJECT_TYPE _objectType) { m_objects[(size_t)_objectType].push_back(_object); }
	void PullObject(CObject* _object, OBJECT_TYPE _objectType);
	void PushUI(CUI* _ui) { m_uis.push_back(_ui); }

	void DeleteObject(CObject* _object, OBJECT_TYPE _objectType);

	// 순수 가상 함수
	virtual void Enter();
	virtual void Exit();

	virtual void CreateStructure();
	virtual void CreateMonster();
	virtual void CreateUI();

public:
	const vector<CObject*>& GetSceneObj(OBJECT_TYPE _objectType) { return m_objects[(size_t)_objectType]; }
	const vector<CUI*>& GetUI() { return m_uis; }

	void SetPlayer(CPlayer* _player) { m_player = _player; }
	CPlayer* GetPlayer() { return m_player; }
	void SetCamera(CCamera* _camera) { m_camera = _camera; }
	CCamera* GetCamera() { return m_camera; }
	void SetLight(CLight* _light) { m_light = _light; }
	CLight* GetLight() { return m_light; }
	void SetDepthTexture(CDepthTexture* _depthTexture) { m_depthTexture = _depthTexture; }
	CDepthTexture* GetDepthTexture() { return m_depthTexture; }

	void InitBullet(int _bulletNum);
	CBullet* GetBullet();
	void ReturnBullet(CBullet* _bullet);

	void SetMenuPanel(CUI* _menuPanel) { m_menuPanel = _menuPanel; }
	CUI* GetMenuPanel() { return m_menuPanel; }
	
	void SetGameState(GAME_STATE _gameState) { m_gameState = _gameState; }
	GAME_STATE GetGameState() { return m_gameState; }

public:
	CScene();
	virtual ~CScene();
};

