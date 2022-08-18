// -----------------------------------
// 상수 버퍼 타입
// -----------------------------------
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer LightBuffer2
{
    float3 lightPosition;
    float padding;
};
// -----------------------------------
// -----------------------------------

// -----------------------------------
// 입력 타입
// -----------------------------------
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
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};
// -----------------------------------
// -----------------------------------

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

    // 행렬 계산을 위해 1넣어줌
    input.position.w = 1.0f;

    // 월드, 뷰 및 투영 행렬에 대한 정점의 위치 계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // 빛을 기준으로 보았을 때의 정점 위치 계산
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    // 텍스쳐 좌표 저장
    output.tex = input.tex;

    // 월드 행렬에 대해서만 법선 벡터 계산.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

    // 법선 벡터를 정규화
    output.normal = normalize(output.normal);

    // 세계 기준의 정점 위치
    worldPosition = mul(input.position, worldMatrix);

    // 빛의 위치와 세계의 정점 위치를 기반으로 빛을 보는 방향 설정
    output.lightPos = lightPosition.xyz - worldPosition.xyz;

    // 빛 보는 방향 정규화
    output.lightPos = normalize(output.lightPos);

    return output;
}