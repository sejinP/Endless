#include "global.h"
#include "CUIMgr.h"

#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CUI.h"

CUIMgr::CUIMgr()
	: m_targetedUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::Update()
{
	// 클릭된 UI가 없다면
	if (nullptr == m_targetedUI)
	{
		// 마우스 왼쪽 버튼 탭을 감지하여 탭을 했다면
		// 탭을 한 순간의 마우스 포지션에 해당하는 UI를 가져온다.
		if (CKeyMgr::Instance().CheckMouseState(MOUSE_BUTTON::LEFT, KEY_STATE::TAP))
		{
			m_targetedUI = GetTargetUI();
		}
	}
	// 클릭된 UI가 있다면
	else
	{
		// 마우스 왼쪽 버튼 어웨이를 감지하여 어웨이 상태라면
		// 그때의 마우스 포지션에 해당하는 UI와 탭을 했던 UI를 비교하여
		// 같다면 마우스 클릭 이벤트 발생
		if (CKeyMgr::Instance().CheckMouseState(MOUSE_BUTTON::LEFT, KEY_STATE::AWAY))
		{
			CUI* awayUI = GetTargetUI();
			if (m_targetedUI == awayUI)
			{
				m_targetedUI->MouseClicked();
			}
			m_targetedUI = nullptr;
		}
	}
}

CUI* CUIMgr::GetTargetUI()
{
	const vector<CUI*> uis = CSceneMgr::Instance().GetCurrentScene()->GetUI();
	CUI* targetedUI = nullptr;

	XMFLOAT2 mousePos = CKeyMgr::Instance().GetMousePos();
	float mousePosX = mousePos.x;
	float mousePosY = mousePos.y;

	queue<CUI*> uiQueue;
	for (size_t i = 0; i < uis.size(); ++i)
	{
		uiQueue.push(uis[i]);
	}

	while (!uiQueue.empty())
	{
		CUI* temp = uiQueue.front();
		uiQueue.pop();

		// 활성화 상태일때만
		if (temp->GetVisualize())
		{
			if (temp->CheckIsMouseOn(mousePosX, mousePosY))
			{
				targetedUI = temp;
				const vector<CUI*> childUIs = temp->GetChildUI();
				for (size_t i = 0; i < childUIs.size(); ++i)
				{
					uiQueue.push(childUIs[i]);
				}
			}
		}
	}

	return targetedUI;
}
