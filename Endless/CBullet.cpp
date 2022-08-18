#include "global.h"
#include "CBullet.h"

#include "CCentral.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CObjectBuffer.h"
#include "CBufferMgr.h"

#include "CLight.h"
#include "CCamera.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CShaderMgr.h"
#include "CTimeMgr.h"

#include "CDepthTexture.h"

CBullet::CBullet()
    : m_buffer(nullptr)
    , m_forwardDir(XMFLOAT3(0.0f, 0.0f, 0.0f))
    , m_accumulateTime(0.0f)
    , m_lifeTime(0.0f)
{
    // 이름 설정
    wstring objName = L"MONSTER";
    SetObjectName(objName);

    // 모델 로드, 텍스쳐 정보, 버퍼 생성
    LoadModel(L"sphere.txt");

    CTexture* texture = CTextureMgr::Instance().GetTexture(L"black.dds");
    SetTexture(texture);

    m_buffer = CBufferMgr::Instance().GetBulletBuffer(GetModelVtx(), GetModelIdx());
}

CBullet::CBullet(const CBullet& _other)
    : CObject(_other)
    , m_buffer(_other.m_buffer)
    , m_forwardDir(_other.m_forwardDir)
    , m_accumulateTime(_other.m_accumulateTime)
    , m_lifeTime(_other.m_lifeTime)
{

}

CBullet::~CBullet()
{
    
}

void CBullet::Update()
{
    // 컴포넌트 업데이트
    CObject::Update();

    // 바라보고 있는 방향으로 이동만 함
    float dt = CTimeMgr::Instance().GetDT();
    float speed = GetSpeed();

    XMVECTOR forwardDirVec = XMLoadFloat3(&m_forwardDir);
    forwardDirVec = XMVector3Normalize(forwardDirVec);

    XMFLOAT3 position = GetPosition();
    XMVECTOR posVec = XMLoadFloat3(&position);

    posVec += forwardDirVec * speed * dt;
    XMStoreFloat3(&position, posVec);
    SetPosition(position);

    // 시간 누적 후 생명 시간이 다 되면 자기 자신을 반납
    m_accumulateTime += dt;
    if (m_lifeTime <= m_accumulateTime)
    {
        CSceneMgr::Instance().GetCurrentScene()->ReturnBullet(this);
        m_accumulateTime = 0.0f;
    }
}

void CBullet::DepthRender(ID3D11DeviceContext* _deviceContext)
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

void CBullet::Render(ID3D11DeviceContext* _deviceContext)
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

void CBullet::OnCollisionEnter(CCollider* _otherCol)
{
    // 누군가와 닿으면 사라짐
    CSceneMgr::Instance().GetCurrentScene()->ReturnBullet(this);
    m_accumulateTime = 0.0f;
}

void CBullet::OnCollision(CCollider* _otherCol)
{
}

void CBullet::OnCollisionExit(CCollider* _otherCol)
{
}