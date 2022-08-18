#include "global.h"
#include "CFbxMgr.h"

#include "CCentral.h"

CFbxMgr::CFbxMgr()
{
}

CFbxMgr::~CFbxMgr()
{
}

void CFbxMgr::LoadFbxModel(const wchar_t* _fileName, vector<ModelType>& _modelVtx, vector<UINT>& _modelIdx)
{
    // FbxManager를 통한 fbx의 정보 불러오기.
    FbxManager* fbxSdkManager = nullptr;

    if (fbxSdkManager == nullptr)
    {
        fbxSdkManager = FbxManager::Create();

        FbxIOSettings* IOsettings = FbxIOSettings::Create(fbxSdkManager, IOSROOT);
        fbxSdkManager->SetIOSettings(IOsettings);
    }

    FbxImporter* importer = FbxImporter::Create(fbxSdkManager, "");
    FbxScene* fbxScene = FbxScene::Create(fbxSdkManager, "");
    
    const char* fileName = CCentral::Instance().ConvertWCtoC(_fileName);
    bool bSuccess = importer->Initialize(fileName, -1, fbxSdkManager->GetIOSettings());
    if (!bSuccess)
        assert(nullptr);
    delete fileName;

    bSuccess = importer->Import(fbxScene);
    if (!bSuccess)
        assert(nullptr);

    importer->Destroy();

    FbxNode* fbxRootNode = fbxScene->GetRootNode();

    FbxGeometryConverter geometryConverter(fbxSdkManager);
    geometryConverter.Triangulate(fbxScene, true);

    if (fbxRootNode)
    {
        for (int i = 0; i < fbxRootNode->GetChildCount(); ++i)
        {
            FbxNode* fbxChildNode = fbxRootNode->GetChild(i);

            if (fbxChildNode->GetNodeAttribute() == NULL)
                continue;

            FbxNodeAttribute::EType attributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();

            if (attributeType != FbxNodeAttribute::eMesh)
                continue;

            FbxMesh* mesh = (FbxMesh*)fbxChildNode->GetNodeAttribute();

            UINT vertexCount = 0;

            // 삼각형 개수만큼
            for (int j = 0; j < mesh->GetPolygonCount(); ++j)
            {
                int numVertices = mesh->GetPolygonSize(j);
                assert(numVertices == 3);

                // 그 삼각형의 각 정점들에 대한 좌표, 법선 벡터, uv좌표 불러오기
                for (int k = 0; k < numVertices; ++k) {
                    int iControlPointIndex = mesh->GetPolygonVertex(j, k);

                    ModelType vertex;

                    ReadVertex(mesh, iControlPointIndex, vertexCount, vertex);
                    ReadNormal(mesh, iControlPointIndex, vertexCount, vertex);
                    ReadUV(mesh, iControlPointIndex, mesh->GetTextureUVIndex(j, k), vertex);

                    _modelVtx.push_back(vertex);
                    _modelIdx.push_back(vertexCount++);
                }
            }
        }
    }
}

void CFbxMgr::ReadVertex(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex)
{
    FbxVector4* vertices = _mesh->GetControlPoints();

    _vertex.position.x = (float)vertices[_controlPointIndex].mData[0];
    _vertex.position.y = (float)vertices[_controlPointIndex].mData[1];
    _vertex.position.z = (float)vertices[_controlPointIndex].mData[2];
}

void CFbxMgr::ReadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex)
{
    FbxGeometryElementNormal* normal = _mesh->GetElementNormal(0);

    // 법선 벡터 정보 불러오기
    // 각 참조 모드 마다 방법이 다름
    switch (normal->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (normal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            _vertex.normal.x = (float)normal->GetDirectArray().GetAt(_controlPointIndex).mData[0];
            _vertex.normal.y = (float)normal->GetDirectArray().GetAt(_controlPointIndex).mData[1];
            _vertex.normal.z = (float)normal->GetDirectArray().GetAt(_controlPointIndex).mData[2];
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = normal->GetIndexArray().GetAt(_controlPointIndex);
            _vertex.normal.x = (float)normal->GetDirectArray().GetAt(index).mData[0];
            _vertex.normal.y = (float)normal->GetDirectArray().GetAt(index).mData[1];
            _vertex.normal.z = (float)normal->GetDirectArray().GetAt(index).mData[2];
        }
        break;
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (normal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            _vertex.normal.x = (float)normal->GetDirectArray().GetAt(_vertexCount).mData[0];
            _vertex.normal.y = (float)normal->GetDirectArray().GetAt(_vertexCount).mData[1];
            _vertex.normal.z = (float)normal->GetDirectArray().GetAt(_vertexCount).mData[2];
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = normal->GetIndexArray().GetAt(_vertexCount);
            _vertex.normal.x = (float)normal->GetDirectArray().GetAt(index).mData[0];
            _vertex.normal.y = (float)normal->GetDirectArray().GetAt(index).mData[1];
            _vertex.normal.z = (float)normal->GetDirectArray().GetAt(index).mData[2];
        }
        break;
        }
    }
    break;
    }
    }
}

void CFbxMgr::ReadUV(FbxMesh* _mesh, int _controlPointIndex, int _vertexCount, ModelType& _vertex)
{
    FbxGeometryElementUV* vertexUV = _mesh->GetElementUV(0);

    // uv좌표 정보 불러오기.
    // 참조 모드마다 방법이 다름
    switch (vertexUV->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (vertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            _vertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(_controlPointIndex).mData[0];
            _vertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(_controlPointIndex).mData[1];
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = vertexUV->GetIndexArray().GetAt(_controlPointIndex);
            _vertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(index).mData[0];
            _vertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(index).mData[1];
        }
        break;
        }
    }
    break;
    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (vertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        case FbxGeometryElement::eIndexToDirect:
        {
            _vertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(_vertexCount)[0];
            _vertex.texture.y = 1.0f - (float)vertexUV->GetDirectArray().GetAt(_vertexCount)[1];
        }
        break;
        }
    }
    break;
    }
}
