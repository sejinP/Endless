#pragma once

class CUI;

class CGameMgr
{
public:
	static CGameMgr& Instance() { static CGameMgr instance; return instance; }

private:
	CUI* m_gameOverPanel;
	CUI* m_gameClearPanel;

public:
	void Update();

public:
	void SetGameOverPanel(CUI* _gameOverPanel) { m_gameOverPanel = _gameOverPanel; }
	void SetGameClearPanel(CUI* _gameClearPanel) { m_gameClearPanel = _gameClearPanel; }

public:
	CGameMgr();
	~CGameMgr();
};

