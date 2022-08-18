#pragma once
#include "CObject.h"

class CObjectBuffer;

class CPlayer :
    public CObject
{
private:
    CObjectBuffer*  m_buffer;

private:
    void ShootingUpdate();

public:
    void Update() override;
    void DepthRender(ID3D11DeviceContext* _deviceContext) override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    XMMATRIX GetWorldMatrix() override;

public:
    CPlayer();
    ~CPlayer();
};

