#include "global.h"
#include "CTexture.h"
#include "DDSTextureLoader11.h"

#include "CCentral.h"

CTexture::CTexture()
	: m_resView(nullptr)
{
}

CTexture::~CTexture()
{
	if (m_resView)
		m_resView->Release();
}

void CTexture::Init(ID3D11Device* _device, const WCHAR* _fileName)
{
	wstring absolutePath = CCentral::Instance().GetAbsolutePath();
	absolutePath += L"texture\\";
	absolutePath += _fileName;
	const wchar_t* fileName = absolutePath.c_str();

	HRESULT result = CreateDDSTextureFromFile(_device, fileName, nullptr, &m_resView);
	if (FAILED(result))
		assert(nullptr);
}