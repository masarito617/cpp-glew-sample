#pragma once
#include "Actor.h"

// 宇宙船クラス
class Ship : public Actor {
public:
    Ship(class Game* game);
    void UpdateActor(float deltaTime) override; // アクタ更新
    void ProcessKeyboard(const uint8_t* state); // キーボード入力
};
