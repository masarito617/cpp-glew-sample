#pragma once
#include "Actor.h"

// カメラアクタ
class Camera : public Actor {
public:
    Camera(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ProcessInput(const uint8_t* state) override;

private:
    class Actor* mTargetActor;

public:
    void SetTargetActor(Actor* actor) { mTargetActor = actor; }

};
