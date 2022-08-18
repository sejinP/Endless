#pragma once
#include "CObject.h"

class CObjectBuffer;

class CBullet :
    public CObject
{
private:
    CObjectBuffer*  m_buffer;
    XMFLOAT3        m_forwardDir;

    float           m_accumulateTime;
    float           m_lifeTime;

public:
    void Update() override;
    void DepthRender(ID3D11DeviceContext* _deviceContext) override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    void SetForwardDir(XMFLOAT3 _forwardDir) { m_forwardDir = _forwardDir; }
    void SetLifeTime(float _lifeTime) { m_lifeTime = _lifeTime; }

public:
    CBullet();
    CBullet(const CBullet& _other);
    ~CBullet();
};

