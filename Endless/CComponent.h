#pragma once

class CObject;

class CComponent
{
private:
	CObject* m_owner;

public:
	virtual void Update() = 0;
	
public:
	CObject* GetOwner() { return m_owner; }
	void SetOwner(CObject* _owner) { m_owner = _owner; }

public:
	CComponent();
	virtual ~CComponent();
};

