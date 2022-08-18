#pragma once

class CTexture;
class CComponent;
class CCollider;
class CRigidBody;
class CPlayerController;

class CObject : public AlignedAllocationPolicy<16>
{
private:
	wstring				m_objName;

	XMFLOAT3			m_upVec;

	CTexture*			m_texture;

	XMFLOAT3			m_position;
	XMFLOAT3			m_rotation;
	XMFLOAT3			m_scale;
	XMFLOAT3			m_velocity;

	float				m_maxSpeed;
	float				m_speed;
	float				m_mass;
	float				m_hp;
	float				m_offensivePower;

	vector<ModelType>	m_modelVtx;
	vector<UINT>		m_modelIdx;

	CComponent*			m_components[(size_t)COMPONENT_TYPE::END];

	bool				m_isGround;
	bool				m_isDead;

public:
	virtual void Update() = 0;
	virtual void DepthRender(ID3D11DeviceContext* _deviceContext);
	virtual void Render(ID3D11DeviceContext* _deviceContext);
	virtual void LoadModel(const wchar_t* _fileName);

	virtual void OnCollisionEnter(CCollider* _otherCol);
	virtual void OnCollision(CCollider* _otherCol);
	virtual void OnCollisionExit(CCollider* _otherCol);

public:
	virtual XMMATRIX GetWorldMatrix();			// 플레이어에서는 재정의

	// getter, setter
	const wstring& GetObjectName() { return m_objName; }
	void SetObjectName(const wstring& _objName) { m_objName = _objName; }

	void SetUpVector(XMFLOAT3 _upVec) { m_upVec = _upVec; }
	XMFLOAT3 GetUpVector() { return m_upVec; }

	void SetPosition(XMFLOAT3 _position) { m_position = _position; }
	XMFLOAT3 GetPosition() { return m_position; }
	void SetRotation(XMFLOAT3 _rotation) { m_rotation = _rotation; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	void SetScale(XMFLOAT3 _scale) { m_scale = _scale; }
	XMFLOAT3 GetScale() { return m_scale; }
	void SetVelocity(XMFLOAT3 _velocity) { m_velocity = _velocity; }
	XMFLOAT3 GetVelocity() { return m_velocity; }
	void SetMaxSpeed(float _maxSpeed) { m_maxSpeed = _maxSpeed; }
	float GetMaxSpeed() { return m_maxSpeed; }

	CTexture* GetTexture() { return m_texture; }
	void SetTexture(CTexture* _texture) { m_texture = _texture; }

	void SetSpeed(float _speed) { m_speed = _speed; }
	float GetSpeed() { return m_speed; }
	void SetMass(float _mass) { m_mass = _mass; }
	float GetMass() { return m_mass; }
	void SetHP(float _hp) { m_hp = _hp; }
	float GetHP() { return m_hp; }
	void SetOffensivePower(float _offensivePower) { m_offensivePower = _offensivePower; }
	float GetOffensivePower() { return m_offensivePower; }

	const vector<ModelType>& GetModelVtx() { return m_modelVtx; }
	const vector<UINT>& GetModelIdx() { return m_modelIdx; }
	void SetModelVtx(vector<ModelType> _modelVtx) { m_modelVtx = _modelVtx; }
	void SetModelIdx(vector<UINT> _modelIdx) { m_modelIdx = _modelIdx; }

	UINT GetIndexCount() { return (UINT)m_modelIdx.size(); }
	UINT GetVertexCount() { return (UINT)m_modelVtx.size(); }

	XMMATRIX GetScaleMatrix();
	XMMATRIX GetRotationMatrix();
	XMMATRIX GetTranslateMatrix();

	XMVECTOR GetForwardVec();
	XMVECTOR GetRightVec();

	void SetComponent(CComponent* _component, COMPONENT_TYPE _componentType)
		{ m_components[(size_t)_componentType] = _component; }
	CComponent* GetComponent(COMPONENT_TYPE _componentType)
		{ return m_components[(size_t)_componentType]; }

	CCollider* GetCollider() { return (CCollider*)m_components[(size_t)COMPONENT_TYPE::COLLIDER]; }
	CRigidBody* GetRigidBody() { return (CRigidBody*)m_components[(size_t)COMPONENT_TYPE::RIGIDBODY]; }
	CPlayerController* GetController() { return (CPlayerController*)m_components[(size_t)COMPONENT_TYPE::CONTROLLER]; }

	void SetIsGround(bool _isGround) { m_isGround = _isGround; }
	bool GetIsGround() { return m_isGround; }

	void SetIsDead(bool _isDead) { m_isDead = _isDead; }
	bool GetIsDead() { return m_isDead; }

public:
	CObject();
	CObject(const CObject& _other);
	virtual ~CObject();
};

