#pragma once

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct ModelType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct MatrixBufferType
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX lightViewMatrix;
	XMMATRIX lightProjectionMatrix;
};

// ---------------------------------
// UI에서 사용
// ---------------------------------
struct UIVertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};
// ---------------------------------
// ---------------------------------

struct ColVertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct TransformBufferType
{
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
};