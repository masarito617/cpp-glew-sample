#include "MeshComponent.h"
#include "../Game.h"
#include "../Commons/Texture.h"
#include "../Commons/Mesh.h"
#include "../Commons/VertexArray.h"
#include "../Actors/Actor.h"

MeshComponent::MeshComponent(class Actor *actor)
: Component(actor)
, mMesh(nullptr)
, mShader(nullptr)
{
    mActor->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
    mActor->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw()
{
    if (!mMesh) return;
    if (!mShader) return;

    // シェーダをアクティブにする
    mShader->SetActive();
    // ビュー射影行列、ライティングパラメータを設定
    auto renderer = mActor->GetGame()->GetRenderer();
    mShader->SetViewProjectionUniform(renderer->GetProjectionMatrix() * renderer->GetViewMatrix());
    mShader->SetLightingUniform(renderer);
    // ワールド座標を設定
    Matrix4 world = mActor->GetWorldTransform();
    mShader->SetWorldTransformUniform(world);

    // テクスチャをアクティブにする
    auto texture = mMesh->GetTexture();
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
