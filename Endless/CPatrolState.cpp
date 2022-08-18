#include "global.h"
#include "CPatrolState.h"

#include "CMonster.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"

#include "CEventMgr.h"

CPatrolState::CPatrolState()
	: m_currentPatrolPos(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_nextPatrolPos(XMFLOAT3(0.0f, 0.0f, 0.0f))
{
}

CPatrolState::~CPatrolState()
{
}

void CPatrolState::Update()
{
	CMonster* owner = GetOwner();
	float sensingDist = owner->GetSensingDist();

	// 플레이어와의 거리먼저 계산하여 감지 범위 안에 든다면 쫓는 상태로 변경
	CPlayer* player = CSceneMgr::Instance().GetCurrentScene()->GetPlayer();

	XMFLOAT3 ownerPos = owner->GetPosition();
	XMFLOAT3 playerPos = player->GetPosition();

	// x축과 z축에 대해서만
	XMVECTOR ownerPosVec = XMLoadFloat3(&ownerPos);
	ownerPosVec = XMVectorSet(XMVectorGetX(ownerPosVec), 0.0f, XMVectorGetZ(ownerPosVec), 0.0f);
	XMVECTOR playerPosVec = XMLoadFloat3(&playerPos);
	playerPosVec = XMVectorSet(XMVectorGetX(playerPosVec), 0.0f, XMVectorGetZ(playerPosVec), 0.0f);
	XMVECTOR distVec = playerPosVec - ownerPosVec;
	
	float distance = XMVectorGetX(XMVector3Length(distVec));

	// 플레이어와의 거리가 감지 범위보다 작다면 쫓는 상태로 변경
	if (distance <= sensingDist)
	{
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::CHASE);
		return;
	}

	XMVECTOR currentPatrolPos = XMLoadFloat3(&m_currentPatrolPos);
	XMVECTOR nextPatrolPos = XMLoadFloat3(&m_nextPatrolPos);

	XMVECTOR goDirectionVec = currentPatrolPos - ownerPosVec;

	// 오차 범위
	float epsilon = 0.01f;

	// 현재 정찰 포지션으로 이동을 한 경우 currentPatrolPos 와 nextPatrolPos 스왑 후 Idle상태로 변경
	if (XMVectorGetX(XMVector3Length(goDirectionVec)) <= epsilon)
	{
		XMStoreFloat3(&m_currentPatrolPos, nextPatrolPos);
		XMStoreFloat3(&m_nextPatrolPos, currentPatrolPos);
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::IDLE);
		return;
	}
	else
	{
		// 현재 정찰 포지션으로 이동
		float dt = CTimeMgr::Instance().GetDT();
		float ownerSpeed = owner->GetSpeed();
		goDirectionVec = XMVector3Normalize(goDirectionVec);
		ownerPosVec += goDirectionVec * ownerSpeed * dt;
		ownerPos = XMFLOAT3(XMVectorGetX(ownerPosVec), ownerPos.y, XMVectorGetZ(ownerPosVec));
		owner->SetPosition(ownerPos);

		// 회전 따라가기
		// 왼쪽에 있는지 오른쪽에 있는지 판별하여 그쪽 방향으로 회전
		RotationToTarget(goDirectionVec);
	}
}