#include "global.h"
#include "CTextureMgr.h"

#include "CTexture.h"

#include "CCentral.h"

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	map<const wstring, CTexture*>::iterator iter = m_tex.begin();
	for (; iter != m_tex.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
}

CTexture* CTextureMgr::GetTexture(const wstring& _path)
{
	// 해당 경로의 텍스쳐를 찾고 없다면 생성해서 반환, 없다면 있는걸 반환.
	ID3D11Device* device = CCentral::Instance().GetDevice();
	map<const wstring, CTexture*>::iterator iter = m_tex.find(_path);
	CTexture* texture;
	if (iter == m_tex.end())
	{
		texture = new CTexture;
		texture->Init(device, _path.c_str());
		m_tex.insert(make_pair(_path, texture));
	}
	else
	{
		texture = iter->second;
	}

	return texture;
}