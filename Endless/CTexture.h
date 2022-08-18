#pragma once

class CTexture
{
private:
	ID3D11ShaderResourceView* m_resView;

public:
	void Init(ID3D11Device* _device, const WCHAR* _fileName);

public:
	ID3D11ShaderResourceView* GetTextureView() { return m_resView; }

public:
	CTexture();
	~CTexture();
};

