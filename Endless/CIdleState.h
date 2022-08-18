#pragma once
#include "CState.h"

class CIdleState :
    public CState
{
private:
    float m_stayTime;
    float m_accumulateTime;

public:
    void Update() override;

    void SetStayTime(float _stayTime) { m_stayTime = _stayTime; }

public:
    CIdleState();
    ~CIdleState();
};

