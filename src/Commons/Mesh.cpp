#include "Mesh.h"
#include <SDL.h>
#include <iostream>
#include <vector>
#include "../Game.h"
#include "VertexArray.h"

Mesh::Mesh()
:mVertexArray(nullptr)
,mTexture(nullptr)
{}

Mesh::~Mesh()
{}

bool Mesh::Load(const std::string &filePath, Game* game)
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

    // テクスチャの読込
    // ＊現在の実装だと１つしか読み込めない
    std::string fileName = "default_tex.png";
    int materialCount = scene->GetMaterialCount();
    if (materialCount > 0)
    {
        // マテリアル取得
        FbxSurfaceMaterial* material = scene->GetMaterial(0);
        FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
        int textureCount = property.GetSrcObjectCount();
        if (textureCount > 0)
        {
            // テクスチャ名取得
            FbxFileTexture* fileTexture = scene->GetSrcObject<FbxFileTexture>(0);
            fileName = FbxPathUtils::GetFileName(fileTexture->GetFileName());
        }
    }
    mTexture = game->GetRenderer()->GetTexture(game->GetAssetsPath() + fileName);

    // UVセット名の取得
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);
    const char* uvSetName = uvSetNameList.GetStringAt(0);

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

            // 法線座標の取得
            FbxVector4 normalVec4;
            mesh->GetPolygonVertexNormal(polIndex, polVertexIndex, normalVec4);

            // UV座標の取得
            FbxVector2 uvVec2;
            bool isUnMapped;
            mesh->GetPolygonVertexUV(polIndex, polVertexIndex, uvSetName, uvVec2, isUnMapped);

            if (vertex.size() == 3)
            {
                // 法線座標とUV座標が未設定の場合、頂点情報に付与して設定
                std::vector<float> vertexInfo = CreateVertexInfo(vertex, normalVec4, uvVec2);
                vertexList[vertexIndex] = vertexInfo;
            }
            else if (!IsEqualNormalUV(vertex, normalVec4, uvVec2))
            {
                // ＊同一頂点インデックスの中で法線座標かUV座標が異なる場合、
                // 新たな頂点インデックスとして作成する

                // 新たな頂点インデックスとして作成済かどうか？
                bool isNewVertexCreated = false;
                for (int i = 0; i < newVertexIndexList.size(); i++)
                {
                    int oldIndex = newVertexIndexList[i][0];
                    int newIndex = newVertexIndexList[i][1];
                    if (oldIndex == vertexIndex
                        && IsEqualNormalUV(vertexList[newIndex], normalVec4, uvVec2))
                    {
                        isNewVertexCreated = true;
                        vertexIndex = newIndex;
                        break;
                    }
                }
                // 作成済でない場合
                if (!isNewVertexCreated)
                {
                    // 新たな頂点インデックスとして作成
                    std::vector<float> vertexInfo = CreateVertexInfo(vertex, normalVec4, uvVec2);
                    vertexList.push_back(vertexInfo);
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
    float* vertices;
    vertices = (float*) malloc(sizeof(float) * vertexCount * 8);
    for (int i = 0; i < vertexCount; i++)
    {
        auto vertex = vertexList[i];
        // 位置座標
        vertices[i*8+0] = vertex[0];
        vertices[i*8+1] = vertex[1];
        vertices[i*8+2] = vertex[2];
        // 法線座標
        vertices[i*8+3] = vertex[3];
        vertices[i*8+4] = vertex[4];
        vertices[i*8+5] = vertex[5];
        // UV座標
        vertices[i*8+6] = vertex[6];
        vertices[i*8+7] = -vertex[7]; // V座標は反転させる
    }

    // インデックスバッファ配列の作成
    int indexCount = vertexIndexList.size();
    unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * indexCount);
    for (int i = 0; i < indexCount; i++)
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

// 頂点情報作成処理
std::vector<float> Mesh::CreateVertexInfo(const std::vector<float>& vertex, const FbxVector4& normalVec4,
                         const FbxVector2& uvVec2)
{
    std::vector<float> vertexInfo;
    // 位置座標
    vertexInfo.push_back(vertex[0]);
    vertexInfo.push_back(vertex[1]);
    vertexInfo.push_back(vertex[2]);
    // 法線座標
    vertexInfo.push_back(normalVec4[0]);
    vertexInfo.push_back(normalVec4[1]);
    vertexInfo.push_back(normalVec4[2]);
    // UV座標
    vertexInfo.push_back(uvVec2[0]);
    vertexInfo.push_back(uvVec2[1]);
    return vertexInfo;
}

// vertexInfoに法線、UV座標が設定済かどうか？
bool Mesh::IsEqualNormalUV(const std::vector<float> vertexInfo,
                           const FbxVector4 &normalVec4, const FbxVector2 &uvVec2)
{
    return fabs(vertexInfo[3] - normalVec4[0]) < FLT_EPSILON
            && fabs(vertexInfo[4] - normalVec4[1]) < FLT_EPSILON
            && fabs(vertexInfo[5] - normalVec4[2]) < FLT_EPSILON
            && fabs(vertexInfo[6] - uvVec2[0]) < FLT_EPSILON
            && fabs(vertexInfo[7] - uvVec2[1]) < FLT_EPSILON;
}

void Mesh::Unload()
{
    delete mVertexArray;
    mVertexArray = nullptr;
}

Texture* Mesh::GetTexture()
{
    return mTexture;
}
