#pragma once
#include <vector>
#include "../Commons/Vector2.h"
#include "../Commons/Math.h"

// アクタクラス
// *各アクタはこのクラスを継承する
class Actor
{
public:
    enum State
    {
        EActive, // アクティブ
        EDead    // 死亡
    };

    Actor(class Game* game);
    virtual ~Actor();

    void Update(float deltaTime);              // 更新処理
    void UpdateComponents(float deltaTime);    // コンポーネント更新処理
    virtual void UpdateActor(float deltaTime); // アクタ更新処理

    void AddComponent(class Component* component);    // コンポーネント追加処理
    void RemoveComponent(class Component* component); // コンポーネント削除処理

    void CalculateWouldTransform(); // ワールド座標計算処理

    void SetRotationZ(float radian); // Z軸の回転処理

private:
    State mState;         // 状態
    Vector2 mPosition;    // 位置
    float mScale;         // 大きさ
    Quaternion mRotation; // 回転
    Matrix4 mWorldTransform;         // ワールド変換座標
    bool mRecalculateWorldTransform; // 再計算フラグ

    std::vector<class Component*> mComponents; // 保有するコンポーネント
    class Game* mGame; // ゲームクラス

public:
    // Getter, Setter
    State GetState() const { return mState; }
    void SetState(const State state) { mState = state; }
    class Game* GetGame() const { return mGame; }
    // 座標の設定は再計算させる
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; mRecalculateWorldTransform = true; }
    float GetScale() const { return mScale; }
    void SetScale(const float scale) { mScale = scale; mRecalculateWorldTransform = true; }
    const Quaternion& GetRotation() const { return mRotation; }
    void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecalculateWorldTransform = true; }
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }

};
