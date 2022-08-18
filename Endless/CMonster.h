#pragma once
#include "CObject.h"

class CObjectBuffer;
class CState;

class CMonster :
    public CObject
{
private:
    CObjectBuffer*  m_buffer;
    CState*         m_currentState;
    CState*         m_states[(size_t)MONSTER_STATE::END];

    float           m_sensingDist;
    float           m_attackDist;
    float           m_rotationSpeed;

public:
    void Update() override;
    void DepthRender(ID3D11DeviceContext* _deviceContext) override;
    void Render(ID3D11DeviceContext* _deviceContext) override;
    void LoadModel(const wchar_t* _fileName) override;

    void OnCollisionEnter(CCollider* _otherCol) override;
    void OnCollision(CCollider* _otherCol) override;
    void OnCollisionExit(CCollider* _otherCol) override;

public:
    CObjectBuffer* GetBuffer() { return m_buffer; }
    void SetBuffer(CObjectBuffer* _buffer) { m_buffer = _buffer; }

    CState** GetStates() { return m_states; }

    void AddState(CState* _state, MONSTER_STATE _stateType);
    void ChangeState(MONSTER_STATE _stateType) { m_currentState = m_states[(size_t)_stateType]; }

    void SetSensingDist(float _sensingDist) { m_sensingDist = _sensingDist; }
    float GetSensingDist() { return m_sensingDist; }

    void SetAttackDist(float _attackDist) { m_attackDist = _attackDist; }
    float GetAttackDist() { return m_attackDist; }

    void SetRotationSpeed(float _rotationSpeed) { m_rotationSpeed = _rotationSpeed; }
    float GetRotationSpeed() { return m_rotationSpeed; }

public:
    CMonster();
    CMonster(const CMonster& _other);
    ~CMonster();
};

