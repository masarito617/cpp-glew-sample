#include "Game.h"
#include "GL/glew.h"
#include "SDL_image.h"
#include "Actors/Actor.h"
#include "Components/SpriteComponent.h"
#include "Commons/VertexArray.h"
#include "Commons/Shader.h"
#include "Commons/Texture.h"

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

    // 頂点配列の作成
    CreateSpriteVertices();

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
    // ダブルバッファ
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // ハードウェアアクセラレーション
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // OpenGLウィンドウの作成
    mWindow = SDL_CreateWindow("ShootingGame",
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
    mShader = new Shader();
    if (!mShader->Load(ShaderPath + "SpriteVert.glsl",
                       ShaderPath + "SpriteFrag.glsl"))
    {
        return false;
    }
    mShader->SetActive();

    // ビュー射影座標を設定
    Matrix4 viewProjection = Matrix4::CreateSimpleViewProjection(ScreenWidth, ScreenHeight);
    mShader->SetMatrixUniform(mShader->UNIFORM_VIEW_PROJECTION_NAME, viewProjection);
    viewProjection.PrintMatrix();
    return true;
}

// 頂点作成処理
void Game::CreateSpriteVertices()
{
    // 頂点バッファ(x,y,z,u,v)
    float vertices[] = {
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // TOP LEFT
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // TOP RIGHT
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // BOTTOM RIGHT
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // BOTTOM LEFT
    };
    // インデックスバッファ
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };
    mVertices = new VertexArray(vertices, 4, indices, 6);
}

// ゲームループ処理
void Game::RunLoop()
{
    // アクタの作成
    auto* actor = new Actor(this);
    actor->SetRotation(Math::ToRadians(30.0f)); // Z軸の方向より、逆方向に回転するため注意
    actor->SetPosition(Vector2(50.0f, -100.0f));
    actor->SetScale(1.5f);
    // テクスチャ設定
    auto* sprite = new SpriteComponent(actor);
    sprite->SetTexture(GetTexture(AssetsPath + "ship.png"));

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
}

// ゲームループ 出力処理
void Game::GenerateOutput()
{
    // 背景色をクリア
    glClearColor(0.2f,
                 0.2f,
                 0.2f,
                 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // アルファブレンドを有効にする
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // シェーダ、頂点配列をアクティブにする
    mShader->SetActive();
    mVertices->SetActive();

    // スプライト描画
    for (auto sprite : mSprites)
    {
        sprite->Draw(mShader);
    }

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

    // 頂点、シェーダの破棄
    delete mVertices;
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

// 描画中のスプライト追加処理
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

// 描画中のスプライト削除処理
void Game::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
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
