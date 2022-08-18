#include "global.h"
#include "CStage3Button.h"

#include "CEventMgr.h"

CStage3Button::CStage3Button()
{
	
}

CStage3Button::~CStage3Button()
{
}

void CStage3Button::Update()
{
	UpdateBuffer();
}

void CStage3Button::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CStage3Button::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CStage3Button::MouseClicked()
{
	CEventMgr::Instance().ChangeScene(SCENE_TYPE::STAGE3);
}
