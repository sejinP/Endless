#include "global.h"
#include "CIdleState.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

#include "CEventMgr.h"

CIdleState::CIdleState()
	: m_stayTime(0.0f)
	, m_accumulateTime(0.0f)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Update()
{
	float dt = CTimeMgr::Instance().GetDT();
	m_accumulateTime += dt;

	CMonster* owner = GetOwner();
	CPlayer* player = CSceneMgr::Instance().GetCurrentScene()->GetPlayer();

	// 머무른 시간이 충족이 되면 정찰 상태로 바꿈
	if (m_accumulateTime >= m_stayTime)
	{
		m_accumulateTime = 0.0f;
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::PATROL);
		return;
	}

	// 플레이어가 범위 내로 들어온다면 쫓는 상태로 변경
	XMFLOAT3 ownerPos = owner->GetPosition();
	XMFLOAT3 playerPos = player->GetPosition();

	XMVECTOR ownerPosVec = XMLoadFloat3(&ownerPos);
	XMVECTOR playerPosVec = XMLoadFloat3(&playerPos);
	XMVECTOR distVec = playerPosVec - ownerPosVec;
	
	float distance = XMVectorGetX(XMVector3Length(distVec));
	float sensingDist = owner->GetSensingDist();

	if (distance <= sensingDist)
	{
		// 상태 바꾸기 전 누적 시간은 초기화
		m_accumulateTime = 0.0f;
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::CHASE);
		return;
	}
}