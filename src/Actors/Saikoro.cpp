#include "Saikoro.h"
#include "../Game.h"
#include "../Commons/Mesh.h"
#include "../Components/MeshComponent.h"

Saikoro::Saikoro(class Game *game)
: Actor(game)
{
    // メッシュの設定
    auto* meshComp = new MeshComponent(this);
    auto* mesh = game->GetRenderer()->GetMesh(game->GetAssetsPath() + "saikoro.fbx");
    meshComp->SetMesh(mesh);
}

void Saikoro::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);

    // 回転テスト
    SetRotationX(Math::ToRadians(testRot));
    SetRotationZ(Math::ToRadians(testRot));
}

void Saikoro::ProcessInput(const uint8_t *state)
{
    Actor::ProcessInput(state);
}
