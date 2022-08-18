Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);


// --------------------------------------
// 상수 버퍼 타입
// --------------------------------------
cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};
// --------------------------------------
// --------------------------------------

// --------------------------------------
// 픽셀 셰이더 입력 타입
// --------------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};
// --------------------------------------
// --------------------------------------

float4 main(PixelInputType input) : SV_TARGET
{
	float bias;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;


	// 정밀도로 인한 bias값 설정
	bias = 0.00001f;

	// ambient 색상을 기본색상으로
	color = ambientColor;

	// 빛 기준으로 투영했을 때의 해당 정점의 텍스쳐 좌표
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// 0 ~ 1 범위에 있는지
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// 빛 기준으로 투영했을 때의 해당 정점의 텍스쳐 좌표에서 깊이정보 값 얻어옴(해당 픽셀의 rgb값이 깊이 정보)
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// 빛의 깊이 계산
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// 정밀도 계산
		lightDepthValue = lightDepthValue - bias;

		// depthValue(깊이 텍스쳐에서 얻어온 깊이 정보)가 lightDepthValue(해당 정점의 빛으로부터의 거리)보다 크다면
		// 그려줌 (크다는건 빛을 바라봤을 때 자기 앞에 아무것도 없다는 것)
		// 그려지지 않는다면 기본 ambientColor로 되어 있을 것임 -> 그림자라는 것)
		if (lightDepthValue < depthValue)
		{
			// 빛의 양 계산
			lightIntensity = saturate(dot(input.normal, input.lightPos));

			if (lightIntensity > 0.0f)
			{
				// 확산 색상과 빛의 양을 이용해 color 계산
				color += (diffuseColor * lightIntensity);

				// 최종 빛의 색상
				color = saturate(color);
			}
		}
	}

	// 자기의 텍스쳐 좌표 이용해 텍스쳐 샘플링
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// 빛과 텍스쳐 색상 결합
	color = color * textureColor;

	return color;
}