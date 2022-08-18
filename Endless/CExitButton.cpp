#include "global.h"
#include "CExitButton.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CCentral.h"

CExitButton::CExitButton()
{
	// 텍스쳐 생성
	CTexture* texture = CTextureMgr::Instance().GetTexture(L"exit.dds");
	SetTexture(texture);
}

CExitButton::~CExitButton()
{
}

void CExitButton::Update()
{
	UpdateBuffer();
}

void CExitButton::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CExitButton::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CExitButton::MouseClicked()
{
	CCentral::Instance().GameExit();
}
