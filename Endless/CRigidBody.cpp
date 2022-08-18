#include "global.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"

#include "CObject.h"

CRigidBody::CRigidBody()
	: m_gravity(-15.0f)
	, m_acceleration(0.0f)
	, m_friction(0.0f)
{
	
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Update()
{
	RigidBodyUpdate();
}

void CRigidBody::RigidBodyUpdate()
{
	CObject* owner = GetOwner();
	float dt = CTimeMgr::Instance().GetDT();
	float maxSpeed = owner->GetMaxSpeed();
	XMFLOAT3 gravityForce;

	// 땅에 있는 상태라면 중력 없음
	if (owner->GetIsGround())
		gravityForce = XMFLOAT3(0.0f, 0.0f, 0.0f);
	else
		gravityForce = XMFLOAT3(0.0f, owner->GetMass() * m_gravity, 0.0f);

	// 질량에 따른 가속도. 속도와 포지션도 가져옴
	XMFLOAT3 acceleration = XMFLOAT3(gravityForce.x / owner->GetMass(),
		gravityForce.y / owner->GetMass(), gravityForce.z / owner->GetMass());
	XMFLOAT3 velocity = owner->GetVelocity();
	XMFLOAT3 position = owner->GetPosition();

	XMVECTOR accelerationVec = XMLoadFloat3(&acceleration);
	XMVECTOR velocityVec = XMLoadFloat3(&velocity);
	XMVECTOR posVec = XMLoadFloat3(&position);

	// 속도는 (가속도 * 시간), 포지션은 (속도 * 시간)
	velocityVec += accelerationVec * dt;

	// 속도 조정(최대속력 넘어가지 못하게)
	AdjustVelocity(velocityVec, maxSpeed);

	// 마찰력 적용
	FrictionUpdate(velocityVec, dt);

	// 최종 적용된 속도 세팅
	XMStoreFloat3(&velocity, velocityVec);
	owner->SetVelocity(velocity);

	// 최종 적용된 속도값으로 포지션 계산 후 세팅
	posVec += velocityVec * dt;
	XMStoreFloat3(&position, posVec);
	owner->SetPosition(position);
}

void CRigidBody::AdjustVelocity(XMVECTOR& _velocityVec, float _maxSpeed)
{
	// 최대 속도 넘어가지 못하게 (x, z축만 해당됨)
	XMVECTOR adjustVel = XMVectorSet(XMVectorGetX(_velocityVec), 0.0f, XMVectorGetZ(_velocityVec), 0.0f);
	if (_maxSpeed <= XMVectorGetX(XMVector3Length(adjustVel)))
	{
		adjustVel = XMVector3Normalize(adjustVel) * _maxSpeed;
	}

	// 조정한 속도로 설정 (x, z축)
	_velocityVec = XMVectorSet(XMVectorGetX(adjustVel), XMVectorGetY(_velocityVec), XMVectorGetZ(adjustVel), 0.0f);
}

void CRigidBody::FrictionUpdate(XMVECTOR& _velocityVec, float _dt)
{
	// 마찰은 현재 속도의 반대방향으로 적용됨 (x, z축만 적용)
	XMVECTOR velocityVec = XMVectorSet(XMVectorGetX(_velocityVec), 0.0f, XMVectorGetZ(_velocityVec), 0.0f);
	XMVECTOR frictionVec = velocityVec;
	frictionVec = XMVector3Normalize(frictionVec);
	frictionVec = -frictionVec * _dt * m_friction;

	// 만약 현재 적용된 속도보다(x, z축의 속도) 마찰이 더 크다면 속도를 0으로 만듬 (y축은 제외)
	// 속도가 더 크다면 마찰 적용
	if (XMVectorGetX(XMVector3Length(velocityVec)) <= XMVectorGetX(XMVector3Length(frictionVec)))
		_velocityVec = XMVectorSet(0.0f, XMVectorGetY(_velocityVec), 0.0f, 0.0f);
	else
		_velocityVec += frictionVec;
}

void CRigidBody::AddVelocity(XMFLOAT3 _velocity)
{
	CObject* owner = GetOwner();
	XMFLOAT3 velocity = owner->GetVelocity();
	velocity = XMFLOAT3(velocity.x + _velocity.x, velocity.y + _velocity.y, velocity.z + _velocity.z);
	owner->SetVelocity(velocity);
}