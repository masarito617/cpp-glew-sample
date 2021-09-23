#include "Camera.h"
#include "../Game.h"
#include <SDL.h>

Camera::Camera(class Game *game)
: Actor(game)
, mTargetActor(nullptr)
{}

void Camera::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);

    // カメラ位置よりビュー変換座標を設定する
    Vector3 position = GetPosition();
    Vector3 target = GetPosition() + 100.0f*GetForward(); // 100.0f前方がターゲット
    if (mTargetActor) target = mTargetActor->GetPosition() - GetPosition(); // ターゲットが設定されている場合
    Vector3 up = Math::VEC3_UNIT_Y;
    Matrix4 viewMatrix = Matrix4::CreateLookAt(position, target, up);
    GetGame()->SetViewMatrix(viewMatrix);
}

void Camera::ProcessInput(const uint8_t *state)
{
    Actor::ProcessInput(state);

    // TODO カメラ移動処理
    float moveSpeed = 1.0f;
    Vector3 pos = GetPosition();
    if (state[SDL_SCANCODE_A])
    {
        pos.x += moveSpeed;
    }
    if (state[SDL_SCANCODE_D])
    {
        pos.x -= moveSpeed;
    }
    if (state[SDL_SCANCODE_W])
    {
        pos.y += moveSpeed;
    }
    if (state[SDL_SCANCODE_S])
    {
        pos.y -= moveSpeed;
    }
    if (state[SDL_SCANCODE_UP])
    {
        pos.z += moveSpeed;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        pos.z -= moveSpeed;
    }
    SetPosition(pos);
}
