#include "global.h"
#include "CMonster.h"

#include "CCentral.h"

#include "CShaderMgr.h"

#include "CTextureMgr.h"
#include "CTexture.h"
#include "CDepthTexture.h"

#include "CObjectBuffer.h"
#include "CBufferMgr.h"

#include "CCamera.h"
#include "CLight.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CFbxMgr.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CState.h"
#include "CIdleState.h"
#include "CPatrolState.h"
#include "CChaseState.h"
#include "CAttackState.h"

CMonster::CMonster()
    : m_buffer(nullptr)
    , m_currentState(nullptr)
    , m_sensingDist(0.0f)
    , m_attackDist(0.0f)
    , m_rotationSpeed(0.0f)
{
    // 이름 설정
    wstring objName = L"MONSTER";
    SetObjectName(objName);

    CRigidBody* rigidbody = new CRigidBody;
    rigidbody->SetOwner(this);
    SetComponent(rigidbody, COMPONENT_TYPE::RIGIDBODY);

    for (size_t i = 0; i < (size_t)MONSTER_STATE::END; ++i)
    {
        m_states[i] = nullptr;
    }

    CIdleState* idleState = new CIdleState; AddState(idleState, MONSTER_STATE::IDLE);
    CChaseState* chaseState = new CChaseState; AddState(chaseState, MONSTER_STATE::CHASE);
    CPatrolState* patrolState = new CPatrolState; AddState(patrolState, MONSTER_STATE::PATROL);
    CAttackState* attackState = new CAttackState; AddState(attackState, MONSTER_STATE::ATTACK);
    m_currentState = idleState;
}

CMonster::CMonster(const CMonster& _other)
    : CObject(_other)
    , m_buffer(_other.m_buffer)
    , m_currentState(nullptr)
    , m_sensingDist(_other.m_sensingDist)
    , m_attackDist(_other.m_attackDist)
    , m_rotationSpeed(_other.m_rotationSpeed)
{
    for (size_t i = 0; i < (size_t)MONSTER_STATE::END; ++i)
    {
        m_states[i] = nullptr;
    }

    if (nullptr != _other.m_states[(size_t)MONSTER_STATE::IDLE])
    {
        CIdleState* otherIdle = (CIdleState*)_other.m_states[(size_t)MONSTER_STATE::IDLE];
        CIdleState* idleState = new CIdleState(*otherIdle);
        idleState->SetOwner(this);
        m_states[(size_t)MONSTER_STATE::IDLE] = idleState;
        m_currentState = idleState;
    }

    if (nullptr != _other.m_states[(size_t)MONSTER_STATE::PATROL])
    {
        CPatrolState* otherPatrol = (CPatrolState*)_other.m_states[(size_t)MONSTER_STATE::PATROL];
        CPatrolState* patrolState = new CPatrolState(*otherPatrol);
        patrolState->SetOwner(this);
        m_states[(size_t)MONSTER_STATE::PATROL] = patrolState;
    }

    if (nullptr != _other.m_states[(size_t)MONSTER_STATE::CHASE])
    {
        CChaseState* otherChase = (CChaseState*)_other.m_states[(size_t)MONSTER_STATE::CHASE];
        CChaseState* chaseState = new CChaseState(*otherChase);
        chaseState->SetOwner(this);
        m_states[(size_t)MONSTER_STATE::CHASE] = chaseState;
    }

    if (nullptr != _other.m_states[(size_t)MONSTER_STATE::ATTACK])
    {
        CAttackState* otherAttack = (CAttackState*)_other.m_states[(size_t)MONSTER_STATE::ATTACK];
        CAttackState* attackState = new CAttackState(*otherAttack);
        attackState->SetOwner(this);
        m_states[(size_t)MONSTER_STATE::ATTACK] = attackState;
    }
}

CMonster::~CMonster()
{
    for (size_t i = 0; i < (size_t)MONSTER_STATE::END; ++i)
    {
        if (nullptr != m_states[i])
            delete m_states[i];
    }
}

void CMonster::Update()
{
    // 컴포넌트 업데이트
    CObject::Update();

    // 상태 업데이트
    if(nullptr != m_currentState)
        m_currentState->Update();
}

void CMonster::DepthRender(ID3D11DeviceContext* _deviceContext)
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

void CMonster::Render(ID3D11DeviceContext* _deviceContext)
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

void CMonster::OnCollisionEnter(CCollider* _otherCol)
{
    CObject* otherObj = _otherCol->GetOwner();
    wstring otherName = otherObj->GetObjectName();
    if (L"PLAYER_BULLET" == otherName)
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

void CMonster::OnCollision(CCollider* _otherCol)
{
    
}

void CMonster::OnCollisionExit(CCollider* _otherCol)
{
    
}

void CMonster::AddState(CState* _state, MONSTER_STATE _stateType)
{
    m_states[(size_t)_stateType] = _state;
    _state->SetOwner(this);
}

void CMonster::LoadModel(const wchar_t* _fileName)
{
    vector<ModelType> modelVtx;
    vector<UINT> modelIdx;

    wstring absolutePath = CCentral::Instance().GetAbsolutePath();
    absolutePath += L"fbx\\";
    absolutePath += _fileName;
    const wchar_t* fileName = absolutePath.c_str();

    CFbxMgr::Instance().LoadFbxModel(fileName, modelVtx, modelIdx);

    SetModelVtx(modelVtx);
    SetModelIdx(modelIdx);
}