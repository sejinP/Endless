#include "global.h"
#include "CColliderBuffer.h"

#include "CCentral.h"

CColliderBuffer::CColliderBuffer()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{

}

CColliderBuffer::~CColliderBuffer()
{
    if (m_vertexBuffer)
        m_vertexBuffer->Release();
    if (m_indexBuffer)
        m_indexBuffer->Release();
}

void CColliderBuffer::Init()
{
    ID3D11Device* device = CCentral::Instance().GetDevice();
    HRESULT hr;

    ColVertexType vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ColVertexType) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;
    hr = device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
    if (FAILED(hr))
        assert(nullptr);

    unsigned long indices[] =
    {
        0, 1, 2, 3, 0, 4, 5, 1, 5, 6, 2, 6, 7, 3, 7, 4,
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(unsigned long) * 16;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = device->CreateBuffer(&bd, &InitData, &m_indexBuffer);
    if (FAILED(hr))
        assert(nullptr);
}

void CColliderBuffer::BufferSetting(ID3D11DeviceContext* _deviceContext)
{
    // 정점 버퍼의 단위와 오프셋 설정.
    UINT stride = sizeof(ColVertexType);
    UINT offset = 0;

    // 버텍스 버퍼, 인덱스 버퍼 설정.
    _deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    _deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // linestrip 설정.
    _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}
