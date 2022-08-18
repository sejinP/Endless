#pragma once
#include "CComponent.h"

class CColliderBuffer;

class CCollider :
    public CComponent
{
private:
    static UINT m_uniqueID;

    XMFLOAT3 m_position;
    XMFLOAT3 m_scale;
    XMFLOAT3 m_rotation;
    XMFLOAT3 m_offset;

    CColliderBuffer* m_buffer;

    UINT    m_colliderID; // 콜라이더의 유일한 id

private:
    XMMATRIX GetWorldMatrix();

public:
    void Update() override;
    void Render(ID3D11DeviceContext* _deviceContext);

    void OnCollisionEnter(CCollider* _otherCol);
    void OnCollision(CCollider* _otherCol);
    void OnCollisionExit(CCollider* _otherCol);

public:
    XMFLOAT3 GetPosition() { return XMFLOAT3(m_position.x + m_offset.x,     // offset만큼 떨어진 위치가 실제 위치
        m_position.y + m_offset.y, m_position.z + m_offset.z); }

    XMFLOAT3 GetScale() { return m_scale; }
    void SetScale(XMFLOAT3 _scale) { m_scale = _scale; }

    XMFLOAT3 GetRotation() { return m_rotation; }
    void SetRotation(XMFLOAT3 _rotation) { m_rotation = _rotation; }

    XMFLOAT3 GetOffset() { return m_offset; }
    void SetOffset(XMFLOAT3 _offset) { m_offset = _offset; }

    XMMATRIX GetScaleMatrix();
    XMMATRIX GetRotationMatrix();
    XMMATRIX GetTranslateMatrix();

    UINT GetColliderID() { return m_colliderID; }

public:
    CCollider();
    CCollider(const CCollider& _other);
    ~CCollider();
};

