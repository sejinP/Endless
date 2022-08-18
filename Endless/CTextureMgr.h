#pragma once

class CTexture;

class CTextureMgr
{
public:
	static CTextureMgr& Instance() { static CTextureMgr instance; return instance; }

private:
	map<const wstring, CTexture*> m_tex;

public:
	CTexture* GetTexture(const wstring& _path);

public:
	CTextureMgr();
	~CTextureMgr();
};
