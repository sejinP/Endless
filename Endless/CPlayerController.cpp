#include "global.h"
#include "CPlayerController.h"

#include "CCamera.h"
#include "CScene.h"

#include "CSceneMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CObject.h"
#include "CPlayer.h"

#include "CRigidBody.h"

CPlayerController::CPlayerController()
    : m_mouseSens(0.0f)
    , m_jumpVel(0.0f)
{
}

CPlayerController::~CPlayerController()
{
}

void CPlayerController::Update()
{
	RotationUpdate();
	TransformUpdate();
    JumpUpdate();
}

void CPlayerController::RotationUpdate()
{
    // 마우스 상태
    DIMOUSESTATE mouseState = CKeyMgr::Instance().GetMouseState();

    CObject* owner = GetOwner();

    float dt = CTimeMgr::Instance().GetDT();

    XMFLOAT3 positon = owner->GetPosition();
    XMVECTOR positionVec = XMLoadFloat3(&positon);				// 현재 포지션
    XMFLOAT3 up = owner->GetUpVector();
    XMVECTOR upVec = XMLoadFloat3(&up);					        // 업 벡터

    // 마우스의 변경이 있다면
    if (0 != mouseState.lX || 0 != mouseState.lY)
    {
        // owner의 회전값을 변경
        XMFLOAT3 rotate = owner->GetRotation();
        rotate.y += dt * m_mouseSens * mouseState.lX;
        rotate.x += dt * m_mouseSens * mouseState.lY;

        // 최대 각도 이상 넘어가지 않게 조정
        if (MAXIMUM_ANGLE < rotate.x)
            rotate.x = MAXIMUM_ANGLE;
        else if (rotate.x < -MAXIMUM_ANGLE)
            rotate.x = -MAXIMUM_ANGLE;

        if (360.0f <= abs(rotate.y))
            rotate.y = 0.0f;

        owner->SetRotation(rotate);
    }
}

void CPlayerController::TransformUpdate()
{
    CCamera* camera = CSceneMgr::Instance().GetCurrentScene()->GetCamera();
    CObject* owner = GetOwner();

    // 바라보고있는 곳을 기준으로 방향(앞, 오른쪽)을 구함
    XMVECTOR forwardVec = owner->GetForwardVec();
    XMVECTOR rightVec = owner->GetRightVec();

    XMFLOAT3 up = owner->GetUpVector();
    XMVECTOR upVec = XMLoadFloat3(&up);

    // owner의 속도
    XMFLOAT3 velocity = owner->GetVelocity();
    XMVECTOR velocityVec = XMLoadFloat3(&velocity);

    // 최종 적용시킬 속도
    XMVECTOR finalVel = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    forwardVec = XMVector3Cross(rightVec, upVec);                   // 플레이어 기준 카메라
    //forwardVec = owner->GetForwardVec();                          // 자유 시점 카메라, 중력 제거도 해야됨

    float speed = owner->GetSpeed();
    float dt = CTimeMgr::Instance().GetDT();

    if (CKeyMgr::Instance().CheckKeyState(KEY_BUTTON::KEY_A, KEY_STATE::HOLD))
    {
        finalVel -= rightVec;
    }
    if (CKeyMgr::Instance().CheckKeyState(KEY_BUTTON::KEY_D, KEY_STATE::HOLD))
    {
        finalVel += rightVec;
    }
    if (CKeyMgr::Instance().CheckKeyState(KEY_BUTTON::KEY_W, KEY_STATE::HOLD))
    {
        finalVel += forwardVec;
    }
    if (CKeyMgr::Instance().CheckKeyState(KEY_BUTTON::KEY_S, KEY_STATE::HOLD))
    {
        finalVel -= forwardVec;
    }

    // 속도 적용
    velocityVec = XMVector3Normalize(finalVel) * dt * speed;

    // 리지드바디의 AddVelocity로 속도를 더해줌.
    XMStoreFloat3(&velocity, velocityVec);
    owner->GetRigidBody()->AddVelocity(velocity);
}

void CPlayerController::JumpUpdate()
{
    CObject* owner = GetOwner();
    if (CKeyMgr::Instance().CheckKeyState(KEY_BUTTON::KEY_SPACE, KEY_STATE::TAP) && owner->GetIsGround())
    {
        owner->GetRigidBody()->AddVelocity(XMFLOAT3(0.0f, m_jumpVel, 0.0f));
    }
}
