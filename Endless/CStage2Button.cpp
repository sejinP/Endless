#include "global.h"
#include "CStage2Button.h"

#include "CEventMgr.h"

CStage2Button::CStage2Button()
{
	
}

CStage2Button::~CStage2Button()
{
}

void CStage2Button::Update()
{
	UpdateBuffer();
}

void CStage2Button::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CStage2Button::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CStage2Button::MouseClicked()
{
	CEventMgr::Instance().ChangeScene(SCENE_TYPE::STAGE2);
}
