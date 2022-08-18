#pragma once

class CCollider;
class CObject;

class CCollisionMgr
{
public:
	static CCollisionMgr& Instance() { static CCollisionMgr instance; return instance; }

private:
	// 두 아이디의 조합으로 유일한 id 만듦.
	union IDCombination
	{
		struct
		{
			UINT leftID;
			UINT rightID;
		};
		ULONGLONG uniqueID;
	};

	bool m_colCheck[(size_t)OBJECT_TYPE::END][(size_t)OBJECT_TYPE::END];	// 충돌 체크 실시 여부 나타낸 2차원 배열.

	map<ULONGLONG, COLLISION_STATE> m_collisionState;						// 두 개의 ID 조합의 고유한 값과 이전 충돌 상태정보 저장

private:
	void CollisionCheck();													// 충돌 체크

	void ObjectColCheck(const vector<CObject*>& _objLeft, const vector<CObject*>& _objRight);

	bool ColliderCheck(CCollider* _colLeft, CCollider* _colRight);			// 충돌체 간의 충돌 체크

public:
	void Init();
	void Update();

public:
	void SetColCheck(OBJECT_TYPE _objType1, OBJECT_TYPE _objType2);

public:
	CCollisionMgr();
	~CCollisionMgr();
};

