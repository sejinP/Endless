#include "global.h"
#include "CChaseState.h"

#include "CMonster.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CEventMgr.h"

CChaseState::CChaseState()
{
}

CChaseState::~CChaseState()
{
}

void CChaseState::Update()
{
	CMonster* owner = GetOwner();

	float sensingDist = owner->GetSensingDist();
	float attackDist = owner->GetAttackDist();

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

	// 감지 범위 바깥에 있는 경우 정찰 상태로 변경
	if (sensingDist <= distance)
	{
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::PATROL);
		return;
	}
	// 공격 범위 안에 있는 경우 공격 상태로 변경
	else if (distance <= attackDist)
	{
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::ATTACK);
		return;
	}
	// 이 외에는 플레이어를 따라감
	else
	{
		// 위치 따라가기
		float dt = CTimeMgr::Instance().GetDT();
		float ownerSpeed = owner->GetSpeed();
		distVec = XMVector3Normalize(distVec);
		ownerPosVec += distVec * ownerSpeed * dt;
		ownerPos = XMFLOAT3(XMVectorGetX(ownerPosVec), ownerPos.y, XMVectorGetZ(ownerPosVec));
		owner->SetPosition(ownerPos);

		// 회전 따라가기
		// 왼쪽에 있는지 오른쪽에 있는지 판별하여 그쪽 방향으로 회전
		RotationToTarget(distVec);
	}
}