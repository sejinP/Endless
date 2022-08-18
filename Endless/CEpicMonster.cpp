#include "global.h"
#include "CEpicMonster.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CBufferMgr.h"
#include "CObjectBuffer.h"

CEpicMonster::CEpicMonster()
{
	// 모델 로드, 텍스쳐 정보, 버퍼 생성
	LoadModel(L"skull.fbx");

	CTexture* texture = CTextureMgr::Instance().GetTexture(L"red.dds");
	SetTexture(texture);

	CObjectBuffer* buffer = CBufferMgr::Instance().GetMonsterBuffer(GetModelVtx(), GetModelIdx());
	SetBuffer(buffer);
}

CEpicMonster::CEpicMonster(const CEpicMonster& _other)
	: CMonster(_other)
{
}

CEpicMonster::~CEpicMonster()
{
}

void CEpicMonster::Update()
{
	CMonster::Update();
}

void CEpicMonster::DepthRender(ID3D11DeviceContext* _deviceContext)
{
	CMonster::DepthRender(_deviceContext);
}

void CEpicMonster::Render(ID3D11DeviceContext* _deviceContext)
{
	CMonster::Render(_deviceContext);
}

void CEpicMonster::OnCollisionEnter(CCollider* _otherCol)
{
	CMonster::OnCollisionEnter(_otherCol);
}

void CEpicMonster::OnCollision(CCollider* _otherCol)
{
	CMonster::OnCollision(_otherCol);
}

void CEpicMonster::OnCollisionExit(CCollider* _otherCol)
{
	CMonster::OnCollisionExit(_otherCol);
}
