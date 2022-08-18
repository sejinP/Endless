#pragma once

class CColliderBuffer;
class CObjectBuffer;
class CUIBuffer;

class CBufferMgr
{
public:
	static CBufferMgr& Instance() { static CBufferMgr instance; return instance; }

private:
	CColliderBuffer*	m_colliderBuffer;
	CObjectBuffer*		m_monsterBuffer;
	CObjectBuffer*		m_objectBuffer;
	CObjectBuffer*		m_bulletBuffer;
	CUIBuffer*			m_uiBuffer;

public:
	CColliderBuffer* GetColliderBuffer();
	CObjectBuffer* GetMonsterBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx);
	CObjectBuffer* GetObjectBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx);
	CObjectBuffer* GetBulletBuffer(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx);
	CUIBuffer* GetUIBuffer(UINT _vertexCount, UINT _indexCount);

public:
	CBufferMgr();
	~CBufferMgr();
};

