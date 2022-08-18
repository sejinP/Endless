#include "global.h"
#include "CAttackState.h"

#include "CMonster.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CBullet.h"

#include "CEventMgr.h"

CAttackState::CAttackState()
	: m_attackSpeed(0.0f)
	, m_accumulateTime(0.0f)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::Update()
{
	CMonster* owner = GetOwner();
	float attackDist = owner->GetAttackDist();

	CPlayer* player = CSceneMgr::Instance().GetCurrentScene()->GetPlayer();

	XMFLOAT3 ownerPos = owner->GetPosition();
	XMFLOAT3 playerPos = player->GetPosition();

	// x축과 z축에 대해서만
	XMVECTOR ownerPosVec = XMLoadFloat3(&ownerPos);
	XMVECTOR playerPosVec = XMLoadFloat3(&playerPos);
	XMVECTOR ownerForwardVec = playerPosVec - ownerPosVec;
	ownerForwardVec = XMVector3Normalize(ownerForwardVec);
	ownerPosVec = XMVectorSet(XMVectorGetX(ownerPosVec), 0.0f, XMVectorGetZ(ownerPosVec), 0.0f);
	playerPosVec = XMVectorSet(XMVectorGetX(playerPosVec), 0.0f, XMVectorGetZ(playerPosVec), 0.0f);
	XMVECTOR distVec = playerPosVec - ownerPosVec;

	float distance = XMVectorGetX(XMVector3Length(distVec));

	// 공격 범위를 벗어난다면 쫓는 상태로 변경
	if (attackDist <= distance)
	{
		CEventMgr::Instance().ChangeState(owner, MONSTER_STATE::CHASE);
		return;
	}

	// 회전 따라가기
	// 왼쪽에 있는지 오른쪽에 있는지 판별하여 그쪽 방향으로 회전
	float dt = CTimeMgr::Instance().GetDT();
	RotationToTarget(distVec);

	// 누적 시간이 공격 스피드 시간보다 초과가 됐을 시 공격
	m_accumulateTime += dt;
	if (m_attackSpeed <= m_accumulateTime)
	{
		m_accumulateTime = 0.0f;

		// attack
		XMFLOAT3 ownerForward;
		XMStoreFloat3(&ownerForward, ownerForwardVec);

		XMFLOAT3 myPosition = owner->GetPosition();
		XMVECTOR myPosVec = XMLoadFloat3(&myPosition);
		XMVECTOR bulletPosVec = myPosVec + ownerForwardVec;
		XMFLOAT3 bulletPos;
		XMStoreFloat3(&bulletPos, bulletPosVec);

		CBullet* bullet = CSceneMgr::Instance().GetCurrentScene()->GetBullet();
		bullet->SetObjectName(L"MONSTER_BULLET");
		bullet->SetForwardDir(ownerForward);
		bullet->SetPosition(bulletPos);
		bullet->SetLifeTime(5.0f);
		bullet->SetSpeed(25.0f);
		bullet->SetOffensivePower(owner->GetOffensivePower());
	}
}