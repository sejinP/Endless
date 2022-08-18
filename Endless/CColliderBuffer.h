#pragma once

class CColliderBuffer
{
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

public:
	void Init();
	void BufferSetting(ID3D11DeviceContext* _deviceContext);

public:
	CColliderBuffer();
	~CColliderBuffer();
};