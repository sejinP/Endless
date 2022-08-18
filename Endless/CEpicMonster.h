#pragma once
#include "CMonster.h"

class CEpicMonster :
    public CMonster
{
public:
    void Update() override;
    void DepthRender(ID3D11DeviceContext* _deviceContext) override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    CEpicMonster();
    CEpicMonster(const CEpicMonster& _other);
    ~CEpicMonster();
};

