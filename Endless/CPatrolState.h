#pragma once
#include "CState.h"

class CPatrolState :
    public CState
{
private:
    XMFLOAT3 m_currentPatrolPos;
    XMFLOAT3 m_nextPatrolPos;

public:
    void Update() override;

public:
    void SetPatrolPos(XMFLOAT3 _currentPatrolPos, XMFLOAT3 _nextPatrolPos)
    { m_currentPatrolPos = _currentPatrolPos; m_nextPatrolPos = _nextPatrolPos; }

public:
    CPatrolState();
    ~CPatrolState();
};

