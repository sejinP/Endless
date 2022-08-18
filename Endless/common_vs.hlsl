// --------------------------------------
// 상수 버퍼 타입
// --------------------------------------
cbuffer MatrixBuffer : register( b0 )
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding;
};
// --------------------------------------
// --------------------------------------

// --------------------------------------
// 입력 타입
// --------------------------------------
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};
// --------------------------------------
// --------------------------------------

PixelInputType main(VertexInputType input)
{
    PixelInputType output = (PixelInputType)0;
    

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경
    input.position.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// 픽셀 쉐이더의 텍스처 좌표를 저장
	output.tex = input.tex;
    
	// 월드 행렬에 대해서만 법선 벡터를 계산
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // 법선 벡터를 정규화
    output.normal = normalize(output.normal);

	// 월드의 정점 위치를 계산
    float4 worldPosition = mul(input.position, worldMatrix);

    // 카메라의 위치와 세계의 정점 위치를 기준으로 보기 방향 결정
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // 뷰 방향 벡터를 정규화
    output.viewDirection = normalize(output.viewDirection);

    return output;
}