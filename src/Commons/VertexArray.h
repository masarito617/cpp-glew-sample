#pragma once

// 頂点配列クラス
class VertexArray
{
public:
    VertexArray(const float* vertices,
                unsigned int numVertices,
                const unsigned int* indices,
                unsigned int numIndices);
    ~VertexArray();

    void SetActive();

private:
    unsigned int mNumVertices;  // 頂点バッファの頂点数
    unsigned int mNumIndices;   // インデックスバッファの頂点数
    unsigned int mVertexBuffer; // 頂点バッファのOpenGLID
    unsigned int mIndexBuffer;  // インデックスバッファのOpenGLID
    unsigned int mVertexArray;  // 頂点配列オブジェクトのOpenGLID

public:
    unsigned int GetNumIndices() { return mNumIndices; }

};
