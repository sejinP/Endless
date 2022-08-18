#pragma once
#include "CStructure.h"

class CPlane :
    public CStructure
{
private:

public:
    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    CPlane();
    CPlane(const CPlane& _other);
    ~CPlane();
};

