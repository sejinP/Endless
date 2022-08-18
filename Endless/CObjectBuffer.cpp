#include "global.h"
#include "CObjectBuffer.h"

#include "CCentral.h"

CObjectBuffer::CObjectBuffer()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

CObjectBuffer::~CObjectBuffer()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}

void CObjectBuffer::Init(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx)
{
	ID3D11Device* device = CCentral::Instance().GetDevice();

	UINT vertexCount = (UINT)_modelVtx.size();
	UINT indexCount = (UINT)_modelIdx.size();

	// 정점 배열을 만든다.
	VertexType* vertices = new VertexType[vertexCount];

	// 인덱스 배열을 만든다.
	unsigned long* indices = new unsigned long[indexCount];

	// 정점 배열과 인덱스 배열을 데이터로 읽어온다.
	for (UINT i = 0; i < vertexCount; ++i)
	{
		vertices[i].position = XMFLOAT3(_modelVtx[i].position.x, _modelVtx[i].position.y, _modelVtx[i].position.z);
		vertices[i].texture = XMFLOAT2(_modelVtx[i].texture.x, _modelVtx[i].texture.y);
		vertices[i].normal = XMFLOAT3(_modelVtx[i].normal.x, _modelVtx[i].normal.y, _modelVtx[i].normal.z);
	}

	for (UINT i = 0; i < indexCount; ++i)
	{
		indices[i] = _modelIdx[i];
	}

	// 정적 정점 버퍼의 구조체 설정.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터 설정.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	// 정점 버퍼 생성.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// 정적 인덱스 버퍼의 구조체 설정.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 리소스 구조체에 인덱스 데이터 설정.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	// 생성되고 값이 할당된 정점 배열 인덱스 배열 해제
	delete[] vertices;

	delete[] indices;
}

void CObjectBuffer::BufferSetting(ID3D11DeviceContext* _deviceContext)
{
	// 정점 버퍼의 단위와 오프셋 설정.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 버텍스 버퍼, 인덱스 버퍼 설정.
	_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 삼각형 설정.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}