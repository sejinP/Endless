#pragma once

class CMonsterBuffer
{
private:
    ID3D11Buffer* m_vertexBuffer;
    ID3D11Buffer* m_indexBuffer;

public:
    void Init(const vector<ModelType>& _modelVtx, const vector<UINT>& _modelIdx);
    void BufferSetting(ID3D11DeviceContext* _deviceContext);

public:
    CMonsterBuffer();
    ~CMonsterBuffer();
};

