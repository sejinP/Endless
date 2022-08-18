#pragma once
#include "CObject.h"

class CObjectBuffer;

class CStructure :
    public CObject
{
private:
    CObjectBuffer* m_buffer;

public:
    void Update() override;
    void DepthRender(ID3D11DeviceContext* _deviceContext) override;
    void Render(ID3D11DeviceContext* _deviceContext) override;

    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

    void RePosition(CCollider* _otherCol);

public:
    CObjectBuffer* GetBuffer() { return m_buffer; }
    void SetBuffer(CObjectBuffer* _buffer) { m_buffer = _buffer; }

public:
    CStructure();
    CStructure(const CStructure& _other);
    ~CStructure();
};

