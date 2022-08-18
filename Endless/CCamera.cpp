#include "global.h"
#include "CCamera.h"

#include "CObject.h"

CCamera::CCamera()
	: m_position(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_up(XMFLOAT3(0.0f, 1.0f, 0.0f))
	, m_offset(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_camOwner(nullptr)
{
}

CCamera::~CCamera()
{
	
}

void CCamera::Update()
{	
	if (nullptr != m_camOwner)
	{
		m_position = m_camOwner->GetPosition();
		m_rotation = m_camOwner->GetRotation();
	}
}

XMMATRIX CCamera::GetViewMatrix()
{
	XMVECTOR posVec = XMLoadFloat3(&m_position);
	XMVECTOR lookAtVec = GetCamLookAt();
	XMVECTOR upVec = XMLoadFloat3(&m_up);

	// 뷰 매트릭스 생성하여 반환
	return XMMatrixLookAtLH(posVec, lookAtVec, upVec);
}

XMMATRIX CCamera::GetUIWorldMatrix()
{
	XMVECTOR posVec = XMLoadFloat3(&m_position);
	XMVECTOR lookAtVec = GetCamLookAt();
	XMVECTOR upVec = XMLoadFloat3(&m_up);

	// 포워드 벡터
	XMVECTOR forwardVec = lookAtVec - posVec;
	forwardVec = XMVector3Normalize(forwardVec);

	// 회전 행렬.
	XMMATRIX rotationMatrix = GetRotationMat();

	// 이동 행렬. forwardVec만큼 이동함(앞쪽으로)
	XMMATRIX translateMatrix = XMMatrixTranslation(XMVectorGetX(posVec + forwardVec),
		XMVectorGetY(posVec + forwardVec), XMVectorGetZ(posVec + forwardVec));
	return rotationMatrix * translateMatrix;
}

XMVECTOR CCamera::GetCamLookAt()
{
	XMMATRIX rotationMat = GetRotationMat();

	// 회전 행렬의 z축 방향이 보고있는 방향
	XMVECTOR forwardVec = rotationMat.r[2];
	XMVECTOR posVec = XMLoadFloat3(&m_position);
	return (posVec + forwardVec);
}

XMMATRIX CCamera::GetRotationMat()
{
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_rotation.x),
		XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));
	XMMATRIX rotationMat = XMMatrixRotationQuaternion(rotationQuat);

	return rotationMat;
}