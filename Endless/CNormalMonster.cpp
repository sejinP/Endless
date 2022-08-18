#include "global.h"
#include "CNormalMonster.h"

#include "CTextureMgr.h"
#include "CTexture.h"

#include "CBufferMgr.h"
#include "CObjectBuffer.h"

CNormalMonster::CNormalMonster()
{
	// 모델 로드, 텍스쳐 정보, 버퍼 생성
	LoadModel(L"skull.fbx");

	CTexture* texture = CTextureMgr::Instance().GetTexture(L"ebony.dds");
	SetTexture(texture);

	CObjectBuffer* buffer = CBufferMgr::Instance().GetMonsterBuffer(GetModelVtx(), GetModelIdx());
	SetBuffer(buffer);
}

CNormalMonster::CNormalMonster(const CNormalMonster& _other)
	: CMonster(_other)
{
}

CNormalMonster::~CNormalMonster()
{
}

void CNormalMonster::Update()
{
	CMonster::Update();
}

void CNormalMonster::DepthRender(ID3D11DeviceContext* _deviceContext)
{
	CMonster::DepthRender(_deviceContext);
}

void CNormalMonster::Render(ID3D11DeviceContext* _deviceContext)
{
	CMonster::Render(_deviceContext);
}

void CNormalMonster::OnCollisionEnter(CCollider* _otherCol)
{
	CMonster::OnCollisionEnter(_otherCol);
}

void CNormalMonster::OnCollision(CCollider* _otherCol)
{
	CMonster::OnCollision(_otherCol);
}

void CNormalMonster::OnCollisionExit(CCollider* _otherCol)
{
	CMonster::OnCollisionExit(_otherCol);
}
