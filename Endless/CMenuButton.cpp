#include "global.h"
#include "CMenuButton.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CEventMgr.h"

CMenuButton::CMenuButton()
{
	// 텍스쳐 생성
	CTexture* texture = CTextureMgr::Instance().GetTexture(L"menu.dds");
	SetTexture(texture);
}

CMenuButton::~CMenuButton()
{
}

void CMenuButton::Update()
{
	UpdateBuffer();
}

void CMenuButton::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CMenuButton::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CMenuButton::MouseClicked()
{
	CEventMgr::Instance().ChangeScene(SCENE_TYPE::START);
}
