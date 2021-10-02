#pragma once
#include "Actor.h"

// サイコロクラス
class Saikoro : public Actor{
public:
    Saikoro(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ProcessInput(const uint8_t* state) override;

private:
    // 回転テスト用
    float testRot = 1.0f;
};
