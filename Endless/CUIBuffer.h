#pragma once

class CUI;

class CUIBuffer
{
private:
    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;

    float m_prevPosX;
    float m_prevPosY;
    int m_screenWidth;
    int m_screenHeight;

public:
    void Init(UINT vertexCount, UINT indexCount);
    void BufferSetting(ID3D11DeviceContext* _deviceContext);

    void UpdateBuffer(CUI* _ui);

public:
    CUIBuffer();
    ~CUIBuffer();
};

