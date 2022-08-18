#include "global.h"
#include "CBufferMgr.h"

#include "CColliderBuffer.h"
#include "CObjectBuffer.h"
#include "CUIBuffer.h"

CBufferMgr::CBufferMgr()
	: m_colliderBuffer(nullptr)
	, m_monsterBuffer(nullptr)
	, m_objectBuffer(nullptr)
	, m_bulletBuffer(nullptr)
	, m_uiBuffer(nullptr)
{
}

CBufferMgr::~CBufferMgr()
{
	if (nullptr != m_colliderBuffer)
		delete m_colliderBuffer;

	if (nullptr != m_monsterBuffer)
		delete m_monsterBuffer;

	if (nullptr != m_objectBuffer)
		delete m_objectBuffer;

	if (nullptr != m_bulletBuffer)
		delete m_bulletBuffer;

	if (nullptr != m_uiBuffer)
		delete m_uiBuffer;
}

CColliderBuffer* CBufferMgr::GetColliderBuffer()
{
	// 이미 있다면 있는걸 반환
	if (nullptr != m_colliderBuffer)
		return m_colliderBuffer;

	m_colliderBuffer = new CColliderBuffer;
	m_colliderBuffer->Init();
	return m_colliderBuffer;
}

CObjectBuffer* CBufferMgr::GetMonsterBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx)
{
	// 이미 있다면 있는걸 반환
	if (nullptr != m_monsterBuffer)
		return m_monsterBuffer;

	m_monsterBuffer = new CObjectBuffer;
	m_monsterBuffer->Init(_modelVtx, _modelIdx);
	return m_monsterBuffer;
}

CObjectBuffer* CBufferMgr::GetObjectBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx)
{
	// 이미 있다면 있는걸 반환
	if (nullptr != m_objectBuffer)
		return m_objectBuffer;

	m_objectBuffer = new CObjectBuffer;
	m_objectBuffer->Init(_modelVtx, _modelIdx);
	return m_objectBuffer;
}

CObjectBuffer* CBufferMgr::GetBulletBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx)
{
	// 이미 있다면 있는걸 반환
	if (nullptr != m_bulletBuffer)
		return m_bulletBuffer;

	m_bulletBuffer = new CObjectBuffer;
	m_bulletBuffer->Init(_modelVtx, _modelIdx);
	return m_bulletBuffer;
}

CUIBuffer* CBufferMgr::GetUIBuffer(UINT _vertexCount, UINT _indexCount)
{
	// 이미 있다면 있는걸 반환
	if (nullptr != m_uiBuffer)
		return m_uiBuffer;

	m_uiBuffer = new CUIBuffer;
	m_uiBuffer->Init(_vertexCount, _indexCount);
	return m_uiBuffer;
}
