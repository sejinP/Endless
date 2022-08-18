// ------------------------------------
// 상수 버퍼 타입
// ------------------------------------
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
// ------------------------------------
// ------------------------------------

// ------------------------------------
// 입력 타입
// ------------------------------------
struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};
// ------------------------------------
// ------------------------------------

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    // 적절한 행렬 계산을 위해 w를 1로 설정
    input.position.w = 1.0f;

    // 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // 픽셀 쉐이더가 사용할 입력 색상을 저장
    output.depthPosition = output.position;

    return output;
}