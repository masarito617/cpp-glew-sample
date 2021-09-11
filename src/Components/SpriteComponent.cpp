#include "SpriteComponent.h"
#include "GL/glew.h"
#include "../Game.h"
#include "../Actors/Actor.h"
#include "../Commons/Shader.h"

SpriteComponent::SpriteComponent(class Actor *actor, int drawOrder)
:Component(actor)
,mDrawOrder(drawOrder)
{
    // 描画中のスプライトとして追加
    mActor->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    // 描画中のスプライトから削除
    mActor->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
    // TODO 座標変換も行う
    // 設定されたシェーダを描画
    glDrawElements(
        GL_TRIANGLES,   // 描画するポリゴンの種類
        6,        // インデックスバッファの数
        GL_UNSIGNED_INT, // インデックスの型
        nullptr   // 設定済のためnullptr
    );
}
