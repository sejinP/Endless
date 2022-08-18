// ------------------------------------
// 입력 타입
// ------------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};
// ------------------------------------
// ------------------------------------

float4 main(PixelInputType input) : SV_TARGET
{
	float depthValue;
	float4 color;

	// 깊이값 (z위치) 가져오기
	depthValue = input.depthPosition.z / input.depthPosition.w;

	// 깊이값으로 텍스쳐 컬러 저장
	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}