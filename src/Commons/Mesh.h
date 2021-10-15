#pragma once
#include <vector>
#include <string>
#include <fbxsdk.h>

// モデルクラス
class Mesh {
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName, class Game* game);
    void Unload();

    class Texture* GetTexture();

private:
    // 読み込んだモデル情報
    class VertexArray* mVertexArray; // 頂点座標
    class Texture* mTexture;         // テクスチャ

    // 頂点情報の編集で使用
    std::vector<float> CreateVertexInfo(const std::vector<float>& vertex,
                       const FbxVector4& normalVec4, const FbxVector2& uvVec2);
    bool IsEqualNormalUV(const std::vector<float> vertexInfo,
                         const FbxVector4& normalVec4, const FbxVector2& uvVec2);

public:
    class VertexArray* GetVertexArray() { return mVertexArray; }

};
