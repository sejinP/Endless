#include "global.h"
#include "CStage1Scene.h"

#include "CCentral.h"

#include "CPlayer.h"
#include "CEpicMonster.h"
#include "CNormalMonster.h"
#include "CMonster.h"
#include "CPlane.h"
#include "CWall.h"

#include "CLight.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CResumeButton.h"
#include "CMenuButton.h"
#include "CStage1Button.h"
#include "CStage2Button.h"
#include "CExitButton.h"

#include "CIdleState.h"
#include "CPatrolState.h"
#include "CChaseState.h"
#include "CAttackState.h"

#include "CDepthTexture.h"

#include "CCollisionMgr.h"
#include "CGameMgr.h"
#include "CTextureMgr.h"
#include "CTexture.h"

CStage1Scene::CStage1Scene()
{
}

CStage1Scene::~CStage1Scene()
{
}

void CStage1Scene::Update()
{
	CScene::Update();
}

void CStage1Scene::Render(ID3D11DeviceContext* _deviceContext)
{
	CScene::Render(_deviceContext);
}

void CStage1Scene::Enter()
{
	CScene::Enter();

	ID3D11Device* device = CCentral::Instance().GetDevice();

	// 오브젝트 생성과 빛 설정.
	// 플레이어
	CPlayer* player = new CPlayer;
	player->SetScale(XMFLOAT3(0.5f, 1.0f, 0.5f));
	player->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetPosition(XMFLOAT3(0.0f, 3.0f, 0.0f));
	player->SetMaxSpeed(5.0f);
	player->SetSpeed(30.0f);
	player->SetHP(100.0f);
	player->SetOffensivePower(20.0f);
	PushObject(player, OBJECT_TYPE::PLAYER);
	SetPlayer(player);
	CCollider* collider = new CCollider;
	collider->SetOwner(player);
	collider->SetScale(XMFLOAT3(0.5f, 1.0f, 0.5f));
	collider->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
	player->SetComponent(collider, COMPONENT_TYPE::COLLIDER);

	// 카메라	
	CCamera* myCamera = new CCamera;
	myCamera->SetOwner(player);
	myCamera->SetCamPos(XMFLOAT3(0.0f, 3.0f, -5.0f));
	SetCamera(myCamera);

	// 구조물 생성
	CreateStructure();

	// 몬스터 생성
	CreateMonster();

	// 총알 생성
	InitBullet(50);

	// 빛 세팅
	CLight* light = new CLight;
	light->SetAmbientColor(0.8f, 0.8f, 0.8f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetLightDirection(0.0f, -1.0f, 0.0f);
	light->SetSpecularPower(64.0f);
	light->SetLightPos(0.0f, 20.0f, -1.0f);
	light->SetLightLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateProjectionMatrix();
	SetLight(light);

	// 그림자 렌더를 위한 깊이 텍스쳐 생성
	CDepthTexture* depthTexture = new CDepthTexture;
	depthTexture->Init(device);
	SetDepthTexture(depthTexture);

	// 충돌 설정
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::STRUCTURE, OBJECT_TYPE::PLAYER);
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::STRUCTURE, OBJECT_TYPE::MONSTER);
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::BULLET, OBJECT_TYPE::STRUCTURE);
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::PLAYER, OBJECT_TYPE::BULLET);
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::MONSTER, OBJECT_TYPE::BULLET);
	CCollisionMgr::Instance().SetColCheck(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MONSTER);

	// UI 생성
	CreateUI();

	SetGameState(GAME_STATE::PLAYING);

	// 커서 숨김
	ShowCursor(false);
}

void CStage1Scene::Exit()
{
	CScene::Exit();
}

void CStage1Scene::CreateStructure()
{
	// 바닥
	CPlane* plane = new CPlane;
	plane->SetScale(XMFLOAT3(12.0f, 0.05f, 12.0f));
	plane->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	plane->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	PushObject(plane, OBJECT_TYPE::STRUCTURE);
	// 바닥 충돌체
	CCollider* planeCollider = new CCollider;
	planeCollider->SetOwner(plane);
	planeCollider->SetScale(XMFLOAT3(12.0f, 0.05f, 12.0f));
	plane->SetComponent(planeCollider, COMPONENT_TYPE::COLLIDER);

	// 벽 생성
	CWall* wall = new CWall;
	wall->SetScale(XMFLOAT3(12.0f, 2.0f, 1.0f));
	wall->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	wall->SetPosition(XMFLOAT3(0.0f, 2.0f, 13.0f));
	// 벽 충돌체
	CCollider* wallCollider = new CCollider;
	wallCollider->SetOwner(wall);
	wallCollider->SetScale(XMFLOAT3(12.0f, 2.0f, 1.0f));
	wall->SetComponent(wallCollider, COMPONENT_TYPE::COLLIDER);
	PushObject(wall, OBJECT_TYPE::STRUCTURE);

	CWall* wall1 = new CWall(*wall);
	wall1->SetScale(XMFLOAT3(12.0f, 2.0f, 1.0f));
	wall1->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	wall1->SetPosition(XMFLOAT3(0.0f, 2.0f, -13.0f));
	CCollider* wall1Collider = wall1->GetCollider();
	wall1Collider->SetScale(XMFLOAT3(12.0f, 2.0f, 1.0f));
	PushObject(wall1, OBJECT_TYPE::STRUCTURE);

	CWall* wall2 = new CWall(*wall);
	wall2->SetScale(XMFLOAT3(1.0f, 2.0f, 12.0f));
	wall2->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	wall2->SetPosition(XMFLOAT3(-13.0f, 2.0f, 0.0f));
	CCollider* wall2Collider = wall2->GetCollider();
	wall2Collider->SetScale(XMFLOAT3(1.0f, 2.0f, 12.0f));
	PushObject(wall2, OBJECT_TYPE::STRUCTURE);

	CWall* wall3 = new CWall(*wall);
	wall3->SetScale(XMFLOAT3(1.0f, 2.0f, 12.0f));
	wall3->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	wall3->SetPosition(XMFLOAT3(13.0f, 2.0f, 0.0f));
	CCollider* wall3Collider = wall3->GetCollider();
	wall3Collider->SetScale(XMFLOAT3(1.0f, 2.0f, 12.0f));
	PushObject(wall3, OBJECT_TYPE::STRUCTURE);
}

void CStage1Scene::CreateMonster()
{
	// 일반 몬스터
	CNormalMonster* normMonster = new CNormalMonster;
	normMonster->SetPosition(XMFLOAT3(0.0f, 2.0f, 7.0f));
	normMonster->SetScale(XMFLOAT3(0.005f, 0.005f, 0.005f));
	normMonster->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	normMonster->SetSensingDist(8.0f);
	normMonster->SetAttackDist(4.0f);
	normMonster->SetSpeed(2.0f);
	normMonster->SetRotationSpeed(250.0f);
	normMonster->SetHP(50.0f);
	normMonster->SetOffensivePower(5.0f);
	// 몬스터 충돌체
	CCollider* monsterCollider = new CCollider;
	monsterCollider->SetOwner(normMonster);
	monsterCollider->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	normMonster->SetComponent(monsterCollider, COMPONENT_TYPE::COLLIDER);
	// 몬스터 상태
	CState** states = normMonster->GetStates();
	((CIdleState*)states[(size_t)MONSTER_STATE::IDLE])->SetStayTime(2.0f);
	((CPatrolState*)states[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(-2.0f, 0.0f, 7.0f), XMFLOAT3(2.0f, 0.0f, 7.0f));
	((CAttackState*)states[(size_t)MONSTER_STATE::ATTACK])->SetAttackSpeed(1.0f);
	PushObject(normMonster, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster1 = new CNormalMonster(*normMonster);
	normMonster1->SetPosition(XMFLOAT3(-5.5f, 2.0f, 5.5f));
	CState** states1 = normMonster1->GetStates();
	((CPatrolState*)states1[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(-5.5f, 0.0f, 7.0f), XMFLOAT3(-5.5f, 0.0f, 4.0f));
	PushObject(normMonster1, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster2 = new CNormalMonster(*normMonster);
	normMonster2->SetPosition(XMFLOAT3(-7.0f, 2.0f, 0.0f));
	CState** states2 = normMonster2->GetStates();
	((CPatrolState*)states2[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(-8.0f, 0.0f, -1.0f), XMFLOAT3(-6.0f, 0.0f, 1.0f));
	PushObject(normMonster2, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster3 = new CNormalMonster(*normMonster);
	normMonster3->SetPosition(XMFLOAT3(-5.5f, 2.0f, -5.5f));
	CState** states3 = normMonster3->GetStates();
	((CPatrolState*)states3[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(-6.5f, 0.0f, -5.5f), XMFLOAT3(-4.5f, 0.0f, -5.5f));
	PushObject(normMonster3, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster4 = new CNormalMonster(*normMonster);
	normMonster4->SetPosition(XMFLOAT3(0.0f, 2.0f, -7.0f));
	CState** states4 = normMonster4->GetStates();
	((CPatrolState*)states4[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(-2.0f, 0.0f, -7.0f), XMFLOAT3(2.0f, 0.0f, -7.0f));
	PushObject(normMonster4, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster5 = new CNormalMonster(*normMonster);
	normMonster5->SetPosition(XMFLOAT3(5.5f, 2.0f, -5.5f));
	CState** states5 = normMonster5->GetStates();
	((CPatrolState*)states5[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(4.5f, 0.0f, -4.5f), XMFLOAT3(6.5f, 0.0f, -6.5f));
	PushObject(normMonster5, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster6 = new CNormalMonster(*normMonster);
	normMonster6->SetPosition(XMFLOAT3(7.0f, 2.0f, 0.0f));
	CState** states6 = normMonster6->GetStates();
	((CPatrolState*)states6[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(6.0f, 0.0f, 0.0f), XMFLOAT3(8.0f, 0.0f, 0.0f));
	PushObject(normMonster6, OBJECT_TYPE::MONSTER);

	CNormalMonster* normMonster7 = new CNormalMonster(*normMonster);
	normMonster7->SetPosition(XMFLOAT3(5.5f, 2.0f, 5.5f));
	CState** states7 = normMonster7->GetStates();
	((CPatrolState*)states7[(size_t)MONSTER_STATE::PATROL])->SetPatrolPos(XMFLOAT3(6.5f, 0.0f, 6.5f), XMFLOAT3(4.5f, 0.0f, 4.5f));
	PushObject(normMonster7, OBJECT_TYPE::MONSTER);
}

void CStage1Scene::CreateUI()
{
	CTexture* texture;

	int screenWidth = CCentral::Instance().GetScreenWidth();
	int screenHeight = CCentral::Instance().GetScreenHeight();

	// -----------------------------------------------------
	// -----------------------------------------------------
	CPanelUI* panelUI = new CPanelUI;
	panelUI->SetUIPos(XMFLOAT2(screenWidth / 2.0f - 200.0f, screenHeight / 2.0f - 125.0f));
	panelUI->SetUIWidth(400);
	panelUI->SetUIHeight(250);
	texture = CTextureMgr::Instance().GetTexture(L"black.dds");
	panelUI->SetTexture(texture);

	CResumeButton* resumeButton = new CResumeButton;
	resumeButton->SetUIPos(XMFLOAT2(70.0f, 140.0f));
	resumeButton->SetUIWidth(50);
	resumeButton->SetUIHeight(50);
	resumeButton->SetParentUI(panelUI);

	CMenuButton* menuButton = new CMenuButton;
	menuButton->SetUIPos(XMFLOAT2(175.0f, 140.0f));
	menuButton->SetUIWidth(50);
	menuButton->SetUIHeight(50);
	menuButton->SetParentUI(panelUI);

	CExitButton* exitButton = new CExitButton;
	exitButton->SetUIPos(XMFLOAT2(275.0f, 140.0f));
	exitButton->SetUIWidth(50);
	exitButton->SetUIHeight(50);
	exitButton->SetParentUI(panelUI);

	panelUI->PushChildUI(menuButton);
	panelUI->PushChildUI(resumeButton);
	panelUI->PushChildUI(exitButton);
	
	PushUI(panelUI);
	panelUI->SetVisualize(false);

	SetMenuPanel(panelUI);
	// -----------------------------------------------------
	// -----------------------------------------------------


	// -----------------------------------------------------
	// -----------------------------------------------------
	CPanelUI* clearPanel = new CPanelUI;
	clearPanel->SetUIPos(XMFLOAT2(screenWidth / 2.0f - 200.0f, screenHeight / 2.0f - 125.0f));
	clearPanel->SetUIWidth(400);
	clearPanel->SetUIHeight(250);
	texture = CTextureMgr::Instance().GetTexture(L"clear.dds");
	clearPanel->SetTexture(texture);

	CMenuButton* menuButton1 = new CMenuButton;
	menuButton1->SetUIPos(XMFLOAT2(70.0f, 140.0f));
	menuButton1->SetUIWidth(50);
	menuButton1->SetUIHeight(50);
	menuButton1->SetParentUI(clearPanel);

	CStage2Button* stage2Button = new CStage2Button;
	stage2Button->SetUIPos(XMFLOAT2(275.0f, 140.0f));
	stage2Button->SetUIWidth(50);
	stage2Button->SetUIHeight(50);
	stage2Button->SetParentUI(clearPanel);
	texture = CTextureMgr::Instance().GetTexture(L"next.dds");
	stage2Button->SetTexture(texture);

	clearPanel->PushChildUI(menuButton1);
	clearPanel->PushChildUI(stage2Button);

	PushUI(clearPanel);
	clearPanel->SetVisualize(false);

	CGameMgr::Instance().SetGameClearPanel(clearPanel);
	// -----------------------------------------------------
	// -----------------------------------------------------


	// -----------------------------------------------------
	// -----------------------------------------------------
	CPanelUI* overPanel = new CPanelUI;
	overPanel->SetUIPos(XMFLOAT2(screenWidth / 2.0f - 200.0f, screenHeight / 2.0f - 125.0f));
	overPanel->SetUIWidth(400);
	overPanel->SetUIHeight(250);
	texture = CTextureMgr::Instance().GetTexture(L"gameover.dds");
	overPanel->SetTexture(texture);

	CStage1Button* stage1Button = new CStage1Button;
	stage1Button->SetUIPos(XMFLOAT2(70.0f, 140.0f));
	stage1Button->SetUIWidth(50);
	stage1Button->SetUIHeight(50);
	stage1Button->SetParentUI(overPanel);
	texture = CTextureMgr::Instance().GetTexture(L"again.dds");
	stage1Button->SetTexture(texture);

	CMenuButton* menuButton2 = new CMenuButton;
	menuButton2->SetUIPos(XMFLOAT2(275.0f, 140.0f));
	menuButton2->SetUIWidth(50);
	menuButton2->SetUIHeight(50);
	menuButton2->SetParentUI(overPanel);

	overPanel->PushChildUI(stage1Button);
	overPanel->PushChildUI(menuButton2);

	PushUI(overPanel);
	overPanel->SetVisualize(false);

	CGameMgr::Instance().SetGameOverPanel(overPanel);
	// -----------------------------------------------------
	// -----------------------------------------------------
}
