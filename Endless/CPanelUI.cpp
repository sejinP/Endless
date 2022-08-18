#include "global.h"
#include "CPanelUI.h"

#include "CKeyMgr.h"

CPanelUI::CPanelUI()
{
	
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::Update()
{
	UpdateBuffer();
}

void CPanelUI::Render(ID3D11DeviceContext* _deviceContext)
{
	CUI::Render(_deviceContext);
}

void CPanelUI::UpdateBuffer()
{
	CUI::UpdateBuffer();
}

void CPanelUI::MouseClicked()
{

}
