#pragma once

class CObject;
class CMonster;
class CScene;

class CEventMgr
{
public:
	static CEventMgr& Instance() { static CEventMgr instance; return instance; }

private:
	vector<pair<CObject*, OBJECT_TYPE>>		m_pushObjects;		// 현재 씬에 푸시할 오브젝트들 임시 저장
	vector<pair<CObject*, OBJECT_TYPE>>		m_pullObjects;		// 현재 씬에서 빼낼 오브젝트를 임시 저장
	vector<pair<CObject*, OBJECT_TYPE>>		m_deleteObjects;	// 현재 씬에서 삭제할 오브젝트를 임시 저장
	SCENE_TYPE								m_changeScene;		// 바꿀 씬의 정보 저장
	vector<pair<CMonster*, MONSTER_STATE>>	m_changeState;		// 바꿀 몬스터 상태의 정보 저장

public:
	void Update();

	void PushObject(CObject* _object, OBJECT_TYPE _objType) { m_pushObjects.push_back(make_pair(_object, _objType)); }
	void PullObject(CObject* _object, OBJECT_TYPE _objType) { m_pullObjects.push_back(make_pair(_object, _objType)); }
	void DeleteObject(CObject* _object, OBJECT_TYPE _objType) { m_deleteObjects.push_back(make_pair(_object, _objType)); }
	void ChangeScene(SCENE_TYPE _sceneType) { m_changeScene = _sceneType; }
	void ChangeState(CMonster* _monster, MONSTER_STATE _monsterState) { m_changeState.push_back(make_pair(_monster, _monsterState)); }

public:
	CEventMgr();
	~CEventMgr();
};

