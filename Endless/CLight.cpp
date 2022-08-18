#include "global.h"
#include "CLight.h"

#include "CCentral.h"

CLight::CLight()
	: m_ambientColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
	, m_diffuseColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
	, m_lightDirection(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_position(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_lookAt(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_viewMatrix(XMMATRIX())
	, m_projectionMatrix(XMMATRIX())
	, m_specularPower(0.0f)
{
	
}

CLight::~CLight()
{
}

XMMATRIX CLight::GetViewMatrix()
{
	// upº¤ÅÍ, Æ÷Áö¼Ç, lookAtº¤ÅÍ
	XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR positionVec = XMLoadFloat3(&m_position);
	XMVECTOR lookAtVec = XMLoadFloat3(&m_lookAt);

	m_viewMatrix = XMMatrixLookAtLH(positionVec, lookAtVec, upVec);

	return m_viewMatrix;
}

void CLight::GenerateProjectionMatrix()
{
	// Á¤»ç°¢Çü ±¤¿ø
	float screenAspect = 1.0f;
	float nearZ = CCentral::Instance().GetNearZ();
	float farZ = CCentral::Instance().GetFarZ();

	m_projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, screenAspect, nearZ, farZ);
}