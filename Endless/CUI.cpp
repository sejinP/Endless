#include "global.h"
#include "CUI.h"

#include "CCentral.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CUIBuffer.h"

#include "CShaderMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CCamera.h"

CUI::CUI()
	: m_uiWidth(0)
	, m_uiHeight(0)
	, m_pos(XMFLOAT2(0.0f, 0.0f))
	, m_realPos(XMFLOAT2(0.0f, 0.0f))
	, m_vertexCount(6)	// 삼각형 2개 필요하므로 6개
	, m_indexCount(6)
	, m_uiBuffer(nullptr)
	, m_texture(nullptr)
	, m_parentUI(nullptr)
	, m_childUIs()
	, m_visualized(false)
{
	m_uiBuffer = new CUIBuffer;
	m_uiBuffer->Init(m_vertexCount, m_indexCount);
}

CUI::~CUI()
{
	if (m_uiBuffer)
		delete m_uiBuffer;

	for (size_t i = 0; i < m_childUIs.size(); ++i)
	{
		if (nullptr != m_childUIs[i])
			delete m_childUIs[i];
	}
	m_childUIs.clear();
}

void CUI::Update()
{
	UpdateBuffer();
}

void CUI::Render(ID3D11DeviceContext* _deviceContext)
{
	CCamera* camera = CSceneMgr::Instance().GetCurrentScene()->GetCamera();

	XMMATRIX worldMatrix = camera->GetUIWorldMatrix();
	XMMATRIX viewMatrix = camera->GetViewMatrix();
	XMMATRIX projectionMatrix = CCentral::Instance().GetOrthogonalMatrix();

	TransformBufferType constantBuffer;
	constantBuffer.worldMatrix = worldMatrix;
	constantBuffer.viewMatrix = viewMatrix;
	constantBuffer.projectionMatrix = projectionMatrix;

	m_uiBuffer->BufferSetting(_deviceContext);
	CShaderMgr::Instance().RenderUIShader(_deviceContext, constantBuffer, m_texture->GetTextureView(), m_indexCount);

	for (size_t i = 0; i < m_childUIs.size(); ++i)
	{
		m_childUIs[i]->Render(_deviceContext);
	}
}

void CUI::UpdateBuffer()
{
	m_uiBuffer->UpdateBuffer(this);
	
	for (size_t i = 0; i < m_childUIs.size(); ++i)
	{
		m_childUIs[i]->UpdateBuffer();
	}
}

void CUI::MouseClicked()
{
	
}

bool CUI::CheckIsMouseOn(float mousePosX, float mousePosY)
{
	float realPosX = m_realPos.x;
	float realPosY = m_realPos.y;

	if (realPosX <= mousePosX && mousePosX <= realPosX + m_uiWidth &&
		realPosY <= mousePosY && mousePosY <= realPosY + m_uiHeight)
	{
		return true;
	}

	return false;
}

void CUI::SetVisualize(bool _visualize)
{
	// 자기 자신과, 자식들도 똑같이 설정
	m_visualized = _visualize;
	for (size_t i = 0; i < m_childUIs.size(); ++i)
	{
		m_childUIs[i]->SetVisualize(_visualize);
	}
}