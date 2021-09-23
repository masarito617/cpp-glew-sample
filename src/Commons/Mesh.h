#pragma once
#include <vector>
#include <string>

// モデルクラス
class Mesh {
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName);
    void Unload();

    class Texture* GetTexture(size_t index);

private:
    // 読み込んだモデル情報
    std::vector<class Texture*> mTextures; // テクスチャ
    class VertexArray* mVertexArray;       // 頂点座標

    std::string mShaderName; // シェーダ名
    float mRadius;           // 境界球の半径（原点から最も遠い点までの距離）

public:
    class VertexArray* GetVertexArray() { return mVertexArray; }

};
