#include "Ship.h"
#include "../Game.h"
#include "../Components/SpriteComponent.h"

Ship::Ship(Game* game)
:Actor(game)
{
    // TODO スプライト設定
}

// アクタ更新
void Ship::UpdateActor(float deltaTime)
{
    // 親のメソッド呼び出し
    Actor::UpdateActor(deltaTime);
}

// キーボード入力
void Ship::ProcessKeyboard(const uint8_t* state)
{
}
