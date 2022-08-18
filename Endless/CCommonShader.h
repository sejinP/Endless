#pragma once

class CLight;

class CCommonShader
{
private:
    struct LightBufferType
    {
        XMFLOAT4 ambientColor;
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float specularPower;
    };

    struct CameraBufferType
    {
        XMFLOAT3 cameraPosition;
        float padding;
    };

    ID3D11VertexShader*     m_vertexShader;
    ID3D11PixelShader*      m_pixelShader;
    ID3D11InputLayout*      m_inputLayout;
    ID3D11SamplerState*     m_samplerState;
    ID3D11Buffer*           m_matrixBuffer;
    ID3D11Buffer*           m_cameraBuffer;
    ID3D11Buffer*           m_lightBuffer;

private:
    void SetShaderParameters(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
        CLight* _light, ID3D11ShaderResourceView* _texture, XMFLOAT3 _cameraPosition);

    void RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount);

    void OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName);

public:
    void Init(ID3D11Device* _device, HWND _hwnd);

    void Render(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix,
        CLight* _light, ID3D11ShaderResourceView* _texture, XMFLOAT3 _cameraPosition, UINT _indexCount);

public:
    CCommonShader();
    ~CCommonShader();
};

