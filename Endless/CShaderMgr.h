#pragma once

class CLight;

class CCommonShader;
class CDepthShader;
class CShadowShader;
class CUIShader;
class CColliderShader;

class CShaderMgr
{
public:
	static CShaderMgr& Instance() { static CShaderMgr instance; return instance; }

private:
	// 셰이더를 들고있음. 렌더 할 때마다 각 오브젝트에 맞는 셰이더로 렌더하게 됨
	CCommonShader*		m_commonShader;
	CDepthShader*		m_depthShader;
	CShadowShader*		m_shadowShader;
	CUIShader*			m_uiShader;
	CColliderShader*	m_colShader;

public:
	void Init();

	void RenderCommonShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
		CLight* _light, ID3D11ShaderResourceView* _texture, XMFLOAT3 _cameraPosition, UINT _indexCount);

	void RenderDepthShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix, UINT _indexCount);

	void RenderShadowShader(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _depthMapTexture, CLight* _light, UINT _indexCount);

	void RenderUIShader(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix,
		ID3D11ShaderResourceView* _texture, UINT _indexCount);

	void RenderColliderShader(ID3D11DeviceContext* _deviceContext, TransformBufferType _transformMatrix, UINT _indexCount);

public:
	CShaderMgr();
	~CShaderMgr();
};

