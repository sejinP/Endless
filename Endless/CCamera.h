#pragma once

class CObject;

class CCamera
{
private:
	XMFLOAT3 m_position;	// 카메라의 현재 위치
	XMFLOAT3 m_rotation;	// 회전 정보. 오너의 회전 정보를 따라감
	XMFLOAT3 m_up;			// 상방 벡터
	XMFLOAT3 m_offset;

	CObject* m_camOwner;	// 따라다닐 오브젝트

public:
	void Update();

public:
	XMMATRIX GetViewMatrix();
	XMMATRIX GetUIWorldMatrix();

	void SetCamPos(XMFLOAT3 _camPos) { m_position = _camPos; }
	XMFLOAT3 GetCamPos() { return m_position; }

	void SetCamRot(XMFLOAT3 _camRot) { m_rotation = _camRot; }
	XMFLOAT3 GetCamRot() { return m_rotation; }

	void SetOwner(CObject* _owner) { m_camOwner = _owner; }
	CObject* GetOwner() { return m_camOwner; }

	XMMATRIX GetRotationMat();

private:
	XMVECTOR GetCamLookAt();

public:
	CCamera();
	~CCamera();
};

