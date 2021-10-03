#include "SpriteComponent.h"
#include <GL/glew.h>
#include "../Game.h"
#include "../Actors/Actor.h"
#include "../Commons/Texture.h"

SpriteComponent::SpriteComponent(class Actor *actor, int drawOrder)
:Component(actor)
,mTexture(nullptr)
,mDrawOrder(drawOrder)
{
    // 描画中のスプライトとして追加
    mActor->GetGame()->GetRenderer()->AddSpriteComp(this);
}

SpriteComponent::~SpriteComponent()
{
    // 描画中のスプライトから削除
    mActor->GetGame()->GetRenderer()->RemoveSpriteComp(this);
}

void SpriteComponent::Draw(Shader* shader)
{
    if (!mTexture) return;

    // テクスチャサイズを考慮したワールド変換座標を設定
    Matrix4 scaleMatrix = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()),
                                               static_cast<float>(mTexture->GetHeight()),
                                               1.0f);
    Matrix4 world = mActor->GetWorldTransform() * scaleMatrix;
    shader->SetWorldTransformUniform(world);

    // テクスチャをアクティブにする
    mTexture->SetActive();

    // 設定されたシェーダを描画
    glDrawElements(
        GL_TRIANGLES,   // 描画するポリゴンの種類
        6,        // インデックスバッファの数
        GL_UNSIGNED_INT, // インデックスの型
        nullptr   // 設定済のためnullptr
    );
}
