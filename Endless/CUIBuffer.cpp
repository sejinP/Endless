#include "global.h"
#include "CUIBuffer.h"

#include "CCentral.h"

#include "CUI.h"

CUIBuffer::CUIBuffer()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_prevPosX(-1.0f)
	, m_prevPosY(-1.0f)
	, m_screenWidth(0)
	, m_screenHeight(0)
{
	m_screenWidth = CCentral::Instance().GetScreenWidth();
	m_screenHeight = CCentral::Instance().GetScreenHeight();
}

CUIBuffer::~CUIBuffer()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}

void CUIBuffer::Init(UINT _vertexCount, UINT _indexCount)
{
	ID3D11Device* device = CCentral::Instance().GetDevice();

	// 정점 배열을 만든다.
	UIVertexType* vertices = new UIVertexType[_vertexCount];

	// 인덱스 배열을 만든다.
	unsigned long* indices = new unsigned long[_indexCount];

	memset(vertices, 0, (sizeof(UIVertexType) * _vertexCount));

	// 인덱스 배열
	for (UINT i = 0; i < _indexCount; ++i)
	{
		indices[i] = i;
	}

	// 동적 정점 버퍼 구조체
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(UIVertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터 설정.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼 생성
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// 정적 인덱스 버퍼의 구조체 설정.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
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

void CUIBuffer::BufferSetting(ID3D11DeviceContext* _deviceContext)
{
	// 정점 버퍼의 단위와 오프셋 설정.
	UINT stride = sizeof(UIVertexType);
	UINT offset = 0;

	// 버텍스 버퍼, 인덱스 버퍼 설정.
	_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 삼각형 설정.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CUIBuffer::UpdateBuffer(CUI* _ui)
{
	XMFLOAT2 uiPos = _ui->GetUIPos();
	XMFLOAT2 uiRealPos = _ui->GetUIRealPos();
	int uiWidth = _ui->GetUIWidth();
	int uiHeight = _ui->GetUIHeight();
	int uiVertexCount = _ui->GetVertexCount();
	CUI* parentUI = _ui->GetParentUI();

	if (nullptr == parentUI)
	{
		// 부모 UI가 없다면 상대적 포지션이 리얼 포지션
		uiRealPos = XMFLOAT2(uiPos.x, uiPos.y);
	}
	else
	{
		// 부모 UI가 있다면 부모 UI의 리얼 포지션 기준으로 포지션이 정해짐.
		uiRealPos = parentUI->GetUIRealPos();
		uiRealPos = XMFLOAT2(uiRealPos.x + uiPos.x, uiRealPos.y + uiPos.y);
	}

	_ui->SetUIRealPos(uiRealPos);

	float positionX = uiRealPos.x;
	float positionY = uiRealPos.y;

	float left, right, top, bottom;
	UIVertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	UIVertexType* verticesPtr;
	ID3D11DeviceContext* deviceContext = CCentral::Instance().GetDeviceContext();


	// 이전과 같은 포지션이라면 업데이트 하지 않음
	if ((positionX == m_prevPosX) && (positionY == m_prevPosY))
		return;
	
	// 포지션 갱신
	m_prevPosX = positionX;
	m_prevPosY = positionY;

	// 왼쪽 화면 좌표
	left = (float)((m_screenWidth / 2) * -1) + positionX;

	// 오른쪽 화면 좌표
	right = left + (float)uiWidth;

	// 상단 화면 좌표
	top = (float)(m_screenHeight / 2) - positionY;

	// 하단 화면 좌표
	bottom = top - (float)uiHeight;

	// 정점 배열
	vertices = new UIVertexType[uiVertexCount];

	// 첫 번째 삼각형
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// 두 번째 삼각형
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// 버텍스 버퍼를 쓸 수 있도록 잠금.
	deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 정점 버퍼의 데이터를 가리키는 포인터
	verticesPtr = (UIVertexType*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사
	memcpy(verticesPtr, (void*)vertices, (sizeof(UIVertexType) * uiVertexCount));

	// 정점 버퍼의 잠금 해제
	deviceContext->Unmap(m_vertexBuffer, 0);

	// 버텍스 배열 해제
	delete[]vertices;
}
