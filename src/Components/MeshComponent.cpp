#include "MeshComponent.h"
#include "../Game.h"
#include "../Commons/Shader.h"
#include "../Commons/Texture.h"
#include "../Commons/Mesh.h"
#include "../Commons/VertexArray.h"
#include "../Actors/Actor.h"

MeshComponent::MeshComponent(class Actor *actor)
: Component(actor)
, mMesh(nullptr)
{
    mActor->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
    mActor->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(class Shader *shader)
{
    if (!mMesh) return;

    // シェーダにワールド座標を設定
    Matrix4 world = mActor->GetWorldTransform();
    shader->SetMatrixUniform(shader->UNIFORM_WOULD_TRANSFORM_NAME, world);

    // テクスチャをアクティブにする
    Texture* texture = mMesh->GetTexture();
    if (texture) texture->SetActive();

    // 頂点座標をアクティブにする
    auto vertexArray = mMesh->GetVertexArray();
    vertexArray->SetActive();

    // シェーダを描画する
    glDrawElements(
        GL_TRIANGLES,   // 描画するポリゴンの種類
        vertexArray->GetNumIndices(), // インデックスバッファの数
        GL_UNSIGNED_INT, // インデックスの型
        nullptr   // 設定済のためnullptr
        );
}
