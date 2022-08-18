#pragma once
#include "CStructure.h"

class CWall :
    public CStructure
{
private:

public:
    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    CWall();
    CWall(const CWall& _other);
    ~CWall();
};

