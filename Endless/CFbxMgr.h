#pragma once

class fbxsdk::FbxMesh;

class CFbxMgr
{
public:
	static CFbxMgr& Instance() { static CFbxMgr instance; return instance; }

public:
	void LoadFbxModel(const wchar_t* _fileName, vector<ModelType>& _modelVtx, vector<UINT>& _vertex);
	void ReadVertex(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex);
	void ReadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex);
	void ReadUV(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex);

public:
	CFbxMgr();
	~CFbxMgr();
};

