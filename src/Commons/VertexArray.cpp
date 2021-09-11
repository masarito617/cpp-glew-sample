#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(const float *vertices,
                         unsigned int numVertices,
                         const unsigned int *indices,
                         unsigned int numIndices)
:mNumVertices(numVertices)
,mNumIndices(numIndices)
{
    // 頂点配列オブジェクトの作成
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    // 頂点バッファの作成
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,                 // バッファの種類
                 numVertices * 3 * sizeof(float), // コピーするバイト数(x, y, z)
                 vertices,                        // コピー元
                 GL_STATIC_DRAW);                 // データの利用方法

    // インデックスバッファの作成
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // バッファの種類
                 numIndices * sizeof(unsigned int), // コピーするバイト数
                 indices,                           // コピー元
                 GL_STATIC_DRAW);                   // データの利用方法

    // 頂点レイアウトの指定
    // 頂点属性0: x,y,z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
    glBindVertexArray(mVertexArray);
}
