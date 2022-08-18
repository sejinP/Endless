#pragma once
#include "CState.h"

class CAttackState :
    public CState
{
private:
    float m_attackSpeed;
    float m_accumulateTime;

public:
    void Update() override;

public:
    void SetAttackSpeed(float _attackSpeed) { m_attackSpeed = _attackSpeed; }

public:
    CAttackState();
    ~CAttackState();
};

