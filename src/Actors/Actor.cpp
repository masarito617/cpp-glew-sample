#include "Actor.h"
#include <vector>
#include "../Game.h"
#include "../Components/Component.h"
#include "../Components/SpriteComponent.h"

Actor::Actor(Game* game)
:mState(EActive)
,mPosition(Math::VEC3_ZERO)
,mScale(Math::VEC3_UNIT)
,mRotation(Quaternion())
,mGame(game)
,mRecalculateWorldTransform(true)
{
    // アクタ追加
    mGame->AddActor(this);
}

Actor::~Actor()
{
    // アクタ削除
    mGame->RemoveActor(this);
    // コンポーネント削除
    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
}

// 更新処理
void Actor::Update(float deltaTime)
{
    if (mState == EActive)
    {
        CalculateWouldTransform();
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

// コンポーネント更新処理
void Actor::UpdateComponents(float deltaTime)
{
    for (auto component : mComponents)
    {
        component->Update(deltaTime);
    }
}

// アクタ更新処理
void Actor::UpdateActor(float deltaTime)
{
}

// キー入力処理
void Actor::ProcessInput(const uint8_t *state)
{
}

// コンポーネント追加
void Actor::AddComponent(Component* component)
{
    // 設定された更新順となるようソートする
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); ++iter)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }
    mComponents.insert(iter, component);
}

// コンポーネント削除
void Actor::RemoveComponent(Component* component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}

// ワールド変換座標計算処理
void Actor::CalculateWouldTransform()
{
    // ワールド変換座標を再計算する
    if (mRecalculateWorldTransform)
    {
        // 拡大縮小 -> 回転 -> 平行移動
        // を逆の順番で乗算する。
        mRecalculateWorldTransform = false;
        mWorldTransform = Matrix4::CreateTranslation(mPosition.x, mPosition.y, mPosition.z);
        mWorldTransform *= Matrix4::CreateQuaternion(mRotation);
        mWorldTransform *= Matrix4::CreateScale(mScale.x, mScale.y, mScale.z);
    }
}

// 前方ベクトルを取得する
Vector3 Actor::GetForward() const
{
    // Z方向の単位ベクトルとクォータニオンから計算
    return Quaternion::RotateVec(Math::VEC3_UNIT_Z, mRotation);
}

// クォータニオンを加えて回転させる
void Actor::SetRotationX(float radian)
{
    Quaternion q(Math::VEC3_UNIT_X, radian);
    mRotation = Quaternion::Concatenate(mRotation, q);
    mRecalculateWorldTransform = true;
}
void Actor::SetRotationY(float radian)
{
    Quaternion q(Math::VEC3_UNIT_Y, radian);
    mRotation = Quaternion::Concatenate(mRotation, q);
    mRecalculateWorldTransform = true;
}
void Actor::SetRotationZ(float radian)
{
    Quaternion q(Math::VEC3_UNIT_Z, radian);
    mRotation = Quaternion::Concatenate(mRotation, q);
    mRecalculateWorldTransform = true;
}
