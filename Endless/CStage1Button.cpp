#include "global.h"
#include "CStage1Button.h"

#include "CEventMgr.h"

CStage1Button::CStage1Button()
{
	
}

CStage1Button::~CStage1Button()
{
}

void CStage1Button::Update()
{
	UpdateBuffer();
}

void CStage1Button::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CStage1Button::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CStage1Button::MouseClicked()
{
	CEventMgr::Instance().ChangeScene(SCENE_TYPE::STAGE1);
}
