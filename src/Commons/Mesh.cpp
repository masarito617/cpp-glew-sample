#include "Mesh.h"
#include "VertexArray.h"
#include <fbxsdk.h>
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>

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

    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);

    // 頂点座標の読込
    int vertexCount = mesh->GetControlPointsCount();
    std::map<int, std::vector<float>> cachedVerticesMap; // Indexでソートするためmapを使用する

    // ポリゴンごとにループする
    int polCount = mesh->GetPolygonCount();
    for (int polIndex = 0; polIndex < polCount; polIndex++)
    {
        // 頂点数の取得
        int polVertexCount = mesh->GetPolygonSize(polIndex);
        for (int polVertexIndex = 0; polVertexIndex < polVertexCount; polVertexIndex++)
        {
            // 頂点インデックスからポイントを取得
            int vertexIndex = mesh->GetPolygonVertex(polIndex, polVertexIndex);
            // キャッシュ済なら飛ばす
            auto iter = cachedVerticesMap.find(vertexIndex);
            if (iter != cachedVerticesMap.end())
            {
                continue;
            }

            // 頂点座標の設定
            FbxVector4 point = mesh->GetControlPointAt(vertexIndex);
            std::vector<float> vertices;
            vertices.push_back(point[0]);
            vertices.push_back(point[1]);
            vertices.push_back(point[2]);

            // TODO 法線の設定
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            // TODO UV値の設定
            // const char* uvSetName = uvSetNameList.GetStringAt(i);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            cachedVerticesMap.emplace(vertexIndex, vertices);
        }
    }

    // キャッシュした情報を頂点座標配列に変換
    float vertices[cachedVerticesMap.size() * 8];
    int index = 0;
    for (auto cachedVertices : cachedVerticesMap)
    {
        for (auto vertexInfo : cachedVertices.second)
        {
            vertices[index++] = vertexInfo;
        }
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
