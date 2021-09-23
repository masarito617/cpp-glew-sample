#include "Mesh.h"
#include "VertexArray.h"
#include <fbxsdk.h>
#include <SDL.h>

Mesh::Mesh()
:mVertexArray(nullptr)
,mRadius(0.0f)
{}

Mesh::~Mesh()
{}

bool Mesh::Load(const std::string &filePath)
{
    // マネージャーの初期化
    FbxManager* manager = FbxManager::Create();

    // インポーターの初期化
    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings()))
    {
        SDL_Log("failed fbx initialize importer.");
        return false;
    }

    // シーンの作成
    FbxScene* scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    importer->Destroy();

    // 三角ポリゴンへのコンバート
    FbxGeometryConverter geometryConverter(manager);
    if (!geometryConverter.Triangulate(scene, true))
    {
        SDL_Log("failed fbx convert triangle.");
        return false;
    }

    // メッシュ取得
    FbxMesh* mesh = scene->GetSrcObject<FbxMesh>();
    if (!mesh)
    {
        SDL_Log("failed fbx scene get mesh.");
        return false;
    }

    // 頂点座標の読込
    int vertexCount = mesh->GetControlPointsCount();
    float vertices[vertexCount * 8]; // 頂点数*8(position,normal,u,v)
    for (int i = 0; i < vertexCount; i++)
    {
        int index = i * 8;
        // position(x,y,z)
        FbxVector4 point = mesh->GetControlPointAt(i);
        vertices[index + 0] = point[0];
        vertices[index + 1] = point[1];
        vertices[index + 2] = point[2];
        // TODO 法線、UV値の設定
        vertices[index + 3] = 0.0f;
        vertices[index + 4] = 0.0f;
        vertices[index + 5] = 0.0f;
        vertices[index + 6] = 0.0f;
        vertices[index + 7] = 0.0f;
    }

    // インデックスバッファの読込
    int indexCount = mesh->GetPolygonVertexCount();
    unsigned int indices[indexCount];
    for (int i = 0; i < indexCount; i++)
    {
        indices[i] = mesh->GetPolygonVertices()[i];
    }

    // 頂点クラスの初期化
    mVertexArray = new VertexArray(vertices, vertexCount, indices, indexCount);

    // マネージャー、シーンの破棄
    scene->Destroy();
    manager->Destroy();

    return true;
}

void Mesh::Unload()
{
    delete mVertexArray;
    mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
    return index < mTextures.size() ? mTextures[index] : nullptr;
}
