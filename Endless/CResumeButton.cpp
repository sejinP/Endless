#include "global.h"
#include "CResumeButton.h"

#include "CTextureMgr.h"
#include "CTexture.h"

CResumeButton::CResumeButton()
{
	// 텍스쳐 생성
	CTexture* texture = CTextureMgr::Instance().GetTexture(L"resume.dds");
	SetTexture(texture);
}

CResumeButton::~CResumeButton()
{
}

void CResumeButton::Update()
{
	UpdateBuffer();
}

void CResumeButton::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CResumeButton::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CResumeButton::MouseClicked()
{
	GetParentUI()->SetVisualize(false);
	ShowCursor(false);
}
