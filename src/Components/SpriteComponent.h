#pragma once
#include <SDL.h>
#include "Component.h"

// スプライトコンポーネントクラス
// *描画を行うコンポーネントはこのクラスを継承する
class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* actor, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader); // 描画処理

protected:
    class Texture* mTexture; // テクスチャ
    int mDrawOrder; // 描画順

public:
    // Getter, Setter
    int GetDrawOrder() const { return mDrawOrder; }
    void SetTexture(class Texture* texture) { mTexture = texture; }
};
