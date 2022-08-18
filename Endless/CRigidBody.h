#pragma once
#include "CComponent.h"

class CRigidBody :
    public CComponent
{
private:
    float m_gravity;
    float m_acceleration;
    float m_friction;

private:
    void RigidBodyUpdate();
    void AdjustVelocity(XMVECTOR& _velocityVec, float _maxSpeed);
    void FrictionUpdate(XMVECTOR& _velocityVec, float _dt);

public:
    void Update() override;

public:
    void AddVelocity(XMFLOAT3 _velocity);

    void SetFriction(float _friction) { m_friction = _friction; }

public:
    CRigidBody();
    ~CRigidBody();
};

