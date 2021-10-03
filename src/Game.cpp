#include "Game.h"
#include <GL/glew.h>
#include <SDL_image.h>
#include "Actors/Actor.h"
#include "Actors/Saikoro.h"
#include "Commons/Renderer.h"

Game::Game()
:mTicksCount(0)
,mIsRunning(true)
,mUpdatingActors(false)
{
}

// ゲーム初期化
bool Game::Initialize()
{
    // レンダラー初期化
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize())
    {
        SDL_Log("Failed initialize renderer.");
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }

    // ゲーム時間取得
    mTicksCount = SDL_GetTicks();

    if (!LoadData())
    {
        SDL_Log("failed load data.");
        return false;
    }

    return true;
}

// データロード処理
bool Game::LoadData()
{
    // 描画データロード
    if (!mRenderer->LoadData()) return false;

    // サイコロ作成
    auto* saikoro = new Saikoro(this, Shader::ShaderType::PHONG);
    saikoro->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    saikoro->SetScale(Vector3(50.0f, 50.0f, 50.0f));

    auto* saikoro2 = new Saikoro(this, Shader::ShaderType::SPRITE);
    saikoro2->SetPosition(Vector3(200.0f, 0.0f, 0.0f));
    saikoro2->SetScale(Vector3(50.0f, 50.0f, 50.0f));

    auto* saikoro3 = new Saikoro(this, Shader::ShaderType::BASIC);
    saikoro3->SetPosition(Vector3(-200.0f, 0.0f, 0.0f));
    saikoro3->SetScale(Vector3(50.0f, 50.0f, 50.0f));

    return true;
}

// ゲームループ処理
void Game::RunLoop()
{
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

    // 各アクタの入力イベント
    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
}

// ゲームループ 出力処理
void Game::GenerateOutput()
{
    mRenderer->Draw();
}

// シャットダウン処理
void Game::Shutdown()
{
    // アクタを破棄
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    // レンダラー破棄
    mRenderer->ShutDown();
}

// アクタ追加・削除処理
void Game::AddActor(Actor* actor)
{
    // アクタ更新中ならPendingに格納
    mUpdatingActors
    ? mPendingActors.emplace_back(actor)
    : mActors.emplace_back(actor);
}
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
