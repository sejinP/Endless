#include "global.h"
#include "CShaderMgr.h"

#include "CCentral.h"

#include "CCommonShader.h"
#include "CDepthShader.h"
#include "CShadowShader.h"
#include "CUIShader.h"
#include "CColliderShader.h"

#include "CLight.h"

CShaderMgr::CShaderMgr()
	: m_commonShader(nullptr)
	, m_depthShader(nullptr)
	, m_shadowShader(nullptr)
	, m_uiShader(nullptr)
	, m_colShader(nullptr)
{
	
}

CShaderMgr::~CShaderMgr()
{
	if (m_commonShader)
		delete m_commonShader;
	if (m_depthShader)
		delete m_depthShader;
	if (m_shadowShader)
		delete m_shadowShader;
	if (m_uiShader)
		delete m_uiShader;
	if (m_colShader)
		delete m_colShader;
}

void CShaderMgr::Init()
{
	ID3D11Device* _device = CCentral::Instance().GetDevice();
	HWND _hwnd = CCentral::Instance().GetWindHandle();

	m_commonShader = new CCommonShader;
	m_commonShader->Init(_device, _hwnd);

	m_depthShader = new CDepthShader;
	m_depthShader->Init(_device, _hwnd);

	m_shadowShader = new CShadowShader;
	m_shadowShader->Init(_device, _hwnd);

	m_uiShader = new CUIShader;
	m_uiShader->Init(_device, _hwnd);

	m_colShader = new CColliderShader;
	m_colShader->Init(_device, _hwnd);
}

void CShaderMgr::RenderCommonShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
	CLight* _light, ID3D11ShaderResourceView* _texture, XMFLOAT3 _cameraPosition, UINT _indexCount)
{
	m_commonShader->Render(_deviceContext, _transformMatrix, _light, _texture, _cameraPosition, _indexCount);
}

void CShaderMgr::RenderDepthShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix, UINT _indexCount)
{
	m_depthShader->Render(_deviceContext, _transformMatrix, _indexCount);
}

void CShaderMgr::RenderShadowShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
	ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light, UINT _indexCount)
{
	m_shadowShader->Render(_deviceContext, _transformMatrix, _texture, _depthMapTexture, _light, _indexCount);
}

void CShaderMgr::RenderUIShader(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix,
	ID3D11ShaderResourceView* _texture, UINT _indexCount)
{
	m_uiShader->Render(_deviceContext, _transformMatrix, _texture, _indexCount);
}

void CShaderMgr::RenderColliderShader(ID3D11DeviceContext* _deviceContext,
	TransformBufferType _transformMatrix, UINT _indexCount)
{
	m_colShader->Render(_deviceContext, _transformMatrix, _indexCount);
}
