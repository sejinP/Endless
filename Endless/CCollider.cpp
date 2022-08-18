#include "global.h"
#include "CCollider.h"

#include "CCentral.h"

#include "CColliderBuffer.h"
#include "CBufferMgr.h"

#include "CShaderMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "CObject.h"
#include "CCamera.h"

CCollider::CCollider()
	: m_buffer(nullptr)
	, m_position(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_scale(XMFLOAT3(1.0f, 1.0f, 1.0f))
	, m_rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_offset(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_colliderID(m_uniqueID++)
{
	m_buffer = CBufferMgr::Instance().GetColliderBuffer();
}

CCollider::CCollider(const CCollider& _other)
	: m_buffer(_other.m_buffer)
	, m_position(_other.m_position)
	, m_scale(_other.m_scale)
	, m_rotation(_other.m_rotation)
	, m_offset(_other.m_offset)
	, m_colliderID(m_uniqueID++)
{
}

CCollider::~CCollider()
{
	
}

UINT CCollider::m_uniqueID = 0;

void CCollider::Update()
{
	// owner의 포지션을 따라간다.
	CObject* owner = GetOwner();
	XMFLOAT3 ownerPos = owner->GetPosition();
	m_position = ownerPos;
}

void CCollider::Render(ID3D11DeviceContext* _deviceContext)
{
	CCamera* camera = CSceneMgr::Instance().GetCurrentScene()->GetCamera();

	// 월드 행렬
	XMMATRIX worldMatrix = GetWorldMatrix();

	// 뷰 행렬
	XMMATRIX viewMatrix = camera->GetViewMatrix();

	// 투영 행렬
	XMMATRIX projectionMatrix = CCentral::Instance().GetProjectionMatrix();

	// 상수버퍼 설정
	TransformBufferType transformMatrix;
	transformMatrix.worldMatrix = worldMatrix;
	transformMatrix.viewMatrix = viewMatrix;
	transformMatrix.projectionMatrix = projectionMatrix;

	// 버퍼 세팅 후 셰이더를 통해 렌더
	m_buffer->BufferSetting(_deviceContext);
	CShaderMgr::Instance().RenderColliderShader(_deviceContext, transformMatrix, 16);
}

XMMATRIX CCollider::GetWorldMatrix()
{
	XMMATRIX scaleMatrix = GetScaleMatrix();
	XMMATRIX rotationMatrix = GetRotationMatrix();
	XMMATRIX translateMatrix = GetTranslateMatrix();

	return scaleMatrix * rotationMatrix * translateMatrix;
}

XMMATRIX CCollider::GetScaleMatrix()
{
	// 스케일 행렬
	XMMATRIX scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	return scaleMatrix;
}

XMMATRIX CCollider::GetRotationMatrix()
{
	// 회전 행렬 (라디안으로 변환하여 계산)
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_rotation.x),
			XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));
	
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);

	return rotationMatrix;
}

XMMATRIX CCollider::GetTranslateMatrix()
{
	// 이동 행렬
	XMFLOAT3 position = GetPosition();
	XMMATRIX positionMatrix = XMMatrixTranslation(position.x,
		position.y, position.z);

	return positionMatrix;
}

void CCollider::OnCollisionEnter(CCollider* _otherCol)
{
	CObject* owner = GetOwner();
	owner->OnCollisionEnter(_otherCol);
}

void CCollider::OnCollision(CCollider* _otherCol)
{
	CObject* owner = GetOwner();
	owner->OnCollision(_otherCol);
}

void CCollider::OnCollisionExit(CCollider* _otherCol)
{
	CObject* owner = GetOwner();
	owner->OnCollisionExit(_otherCol);
}