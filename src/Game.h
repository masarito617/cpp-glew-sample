#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_map>
#include "Commons/Math.h"

// ゲーム管理クラス
// *ゲーム全体の流れ、アクタ生成、テクスチャ読込を行う
class Game
{
public:
    Game();
    bool Initialize(); // ゲーム初期化
    void RunLoop();    // ゲームループ処理
    void Shutdown();   // シャットダウン処理

    void AddActor(class Actor* actor);    // アクタ追加
    void RemoveActor(class Actor* actor); // アクタ削除

    void AddSpriteComp(class SpriteComponent* sprite);    // アクタのスプライトを追加
    void RemoveSpriteComp(class SpriteComponent* sprite); // アクタのスプライトを削除

    void AddMeshComp(class MeshComponent* mesh);    // アクタのメッシュを追加
    void RemoveMeshComp(class MeshComponent* mesh); // アクタのメッシュを削除

    class Texture* GetTexture(const std::string& filePath); // テクスチャ取得処理
    class Mesh* GetMesh(const std::string& filePath);       // メッシュ取得処理

    constexpr static const float ScreenWidth  = 1024.0f; // スクリーン横幅
    constexpr static const float ScreenHeight = 768.0f;  // スクリーン縦幅

private:
    bool InitSDL();        // SDL関連初期化
    bool LoadShaders();    // シェーダのロード処理
    void Update();         // シーン更新処理
    void ProcessInput();   // 入力検知
    void GenerateOutput(); // 出力処理

    std::vector<class Actor*> mActors;            // アクタリスト
    std::vector<class Actor*> mPendingActors;     // 待機中のアクタリスト
    std::vector<class SpriteComponent*> mSpriteComps; // アクタのスプライトリスト
    std::vector<class MeshComponent*> mMeshComps;     // アクタのメッシュリスト
    std::unordered_map<std::string, class Texture*> mCachedTextures; // キャッシュ済テクスチャリスト
    std::unordered_map<std::string, class Mesh*> mCachedMeshes;      // キャッシュ済メッシュリスト

    class Shader* mShader;     // シェーダ
    Matrix4 mViewMatrix;       // ビュー変換行列
    Matrix4 mProjectionMatrix; // 射影変換行列

    SDL_Window* mWindow;     // SDLウィンドウ
    SDL_GLContext mContext;  // SDLコンテキスト
    Uint32 mTicksCount;      // ゲーム時間
    bool mIsRunning;         // 実行中か否か？
    bool mUpdatingActors;    // アクタ更新中か否か？
    
    // Assetsパス
    const std::string AssetsPath = "../Assets/"; // Mac + CLion
    //const std::string AssetsPath = "Assets\\"; // Win + VisualStudio

    // シェーダーパス
    const std::string ShaderPath = "../src/Shaders/"; // Mac + CLion

    // カメラ
    class Camera* mCamera;

public:
    // getter, setter
    std::vector<class Actor*> GetActors() { return mActors; }
    std::string GetAssetsPath() const { return AssetsPath; }
    void SetViewMatrix(const Matrix4& view) { mViewMatrix = view; }

};
