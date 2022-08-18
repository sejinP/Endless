#include "global.h"
#include "CState.h"

#include "CTimeMgr.h"
#include "CMonster.h"

CState::CState()
	: m_owner(nullptr)
{
}

CState::~CState()
{
}

void CState::RotationToTarget(XMVECTOR _targetVec)
{
	// 타겟을 향해 회전
	float dt = CTimeMgr::Instance().GetDT();
	XMFLOAT3 ownerRotation = m_owner->GetRotation();
	float ownerRotSpeed = m_owner->GetRotationSpeed();

	XMVECTOR ownerForwardVec = m_owner->GetForwardVec();
	_targetVec = XMVector3Normalize(_targetVec);

	// 둘 사이의 각도가 0이라면(0에 가까우면) 회전을 하지 않음
	XMVECTOR angleVec = XMVector3AngleBetweenNormals(ownerForwardVec, _targetVec);
	float angle = XMVectorGetX(angleVec);
	angle = XMConvertToDegrees(angle);
	float epsilon = 0.001f;
	if (epsilon <= angle)
	{
		XMVECTOR crossVec = XMVector3Cross(ownerForwardVec, _targetVec);

		// 자기가 보고있는 방향과, 플레이어와의 방향을 외적한 후 업 벡터와 내적하여 양수라면 오른쪽, 음수라면 왼쪽
		XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR dotVec = XMVector3Dot(crossVec, upVec);
		float dot = XMVectorGetX(dotVec);

		// 왼쪽
		if (dot < 0.0f)
		{
			ownerRotation.y -= ownerRotSpeed * dt;
		}
		// 오른쪽
		else
		{
			ownerRotation.y += ownerRotSpeed * dt;
		}
		m_owner->SetRotation(ownerRotation);
	}
}