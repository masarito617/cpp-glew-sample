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

    // UVセット名の取得
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);
    const char* uvSetName = uvSetNameList.GetStringAt(0); // TODO 複数ある場合を考慮しなければならない

    FbxArray<FbxVector2> uvSets;
    mesh->GetPolygonVertexUVs(uvSetName, uvSets);

    // 頂点座標の読込
    std::vector<std::vector<float>> vertexList;
    for (int i = 0; i < mesh->GetControlPointsCount(); i++)
    {
        FbxVector4 point = mesh->GetControlPointAt(i);
        std::vector<float> vertex;
        vertex.push_back(point[0]);
        vertex.push_back(point[1]);
        vertex.push_back(point[2]);
        vertexList.push_back(vertex);
    }

    // インデックス座標の読込
    std::vector<int> vertexIndexList;
    std::vector<std::vector<int>> newVertexIndexList;
    // ポリゴンごとにループ
    int polCount = mesh->GetPolygonCount();
    for (int polIndex = 0; polIndex < polCount; polIndex++)
    {
        // 頂点ごとにループ
        int polVertexCount = mesh->GetPolygonSize(polIndex);
        for (int polVertexIndex = 0; polVertexIndex < polVertexCount; polVertexIndex++)
        {
            // 頂点インデックスの取得
            int vertexIndex = mesh->GetPolygonVertex(polIndex, polVertexIndex);
            std::vector<float> vertex = vertexList[vertexIndex];

            // UV座標の取得
            FbxVector2 uvVec2;
            bool isUnMapped;
            mesh->GetPolygonVertexUV(polIndex, polVertexIndex, uvSetName, uvVec2, isUnMapped);

            if (vertex.size() == 3)
            {
                // UV座標未設定の場合、設定する
                vertex.push_back(uvVec2[0]);
                vertex.push_back(uvVec2[1]);
                vertexList[vertexIndex] = vertex;
            }
            else if (!(fabs(vertex[3] - uvVec2[0] < FLT_EPSILON) && fabs(vertex[4] - uvVec2[1] < FLT_EPSILON)))
            {
                // ＊同一頂点インデックスの中でUV座標が異なる場合、
                // 新たな頂点インデックスとして作成する

                // 作成済かどうか？
                bool isCreated = false;
                for (int i = 0; i < newVertexIndexList.size(); i++)
                {
                    auto indexInfo = newVertexIndexList[i];
                    int oldIndex = indexInfo[0];
                    int newIndex = indexInfo[1];
                    if (oldIndex == vertexIndex
                    && fabs(vertexList[newIndex][3] - uvVec2[0] < FLT_EPSILON)
                    && fabs(vertexList[newIndex][4] - uvVec2[1] < FLT_EPSILON))
                    {
                        isCreated = true;
                        vertexIndex = newIndex;
                        break;
                    }
                }
                // 作成済でない場合
                if (!isCreated)
                {
                    // 新たな頂点インデックスとして作成
                    std::vector<float> newPosVertex;
                    newPosVertex.push_back(vertex[0]);
                    newPosVertex.push_back(vertex[1]);
                    newPosVertex.push_back(vertex[2]);
                    newPosVertex.push_back(uvVec2[0]);
                    newPosVertex.push_back(uvVec2[1]);
                    vertexList.push_back(newPosVertex);
                    // 作成したインデックス情報を設定
                    int newIndex = vertexList.size() - 1;
                    std::vector<int> newVertexIndex;
                    newVertexIndex.push_back(vertexIndex); // old index
                    newVertexIndex.push_back(newIndex);    // new index
                    newVertexIndexList.push_back(newVertexIndex);
                    vertexIndex = newIndex;
                }
            }
            // インデックスバッファを追加
            vertexIndexList.push_back(vertexIndex);
        }
    }

    // 頂点座標配列の作成
    int vertexCount = vertexList.size();
    float vertices[vertexList.size() * 8];
    for (int i = 0; i < vertexList.size(); i++)
    {
        auto vertex = vertexList[i];
        // 位置座標
        vertices[i*8+0] = vertex[0];
        vertices[i*8+1] = vertex[1];
        vertices[i*8+2] = vertex[2];
        // TODO 法線座標の設定
        vertices[i*8+3] = 0.0f;
        vertices[i*8+4] = 0.0f;
        vertices[i*8+5] = 0.0f;
        // UV座標
        vertices[i*8+6] = vertex[3];
        vertices[i*8+7] = -vertex[4]; // V座標は反転させる

        // TODO ログ出力
        for (int j = 0; j < 8; j++)
        {
            std::cout << vertices[i*8+j] << ", ";
        }
        std::cout << std::endl;
    }

    // インデックスバッファ配列の作成
    int indexCount = vertexIndexList.size();
    unsigned int indices[vertexIndexList.size()];
    for (int i = 0; i < vertexIndexList.size(); i++)
    {
        auto vertexIndex = vertexIndexList[i];
        indices[i] = vertexIndex; // new
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
