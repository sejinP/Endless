#pragma once

class CUI;

class CUIMgr
{
public:
	static CUIMgr& Instance() { static CUIMgr instance; return instance; }

private:
	CUI* m_targetedUI;

public:
	void Update();

private:
	CUI* GetTargetUI();

public:
	CUIMgr();
	~CUIMgr();
};

