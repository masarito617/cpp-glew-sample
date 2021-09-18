#include "Actor.h"
#include <vector>
#include "../Game.h"
#include "../Components/Component.h"
#include "../Components/SpriteComponent.h"

Actor::Actor(Game* game)
:mState(EActive)
,mPosition(Vector2::Zero)
,mScale(1.0f)
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
        // 2DのためZは固定
        mRecalculateWorldTransform = false;
        mWorldTransform = Matrix4::CreateTranslation(mPosition.x, mPosition.y, 0.0f);
        mWorldTransform *= Matrix4::CreateQuaternion(mRotation);
        mWorldTransform *= Matrix4::CreateScale(mScale, mScale, 1.0f);
        // TODO ログ出力
        Matrix4::CreateScale(mScale, mScale, 1.0f).PrintMatrix();
        Matrix4::CreateQuaternion(mRotation).PrintMatrix();
        Matrix4::CreateTranslation(mPosition.x, mPosition.y, 0.0f).PrintMatrix();
        mWorldTransform.PrintMatrix();
    }
}

// TODO 全ての軸で機能するようにする
// Z軸方向の回転を設定する
void Actor::SetRotationZ(float radian)
{
    //Quaternion q(0.0f, 0.0f, 1.0f, radian);
    Quaternion q(Math::VEC3_UNIT_Z, radian);
    mRotation = Quaternion::Concatenate(mRotation, q);
}
