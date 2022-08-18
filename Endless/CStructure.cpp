#include "global.h"
#include "CStructure.h"

#include "CShaderMgr.h"

#include "CCentral.h"

#include "CCamera.h"
#include "CLight.h"

#include "CScene.h"
#include "CSceneMgr.h"

#include "CTextureMgr.h"
#include "CTexture.h"
#include "CDepthTexture.h"

#include "CObjectBuffer.h"
#include "CCommonShader.h"

#include "CCollider.h"

CStructure::CStructure()
    : m_buffer(nullptr)
{
    
}

CStructure::CStructure(const CStructure& _other)
    : CObject(_other)
    , m_buffer(_other.m_buffer)
{
    
}

CStructure::~CStructure()
{
    
}

void CStructure::Update()
{
    // component update
    CObject::Update();
}

void CStructure::DepthRender(ID3D11DeviceContext* _deviceContext)
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

void CStructure::Render(ID3D11DeviceContext* _deviceContext)
{
    CCamera* camera = CSceneMgr::Instance().GetCurrentScene()->GetCamera();
    CLight* light = CSceneMgr::Instance().GetCurrentScene()->GetLight();
    ID3D11ShaderResourceView* texture = GetTexture()->GetTextureView();
    CDepthTexture* depthTexture = CSceneMgr::Instance().GetCurrentScene()->GetDepthTexture();
    ID3D11ShaderResourceView* depthMapTexture = depthTexture->GetShaderResoucreView();
    UINT indexCount = GetIndexCount();

    // 월드 행렬
    XMMATRIX worldMatrix = GetWorldMatrix();

    // 뷰 행렬
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

void CStructure::OnCollisionEnter(CCollider* _otherCol)
{
    // 구조물과 닿았을 때 포지션 재조정으로 밀어냄
    RePosition(_otherCol);
}

void CStructure::OnCollision(CCollider* _otherCol)
{
    // 구조물과 닿았을 때 포지션 재조정으로 밀어냄
    RePosition(_otherCol);
}

void CStructure::OnCollisionExit(CCollider* _otherCol)
{
    CObject* otherObj = _otherCol->GetOwner();
    otherObj->SetIsGround(false);
}

void CStructure::RePosition(CCollider* _otherCol)
{
    CObject* otherObj = _otherCol->GetOwner();
    XMFLOAT3 objectPos = otherObj->GetPosition();
    XMFLOAT3 velocity = otherObj->GetVelocity();

    CCollider* myCol = GetCollider();
    
    XMFLOAT3 myPos = myCol->GetPosition();
    XMFLOAT3 otherPos = _otherCol->GetPosition();
    XMFLOAT3 myScale = myCol->GetScale();
    XMFLOAT3 otherScale = _otherCol->GetScale();

    // 자신의(structure) 충돌체의 작은값과 큰값
    float myMinX = myPos.x - myScale.x, myMaxX = myPos.x + myScale.x;
    float myMinY = myPos.y - myScale.y, myMaxY = myPos.y + myScale.y;
    float myMinZ = myPos.z - myScale.z, myMaxZ = myPos.z + myScale.z;

    // 자신과 부딪친 다른 충돌체의 작은값과 큰값
    float otherMinX = otherPos.x - otherScale.x, otherMaxX = otherPos.x + otherScale.x;
    float otherMinY = otherPos.y - otherScale.y, otherMaxY = otherPos.y + otherScale.y;
    float otherMinZ = otherPos.z - otherScale.z, otherMaxZ = otherPos.z + otherScale.z;

    float epsilon = 0.00001f;

    // 겹쳐진 크기
    float overlapX;
    float overlapY;
    float overlapZ;
    
    // 겹쳐져 있지 않다면 리턴
    if (myMaxX <= otherMinX || otherMaxX <= myMinX || myMaxY <= otherMinY || otherMaxY <= myMinY
        || myMaxZ <= otherMinZ || otherMaxZ <= myMinZ)
        return;

    // 충돌체 기준으로 겹쳐져있는 크기를 구해, 가장 작게 겹쳐진 축 기준으로 밀어낸다.
    // 밀어낼 때는 오브젝트 기준으로 밀어냄 (충돌체가 오브젝트를 따라가므로)
    //
    // 겹쳐진 x축 스케일
    if ((myMinX <= otherMinX && otherMaxX <= myMaxX) || (otherMinX <= myMinX && myMaxX <= otherMaxX))
    {
        if (myMinX <= otherMinX && otherMaxX <= myMaxX)
            overlapX = otherMaxX - otherMinX;
        else
            overlapX = myMaxX - myMinX;
    }
    else
    {
        if (otherMinX <= myMaxX && myMaxX <= otherMaxX)
            overlapX = myMaxX - otherMinX;
        else
            overlapX = otherMaxX - myMinX;
    }

    // 겹쳐진 y축 스케일
    if ((myMinY <= otherMinY && otherMaxY <= myMaxY) || (otherMinY <= myMinY && myMaxY <= otherMaxY))
    {
        if (myMinY <= otherMinY && otherMaxY <= myMaxY)
            overlapY = otherMaxY - otherMinY;
        else
            overlapY = myMaxY - myMinY;
    }
    else
    {
        if (otherMinY <= myMaxY && myMaxY <= otherMaxY)
            overlapY = myMaxY - otherMinY;
        else
            overlapY = otherMaxY - myMinY;
    }

    // 겹쳐진 z축 스케일
    if ((myMinZ <= otherMinZ && otherMaxZ <= myMaxZ) || (otherMinZ <= myMinZ && myMaxZ <= otherMaxZ))
    {
        if (myMinZ <= otherMinZ && otherMaxZ <= myMaxZ)
            overlapZ = otherMaxZ - otherMinZ;
        else
            overlapZ = myMaxZ - myMinZ;
    }
    else
    {
        if (otherMinZ <= myMaxZ && myMaxZ <= otherMaxZ)
            overlapZ = myMaxZ - otherMinZ;
        else
            overlapZ = otherMaxZ - myMinZ;
    }

    // 가장 작은 스케일 축 기준으로 밀어냄
    // x축 스케일이 가장 작다면 x축기준으로 밀어냄
    // 밀어낼때는 오브젝트의 포지션을 밀어냄 (epsilon으로 포지션 조정)
    float checkPos;
    if (overlapX <= overlapY && overlapX <= overlapZ)
    {
        checkPos = otherPos.x - myPos.x;

        // 오른쪽에 있다면 오른쪽으로 밀어냄
        if (checkPos >= 0.0f)
        {
            //objectPos.x = myPos.x + myScale.x + otherScale.x;
            objectPos.x += overlapX - epsilon;
        }
        // 왼쪽에 있다면 왼쪽으로 밀어냄
        else
        {
            //objectPos.x = myPos.x - myScale.x - otherScale.x;
            objectPos.x -= overlapX + epsilon;
        }

        // 충돌했을 시 속도를 0으로 만듦
        velocity.x = 0.0f;
    }

    // y축 스케일이 가장 작다면 y축기준으로 밀어냄
    else if (overlapY <= overlapX && overlapY <= overlapZ)
    {
        checkPos = otherPos.y - myPos.y;

        // 위쪽에 있다면 위쪽으로 밀어냄
        if (checkPos >= 0.0f)
        {
            // y축위로 충돌했을 시에는 IsGround를 true로 설정
            //objectPos.y = myPos.y + myScale.y + otherScale.y;
            objectPos.y += overlapY - epsilon;
            otherObj->SetIsGround(true);
        }
        // 아래쪽에 있다면 아래쪽으로 밀어냄
        else
        {
            //objectPos.y = myPos.y - myScale.y - otherScale.y;
            objectPos.y -= overlapY + epsilon;
        }

        // 충돌했을 시 속도를 0으로 만듦
        velocity.y = 0.0f;
    }

    // z축 스케일이 가장 작다면 z축기준으로 밀어냄
    else
    {
        checkPos = otherPos.z - myPos.z;

        // 앞쪽에 있다면 앞쪽으로 밀어냄
        if (checkPos >= 0.0f)
        {
            //objectPos.z = myPos.z + myScale.z + otherScale.z;
            objectPos.z += overlapZ - epsilon;
        }
        // 뒤쪽에 있다면 뒤쪽으로 밀어냄
        else
        {
            //objectPos.z = myPos.z - myScale.z - otherScale.z;
            objectPos.z -= overlapZ + epsilon;
        }

        // 충돌했을 시 속도를 0으로 만듦
        velocity.z = 0.0f;
    }

    otherObj->SetVelocity(velocity);
    otherObj->SetPosition(objectPos);
}