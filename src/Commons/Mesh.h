#pragma once
#include <vector>
#include <string>

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

    std::string mShaderName; // シェーダ名
    float mRadius;           // 境界球の半径（原点から最も遠い点までの距離）

public:
    class VertexArray* GetVertexArray() { return mVertexArray; }

};
