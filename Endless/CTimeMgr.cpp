#include "global.h"
#include "CTimeMgr.h"
#include "CCentral.h"

CTimeMgr::CTimeMgr()
	: m_titleString{}
	, m_deltaTime(0.0f)
	, m_accumulate(0.0f)
	, m_fps(0)
	, m_count(0)
	, m_timer{}
	, m_startTime{}
	, m_endTime{}
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_timer);
	QueryPerformanceCounter(&m_startTime);
}

void CTimeMgr::Update()
{
	// 델타 타임
	QueryPerformanceCounter(&m_endTime);
	m_deltaTime = (m_endTime.QuadPart - m_startTime.QuadPart) / (float)m_timer.QuadPart;
	m_startTime = m_endTime;

	// dt 조정
	if (m_deltaTime >= (1 / 60.0f))
		m_deltaTime = (1 / 60.0f);

	// 시간 누적
	m_accumulate += m_deltaTime;

	m_count++;

	// 만약 1초가 됐다면
	if (m_accumulate >= 1.0f)
	{
		m_fps = m_count;
		m_count = 0;

		m_accumulate = 0.0f;
	}
}

void CTimeMgr::Render()
{
	// 타이틀 바에 FPS와 DT 나타냄.
	swprintf(m_titleString, 128, L"FPS : %05d  DT : %f", m_fps, m_deltaTime);
	SetWindowTextW(CCentral::Instance().GetWindHandle(), m_titleString);
}