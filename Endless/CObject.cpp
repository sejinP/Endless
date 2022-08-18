#include "global.h"
#include "CObject.h"

#include "CCentral.h"

#include "CTexture.h"

#include "CComponent.h"
#include "CCollider.h"
#include "CPlayerController.h"
#include "CRigidBody.h"

CObject::CObject()
	: m_objName()
	, m_upVec(XMFLOAT3(0.0f, 1.0f, 0.0f))
	, m_texture(nullptr)
	, m_position(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_scale(XMFLOAT3(1.0f, 1.0f, 1.0f))
	, m_velocity(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, m_maxSpeed(0.0f)
	, m_speed(0.0f)
	, m_mass(1.0f)
	, m_hp(0.0f)
	, m_offensivePower(0.0f)
	, m_modelVtx()
	, m_modelIdx()
	, m_isGround(false)
	, m_isDead(false)
{
	size_t size = (size_t)COMPONENT_TYPE::END;
	for (size_t i = 0; i < size; ++i)
	{
		m_components[i] = nullptr;
	}
}

CObject::CObject(const CObject& _other)
	: m_objName(_other.m_objName)
	, m_upVec(_other.m_upVec)
	, m_texture(_other.m_texture)
	, m_position(_other.m_position)
	, m_rotation(_other.m_rotation)
	, m_scale(_other.m_scale)
	, m_velocity(_other.m_velocity)
	, m_maxSpeed(_other.m_maxSpeed)
	, m_speed(_other.m_speed)
	, m_mass(_other.m_mass)
	, m_hp(_other.m_hp)
	, m_offensivePower(_other.m_offensivePower)
	, m_modelVtx(_other.m_modelVtx)
	, m_modelIdx(_other.m_modelIdx)
	, m_isGround(_other.m_isGround)
	, m_isDead(_other.m_isDead)
{
	size_t size = (size_t)COMPONENT_TYPE::END;
	for (size_t i = 0; i < size; ++i)
	{
		m_components[i] = nullptr;
	}

	if (nullptr != _other.m_components[(size_t)COMPONENT_TYPE::COLLIDER])
	{
		CCollider* otherCol = (CCollider*)_other.m_components[(size_t)COMPONENT_TYPE::COLLIDER];
		CCollider* collider = new CCollider(*otherCol);
		collider->SetOwner(this);
		m_components[(size_t)COMPONENT_TYPE::COLLIDER] = collider;
	}

	if (nullptr != _other.m_components[(size_t)COMPONENT_TYPE::CONTROLLER])
	{
		CPlayerController* otherController = (CPlayerController*)_other.m_components[(size_t)COMPONENT_TYPE::CONTROLLER];
		CPlayerController* controller = new CPlayerController(*otherController);
		controller->SetOwner(this);
		m_components[(size_t)COMPONENT_TYPE::CONTROLLER] = controller;
	}

	if (nullptr != _other.m_components[(size_t)COMPONENT_TYPE::RIGIDBODY])
	{
		CRigidBody* otherRigidBody = (CRigidBody*)_other.m_components[(size_t)COMPONENT_TYPE::RIGIDBODY];
		CRigidBody* rigidBody = new CRigidBody(*otherRigidBody);
		rigidBody->SetOwner(this);
		m_components[(size_t)COMPONENT_TYPE::RIGIDBODY] = rigidBody;
	}
}

CObject::~CObject()
{
	size_t size = (size_t)COMPONENT_TYPE::END;
	for (size_t i = 0; i < size; ++i)
	{
		if(nullptr != m_components[i])
			delete m_components[i];
	}
}

void CObject::Update()
{
	size_t size = (size_t)COMPONENT_TYPE::END;
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_components[i])
			m_components[i]->Update();
	}
}

void CObject::DepthRender(ID3D11DeviceContext* _deviceContext)
{
}

void CObject::Render(ID3D11DeviceContext* _deviceContext)
{
	
}

XMMATRIX CObject::GetScaleMatrix()
{
	// 스케일 행렬
	XMMATRIX scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	return scaleMatrix;
}

XMMATRIX CObject::GetRotationMatrix()
{
	// 회전 행렬 (라디안으로 변환하여 계산)
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_rotation.x),
		XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);
	return rotationMatrix;
}

XMMATRIX CObject::GetTranslateMatrix()
{
	// 이동 행렬
	XMMATRIX positionMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	return positionMatrix;
}

XMVECTOR CObject::GetForwardVec()
{
	// 회전 행렬의 세번째 벡터(z축 벡터, 보고 있는 방향)
	XMMATRIX rotationMatrix = GetRotationMatrix();
	XMVECTOR forwardVec = rotationMatrix.r[2];

	return XMVector3Normalize(forwardVec);
}

XMVECTOR CObject::GetRightVec()
{
	// 카메라의 오른쪽 방향 구하기(업 벡터와 포워드 벡터 외적)
	XMVECTOR forwardVec = GetForwardVec();
	XMVECTOR upVec = XMLoadFloat3(&m_upVec);
	XMVECTOR rightVec = XMVector3Cross(upVec, forwardVec);

	return XMVector3Normalize(rightVec);
}

void CObject::LoadModel(const wchar_t* _fileName)
{
	wstring absolutePath = CCentral::Instance().GetAbsolutePath();
	absolutePath += L"fbx\\";
	absolutePath += _fileName;
	const wchar_t* fileName = absolutePath.c_str();

	// 파일 열기
	ifstream fin;
	fin.open(fileName);

	// 파일을 열 수 없으면 종료.
	if (fin.fail())
	{
		assert(nullptr);
	}

	// 버텍스 카운트의 값까지 읽기.
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	int vertexCount;

	// 버텍스 카운트.
	fin >> vertexCount;

	// 데이터의 시작 부분까지 읽음.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// 버텍스 데이터를 읽기.
	for (int i = 0; i < vertexCount; i++)
	{
		ModelType model;
		fin >> model.position.x >> model.position.y >> model.position.z;
		fin >> model.texture.x >> model.texture.y;
		fin >> model.normal.x >> model.normal.y >> model.normal.z;

		m_modelVtx.push_back(model);
		m_modelIdx.push_back(UINT(i));
	}

	// 모델 파일을 닫기.
	fin.close();
}

XMMATRIX CObject::GetWorldMatrix()
{
	// SRT 순서
	return (GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix());
}

void CObject::OnCollisionEnter(CCollider* _otherCol)
{
}

void CObject::OnCollision(CCollider* _otherCol)
{
}

void CObject::OnCollisionExit(CCollider* _otherCol)
{
}