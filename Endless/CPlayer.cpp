#include "global.h"
#include "CPlayer.h"
#include "CCentral.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CCamera.h"
#include "CLight.h"

#include "CKeyMgr.h"

#include "CShaderMgr.h"
#include "CObjectBuffer.h"
#include "CBufferMgr.h"

#include "CTextureMgr.h"
#include "CTexture.h"
#include "CDepthTexture.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CPlayerController.h"
#include "CRigidBody.h"

#include "CEventMgr.h"
#include "CBullet.h"

CPlayer::CPlayer()
    : m_buffer(nullptr)
{
    // 이름 설정
    wstring objName = L"PLAYER";
    SetObjectName(objName);

    LoadModel(L"cube.txt");

    CTexture* texture = CTextureMgr::Instance().GetTexture(L"ebony.dds");
    SetTexture(texture);

    m_buffer = CBufferMgr::Instance().GetObjectBuffer(GetModelVtx(), GetModelIdx());

    CPlayerController* playerController = new CPlayerController;
    playerController->SetOwner(this);
    playerController->SetMouseSens(30.0f);
    playerController->SetJumpVel(5.5f);
    SetComponent(playerController, COMPONENT_TYPE::CONTROLLER);

    CRigidBody* rigidbody = new CRigidBody;
    rigidbody->SetOwner(this);
    rigidbody->SetFriction(10.0f);
    SetComponent(rigidbody, COMPONENT_TYPE::RIGIDBODY);
}

CPlayer::~CPlayer()
{
    
}

void CPlayer::Update()
{
    // component update
    CObject::Update();

    ShootingUpdate();
}

void CPlayer::ShootingUpdate()
{
    if (SCENE_TYPE::START != CSceneMgr::Instance().GetSceneType())
    {
        if (CKeyMgr::Instance().CheckMouseState(MOUSE_BUTTON::LEFT, KEY_STATE::TAP) && !GetIsDead())
        {
            XMVECTOR forwardVec = GetForwardVec();
            XMFLOAT3 forwardDir;
            XMStoreFloat3(&forwardDir, forwardVec);

            XMFLOAT3 myPosition = GetPosition();
            XMVECTOR myPosVec = XMLoadFloat3(&myPosition);
            myPosVec += forwardVec;
            XMStoreFloat3(&myPosition, myPosVec);

            CBullet* bullet = CSceneMgr::Instance().GetCurrentScene()->GetBullet();
            bullet->SetObjectName(L"PLAYER_BULLET");
            bullet->SetForwardDir(forwardDir);
            bullet->SetPosition(myPosition);
            bullet->SetLifeTime(5.0f);
            bullet->SetSpeed(50.0f);
            bullet->SetOffensivePower(GetOffensivePower());
        }
    }
}

void CPlayer::DepthRender(ID3D11DeviceContext* _deviceContext)
{
    CLight* light = CSceneMgr::Instance().GetCurrentScene()->GetLight();
    UINT indexCount = GetIndexCount();
    
    // 월드 행렬
    XMMATRIX worldMatrix = GetWorldMatrix();

    // 빛 뷰 행렬
    XMMATRIX lightViewMatrix = light->GetViewMatrix();

    // 빛 투영 행렬
    XMMATRIX lightProjectionMatrix = light->GetProjectionMatrix();

    // 라이트 기준으로 그릴 것이므로 라이트 뷰 행렬과 라이트 투영 행렬 보냄
    MatrixBufferType transformMatrix;
    transformMatrix.worldMatrix = worldMatrix;
    transformMatrix.viewMatrix = lightViewMatrix;
    transformMatrix.projectionMatrix = lightProjectionMatrix;

    // 깊이 셰이더로 그려줌
    m_buffer->BufferSetting(_deviceContext);
    CShaderMgr::Instance().RenderDepthShader(_deviceContext, transformMatrix, indexCount);
}

void CPlayer::Render(ID3D11DeviceContext* _deviceContext)
{
    CCamera* camera = CSceneMgr::Instance().GetCurrentScene()->GetCamera();
    CLight* light = CSceneMgr::Instance().GetCurrentScene()->GetLight();
    ID3D11ShaderResourceView* texture = GetTexture()->GetTextureView();
    CDepthTexture* depthTexture = CSceneMgr::Instance().GetCurrentScene()->GetDepthTexture();
    ID3D11ShaderResourceView* depthMapTexture = depthTexture->GetShaderResoucreView();
    UINT indexCount = GetIndexCount();

    // 월드 행렬
    XMMATRIX worldMatrix = GetWorldMatrix();

    // 카메라 뷰 행렬
    XMMATRIX viewMatrix = camera->GetViewMatrix();

    // 투영 행렬
    XMMATRIX projectionMatrix = CCentral::Instance().GetProjectionMatrix();

    // 빛 뷰 행렬
    XMMATRIX lightViewMatrix = light->GetViewMatrix();

    // 빛 투영 행렬
    XMMATRIX lightProjectionMatrix = light->GetProjectionMatrix();

    // 바뀐 행렬 적용
    MatrixBufferType transformMatrix;
    transformMatrix.worldMatrix = worldMatrix;
    transformMatrix.viewMatrix = viewMatrix;
    transformMatrix.projectionMatrix = projectionMatrix;
    transformMatrix.lightViewMatrix = lightViewMatrix;
    transformMatrix.lightProjectionMatrix = lightProjectionMatrix;
    
    // 버퍼 세팅 후 셰이더 적용
    m_buffer->BufferSetting(_deviceContext);
    CShaderMgr::Instance().RenderShadowShader(_deviceContext, transformMatrix, texture, depthMapTexture, light, indexCount);
}

XMMATRIX CPlayer::GetWorldMatrix()
{
    // 스케일 행렬
    XMMATRIX scaleMat = GetScaleMatrix();
    
    // 회전 행렬, 플레이어 렌더링는 y축기준 회전만 한다.
    XMFLOAT3 rotation = GetRotation();
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.0f),
        XMConvertToRadians(rotation.y), XMConvertToRadians(0.0f));
    XMMATRIX rotationMat = XMMatrixRotationQuaternion(rotationQuat);

    // 이동 행렬
    XMMATRIX positionMat = GetTranslateMatrix();

    return (scaleMat * rotationMat * positionMat);
}

void CPlayer::OnCollisionEnter(CCollider* _otherCol)
{
    CObject* otherObj = _otherCol->GetOwner();
    const wstring otherName = otherObj->GetObjectName();
    if (L"MONSTER_BULLET" == otherName  || L"MONSTER" == otherName)
    {
        float myHP = GetHP();
        float otherPower = otherObj->GetOffensivePower();
        myHP -= otherPower;
        if (myHP <= 0)
        {
            SetIsDead(true);
        }
        SetHP(myHP);
    }
}

void CPlayer::OnCollision(CCollider* _otherCol)
{
    
}

void CPlayer::OnCollisionExit(CCollider* _otherCol)
{
    
}