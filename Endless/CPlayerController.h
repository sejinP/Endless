#pragma once
#include "CComponent.h"

#define MAXIMUM_ANGLE 89.5f  // 최대 각도. 이 각도를 넘어서지 못함

class CPlayerController :
    public CComponent
{
private:
    float       m_mouseSens;        // 마우스 민감도
    float       m_jumpVel;          // 점프 속력

private:
    void RotationUpdate();
    void TransformUpdate();
    void JumpUpdate();

public:
    void Update() override;

public:
    void SetMouseSens(float _mouseSens) { m_mouseSens = _mouseSens; }
    void SetJumpVel(float _jumpVel) { m_jumpVel = _jumpVel; }

public:
    CPlayerController();
    ~CPlayerController();
};

