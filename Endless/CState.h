#pragma once

class CMonster;

class CState
{
private:
	CMonster* m_owner;

public:
	virtual void Update() = 0;

	void RotationToTarget(XMVECTOR _targetVec);

public:
	CMonster* GetOwner() { return m_owner; }
	void SetOwner(CMonster* _owner) { m_owner = _owner; }

public:
	CState();
	virtual ~CState();
};
