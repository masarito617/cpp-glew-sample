#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "Commons/Math.h"
#include "Commons/Renderer.h"

// ゲーム管理クラス
// *ゲーム全体の流れ、アクタ生成、テクスチャ読込を行う
class Game
{
public:
    Game();
    bool Initialize(); // ゲーム初期化
    bool LoadData();   // データロード処理
    void RunLoop();    // ゲームループ処理
    void Shutdown();   // シャットダウン処理

    // アクタ追加・削除
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    constexpr static const float ScreenWidth  = 1024.0f; // スクリーン横幅
    constexpr static const float ScreenHeight = 768.0f;  // スクリーン縦幅

private:
    void Update();         // シーン更新処理
    void ProcessInput();   // 入力検知
    void GenerateOutput(); // 出力処理

    std::vector<class Actor*> mActors;        // アクタリスト
    std::vector<class Actor*> mPendingActors; // 待機中のアクタリスト

    class Renderer* mRenderer;

    Uint32 mTicksCount;   // ゲーム時間
    bool mIsRunning;      // 実行中か否か？
    bool mUpdatingActors; // アクタ更新中か否か？
    
    // Mac + CLion環境での相対パス
    const std::string AssetsPath = "../Assets/";      // Assetsパス
    const std::string ShaderPath = "../src/Shaders/"; // シェーダーパス

    // Win + VisualStudio環境での相対パス
    //const std::string AssetsPath = "Assets\\";       // Assetsパス
    //const std::string ShaderPath = "src\\Shaders\\"; // シェーダーパス

public:
    // getter, setter
    std::string GetAssetsPath() const { return AssetsPath; }
    std::string GetShaderPath() const { return ShaderPath; }
    class Renderer* GetRenderer() const { return mRenderer; }

};
