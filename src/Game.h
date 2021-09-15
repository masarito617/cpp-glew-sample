#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_map>

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

    void AddSprite(class SpriteComponent* sprite);    // 描画中のスプライトを追加
    void RemoveSprite(class SpriteComponent* sprite); // 描画中のスプライトを削除

    class Texture* GetTexture(const std::string& filePath); // テクスチャ取得処理

    constexpr static const float ScreenWidth  = 1024.0f; // スクリーン横幅
    constexpr static const float ScreenHeight = 768.0f;  // スクリーン縦幅

private:
    bool InitSDL();        // SDL関連初期化
    bool LoadShaders();    // シェーダのロード処理
    void CreateSpriteVertices(); // 頂点配列作成処理
    void Update();         // シーン更新処理
    void ProcessInput();   // 入力検知
    void GenerateOutput(); // 出力処理

    std::vector<class Actor*> mActors;            // アクタリスト
    std::vector<class Actor*> mPendingActors;     // 待機中のアクタリスト
    std::vector<class SpriteComponent*> mSprites; // 描画中のスプライトリスト
    std::unordered_map<std::string, class Texture*> mCachedTextures; // キャッシュ済テクスチャリスト

    class Shader* mShader;        // シェーダ
    class VertexArray* mVertices; // 頂点配列

    SDL_Window* mWindow;     // SDLウィンドウ
    SDL_GLContext mContext;  // SDLコンテキスト
    Uint32 mTicksCount;      // ゲーム時間
    bool mIsRunning;         // 実行中か否か？
    bool mUpdatingActors;    // アクタ更新中か否か？
    
    // 画像パス
    const std::string AssetsPath = "../Assets/"; // Mac + CLion
    //const std::string AssetsPath = "Assets\\"; // Win + VisualStudio

    // シェーダーパス
    const std::string ShaderPath = "../src/Shaders/"; // Mac + CLion

public:
    // getter, setter
    std::vector<class Actor*> GetActors() { return mActors; }
    std::string GetAssetsPath() const { return AssetsPath; }
};
