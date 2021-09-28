#include "Game.h"
#include "GL/glew.h"
#include "SDL_image.h"
#include "Actors/Actor.h"
#include "Actors/Camera.h"
#include "Components/SpriteComponent.h"
#include "Components/MeshComponent.h"
#include "Commons/VertexArray.h"
#include "Commons/Shader.h"
#include "Commons/Texture.h"
#include "Commons/Mesh.h"

Game::Game()
:mWindow(nullptr)
,mShader(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mUpdatingActors(false)
{
}

// ゲーム初期化
bool Game::Initialize()
{
    // SDL関連初期化
    if (!InitSDL())
    {
        SDL_Log("%s", SDL_GetError());
        return false;
    }

    // GLEWの初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return false;
    glGetError();

    // シェーダのロード
    if (!LoadShaders())
    {
        SDL_Log("failed load shaders.");
        return false;
    }

    // ゲーム時間取得
    mTicksCount = SDL_GetTicks();

    return true;
}

// SDL関連初期化
bool Game::InitSDL()
{
    // 初期化に失敗したらfalseを返す
    bool success = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == 0;
    if (!success) return false;

    // OpenGL設定
    // コアOpenGLプロファイル
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // バージョン3.3を指定
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // RGBA各チャネル8ビットのカラーバッファ
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // Zバッファのビット数
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // ダブルバッファ
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // ハードウェアアクセラレーション
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // OpenGLウィンドウの作成
    mWindow = SDL_CreateWindow("OpenGLTest",
                               100, 100, ScreenWidth, ScreenHeight,
                               SDL_WINDOW_OPENGL);
    if (!mWindow) return false;

    // OpenGLコンテキストの作成
    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext) return false;

    return true;
}

// シェーダロード処理
bool Game::LoadShaders()
{
    // Phongシェーダを読み込む
    mShader = new Shader();
    if (!mShader->Load(ShaderPath + "PhongVert.glsl",
                       ShaderPath + "PhongFrag.glsl"))
    {
        return false;
    }
    mShader->SetActive();

    // ビュー射影座標を設定(View*Projectionの逆算)
    mViewMatrix = Matrix4::CreateLookAt(Math::VEC3_ZERO, Math::VEC3_UNIT_Z, Math::VEC3_UNIT_Y); // カメラ無しの初期値
    mProjectionMatrix = Matrix4::CreatePerspectiveFOV(Math::ToRadians(50.0f), ScreenWidth, ScreenHeight,
                                                      25.0f, 10000.0f);
    mShader->SetMatrixUniform(mShader->UNIFORM_VIEW_PROJECTION_NAME, mProjectionMatrix * mViewMatrix);
    return true;
}

// ゲームループ処理
void Game::RunLoop()
{
    // カメラ作成
    mCamera = new Camera(this);
    mCamera->SetPosition(Vector3(0.0f, 0.0f, -450.0f));

    // TODO アクタの作成
    testActor = new Actor(this);
    testActor->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    testActor->SetScale(Vector3(100.0f, 100.0f, 100.0f));
    // メッシュ設定
    auto* meshComp = new MeshComponent(testActor);
    auto* mesh = new Mesh();
    mesh->Load(AssetsPath + "saikoro.fbx", this);
    meshComp->SetMesh(mesh);

    while (mIsRunning)
    {
        ProcessInput();   // 入力検知
        Update();         // シーン更新処理
        GenerateOutput(); // 出力処理
    }
}

// シーン更新処理
void Game::Update()
{
    // 最低16msは待機
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    // フレームの経過時間を取得(最大50ms)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();

    // アクタ更新処理
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // 待機中のアクタを追加
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // 死亡したアクタを破棄
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }
    for (auto actor : deadActors)
    {
        delete actor;
    }

    // TODO 回転テスト
    testActor->SetRotationX(Math::ToRadians(testRot));
    testActor->SetRotationZ(Math::ToRadians(testRot));
}

// ゲームループ 入力検知
void Game::ProcessInput()
{
    // SDLイベント
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT: // ウィンドウが閉じられた時
            mIsRunning = false;
            break;
        }
    }
    // キー入力イベント
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    // 各アクタの入力イベント
    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
}

// ゲームループ 出力処理
void Game::GenerateOutput()
{
    // 背景色をクリア
    glClearColor(0.2f,
                 0.2f,
                 0.2f,
                 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // シェーダをアクティブにする
    mShader->SetActive();
    // カメラオブジェクトで変更されたビュー射影行列を再設定
    mShader->SetMatrixUniform(mShader->UNIFORM_VIEW_PROJECTION_NAME, mProjectionMatrix * mViewMatrix);

    // ライティングのパラメータを設定
    mShader->SetVectorUniform("uCameraPos", mCamera->GetPosition()); // TODO カメラ位置ではだめ？
    mShader->SetVectorUniform("uAmbientColor", Vector3(0.35f, 0.35f, 0.35f));
    mShader->SetFloatUniform("uSpecPower", 300.0f);
    mShader->SetVectorUniform("uDirLight.mDirection", Vector3(0.3f, 0.3f, 0.8f));
    mShader->SetVectorUniform("uDirLight.mDiffuseColor", Vector3(0.78f, 0.88f, 1.0f));
    mShader->SetVectorUniform("uDirLight.mSpecColor", Vector3(0.8f, 0.8f, 0.8f));

    // Zバッファ有効、アルファブレンド無効
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // メッシュ描画
    for (auto mesh : mMeshes)
    {
        mesh->Draw(mShader);
    }

    // Zバッファ無効、アルファブレンド有効
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // TODO スプライト描画
//    for (auto sprite : mSprites)
//    {
//        sprite->Draw(mShader);
//    }

    // バックバッファとスワップ(ダブルバッファ)
    SDL_GL_SwapWindow(mWindow);
}

// シャットダウン処理
void Game::Shutdown()
{
    // アクタを破棄
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    // テクスチャを破棄
    for (auto i : mCachedTextures)
    {
        i.second->Unload();
        delete i.second;
    }
    mCachedTextures.clear();

    // シェーダの破棄
    mShader->Unload();
    delete mShader;

    // SDL関連の変数を破棄
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

// アクタ追加処理
void Game::AddActor(Actor* actor)
{
    // アクタ更新中ならPendingに格納
    mUpdatingActors
    ? mPendingActors.emplace_back(actor)
    : mActors.emplace_back(actor);
}

// アクタ削除処理
void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        mPendingActors.erase(iter);
    }
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        mActors.erase(iter);
    }
}

// スプライト追加処理
void Game::AddSprite(SpriteComponent* sprite)
{
    // 描画順にソートして追加
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for (; iter != mSprites.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }
    mSprites.insert(iter, sprite);
}

// スプライト削除処理
void Game::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

// メッシュを追加
void Game::AddMesh(class MeshComponent* mesh)
{
    mMeshes.emplace_back(mesh);
}

// メッシュを削除
void Game::RemoveMesh(class MeshComponent* mesh)
{
    auto iter = std::find(mMeshes.begin(), mMeshes.end(), mesh);
    mMeshes.erase(iter);
}

// テクスチャロード処理
Texture* Game::GetTexture(const std::string &filePath)
{
    // キャッシュ済なら返却
    auto iter = mCachedTextures.find(filePath);
    if (iter != mCachedTextures.end())
    {
        return iter->second;
    }

    // テクスチャをロードする
    Texture* texture = nullptr;
    texture = new Texture();
    if (texture->Load(filePath))
    {
        mCachedTextures.emplace(filePath, texture);
    }
    else
    {
        delete texture;
        texture = nullptr;
    }
    return texture;
}
