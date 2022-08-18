#pragma once

class CDepthShader
{
private:
    ID3D11VertexShader*     m_vertexShader;
    ID3D11PixelShader*      m_pixelShader;
    ID3D11InputLayout*      m_inputLayout;
    ID3D11Buffer*           m_matrixBuffer;

private:
    void SetShaderParameters(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix);

    void RenderShader(ID3D11DeviceContext* _deviceContext, UINT _indexCount);

    void OutputShaderErrorMessage(ID3D10Blob* _errorMessage, HWND _hwnd, WCHAR* _shaderFileName);

public:
    void Init(ID3D11Device* _device, HWND _hwnd);

    void Render(ID3D11DeviceContext* _deviceContext, MatrixBufferType _transformMatrix, UINT _indexCount);

public:
    CDepthShader();
    ~CDepthShader();
};

