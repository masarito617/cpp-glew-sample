#pragma once
#include <SDL.h>
#include <unordered_map>
#include "../Commons/Math.h"

// 描画クラス
class Renderer {
public:
    Renderer(class Game* game);
    ~Renderer();

    bool Initialize(); // 初期化処理
    bool LoadData();   // データロード処理
    void Draw();       // 描画処理
    void ShutDown();   // 終了処理

    void AddSpriteComp(class SpriteComponent* sprite);      // スプライトコンポーネント追加
    void RemoveSpriteComp(class SpriteComponent* sprite);   // スプライトコンポーネント削除
    void AddMeshComp(class MeshComponent* mesh);            // メッシュコンポーネント追加
    void RemoveMeshComp(class MeshComponent* mesh);         // メッシュコンポーネント削除
    class Texture* GetTexture(const std::string& filePath); // テクスチャ取得、キャッシュ
    class Mesh* GetMesh(const std::string& filePath);       // メッシュ取得、キャッシュ

private:
    bool InitSDL(); // SDL関連初期化

    class Game* mGame;
    SDL_Window* mWindow;    // SDLウィンドウ
    SDL_GLContext mContext; // SDLコンテキスト

    class Camera* mCamera;     // カメラ
    class Shader* mShader;     // シェーダ
    Matrix4 mViewMatrix;       // ビュー変換行列
    Matrix4 mProjectionMatrix; // 射影変換行列

    // ライティングパラメータ
    Vector3 mAmbientLight;         // 環境光
    Vector3 mDirLightDirection;    // 平行光源 向き
    Vector3 mDirLightDiffuseColor; // 平行光源 拡散反射色 kd
    Vector3 mDirLightSpecColor;    // 平行光源 鏡面反射色 ks
    float mSpecPower;              // 鏡面反射指数 a

    std::vector<class SpriteComponent*> mSpriteComps; // アクタのスプライトリスト
    std::vector<class MeshComponent*> mMeshComps;     // アクタのメッシュリスト
    std::unordered_map<std::string, class Texture*> mCachedTextures; // キャッシュ済テクスチャリスト
    std::unordered_map<std::string, class Mesh*> mCachedMeshes;      // キャッシュ済メッシュリスト

public:
    void SetViewMatrix(const Matrix4& view) { mViewMatrix = view; }
    class Camera* GetCamera() const { return mCamera; }
    const Vector3& GetAmbientLight() const { return mAmbientLight; }
    const Vector3& GetDirLightDirection() const { return mDirLightDirection; }
    const Vector3& GetDirLightDiffuseColor() const { return mDirLightDiffuseColor; }
    const Vector3& GetDirLightSpecColor() const { return mDirLightSpecColor; }
    const float GetSpecPower() const { return mSpecPower; }

};
