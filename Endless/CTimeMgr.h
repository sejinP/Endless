#pragma once

class CTimeMgr
{
public:
	static CTimeMgr& Instance() { static CTimeMgr instance; return instance; }

private:
	WCHAR m_titleString[128];

	float	m_deltaTime;
	float	m_accumulate;

	int m_fps;
	int m_count;

	LARGE_INTEGER m_timer;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_endTime;

public:
	void Init();
	void Update();
	void Render();

	float GetDT() { return m_deltaTime; }

public:
	CTimeMgr();
	~CTimeMgr();
};

