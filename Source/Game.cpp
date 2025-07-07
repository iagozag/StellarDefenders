// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Random.h"
#include "Game.h"
#include "HUD.h"
#include "SpatialHashing.h"
#include "Actors/Actor.h"
#include "UIElements/UIScreen.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include "Components/DrawComponents/DrawAnimatedComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "SpatialHashing.h"
#include "Scenes/Simulation/simulation.hpp"
#include "Scenes/Simulation/target.hpp"
#include <glm/gtc/constants.hpp>
#include "rect_transform.hpp"
#include "filter_vector.hpp"

#include "Scenes/1/one.hpp"
#include "Scenes/2/two.hpp"
#include "Scenes/3/three.hpp"

constexpr glm::u8vec4 BACKGROUND_COLOR = {0, 0, 0, 255};

Game::Game(int windowWidth, int windowHeight):
    mSceneManagerState(SceneManagerState::None),
    mSceneManagerTimer(0.0f),
    mWindow(nullptr),
    mRenderer(nullptr),
    mAudio(nullptr),
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mTicksCount(0),
    mIsRunning(true),
    mGameScene(GameScene::MainMenu),
    mNextScene(GameScene::MainMenu),
    m_camera(),
    mHUD(nullptr),
    mGameTimer(0.0f),
    mGameTimeLimit(0),
    mShipMenu(nullptr),
    mCurrentLevel(1)
{

}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Stellar Defenders", 0, 0, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        SDL_Log("Failed to initialize SDL_mixer");
        return false;
    }

    Random::Init();

    mAudio = new AudioSystem();

    mSpatialHashing = new SpatialHashing(32.0f * 4.0f,
                                         WORLD_WIDTH,
                                         WORLD_HEIGHT);
    mTicksCount = SDL_GetTicks();

    SetGameScene(GameScene::MainMenu);

    return true;
}

void Game::SetGameScene(Game::GameScene scene, float transitionTime)
{
    if (mSceneManagerState == SceneManagerState::None)
    {
        if (scene == GameScene::MainMenu || scene == GameScene::Ship || scene == GameScene::Level1 || scene == GameScene::Level2 || scene == GameScene::Level3)
        {
            mNextScene = scene;
            mSceneManagerState = SceneManagerState::Exiting;
            mSceneManagerTimer = transitionTime;
            mSceneManagerAlpha = 0.0f;
        }
        else
        {
            SDL_Log("Invalid GameScene");
            return;
        }
    }
    else
    {
        SDL_Log("SceneManager is not None");
        return;
    }
}

void Game::ResetGameScene(float transitionTime)
{
    SetGameScene(mGameScene, transitionTime);
}

void Game::ChangeScene()
{
    // Unload current Scene
    UnloadScene();

    // Reset game timer
    mGameTimer = 0.0f;

    // Reset gameplau state
    mGamePlayState = GamePlayState::Playing;

    // Reset scene manager state
    mSpatialHashing = new SpatialHashing(32.0f * 4.0f, WORLD_WIDTH, WORLD_HEIGHT);

    // Scene Manager FSM: using if/else instead of switch
    if (mNextScene == GameScene::MainMenu)
    {
        mMusicHandle = mAudio->PlaySound("mainMenu.mp3", true);
        // Initialize main menu actors
        LoadMainMenu();
    }
    else if (mNextScene == GameScene::Ship)
    {
        mMusicHandle = mAudio->PlaySound("spaceshipAmbient.mp3", true);
        mShipMenu = new ShipMenu(*this);
    }
    else if (mNextScene == GameScene::Level1)
    {
        mMusicHandle = mAudio->PlaySound("level1.mp3", true);
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mHUD->SetLevelName("Fase 1");
        m_current_simulation = std::optional(std::unique_ptr<Simulation>(new Level1(new UIScreen(this, "../Assets/Fonts/SMB.ttf"))));
    }
    else if (mNextScene == GameScene::Level2)
    {
        mMusicHandle = mAudio->PlaySound("level2.mp3", true);
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mHUD->SetLevelName("Fase 2");
        m_current_simulation = std::optional(std::unique_ptr<Simulation>(new Level2(new UIScreen(this, "../Assets/Fonts/SMB.ttf"))));
    }
    else if (mNextScene == GameScene::Level3)
    {
        mMusicHandle = mAudio->PlaySound("level1.mp3", true);
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mHUD->SetLevelName("Fase 3");
        m_current_simulation = std::optional(std::unique_ptr<Simulation>(new Level3(new UIScreen(this, "../Assets/Fonts/SMB.ttf"))));
    }

    // Set new scene
    mGameScene = mNextScene;
}

void Game::LoadMainMenu()
{
    auto mainMenu = new UIScreen(this, "../Assets/Fonts/SMB.ttf");

    const glm::vec2 titleSize = glm::vec2(2.2f, 0.6f);
    const glm::vec2 titlePos = glm::vec2(-1.1f, 0.2);
    mainMenu->AddText("Stellar Defenders", titlePos, titleSize);

    const glm::vec2 button1Pos = glm::vec2(-0.8, -0.4);
    const glm::vec2 button2Pos = glm::vec2(-0.8, -0.8);
    const glm::vec2 buttonSize = glm::vec2(1.6, 0.32);

    mainMenu->AddButton("Play", button1Pos, buttonSize, [this]() {
                                SetGameScene(GameScene::Level3, TRANSITION_TIME);
                            });

    mainMenu->AddButton("Exit", button2Pos, buttonSize, [this]() {
                                Shutdown();
                            });
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
            case SDL_KEYDOWN:
                if (!mUIStack.empty()) {
                    mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                }

                HandleKeyPressActors(event.key.keysym.sym, event.key.repeat == 0);

                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    TogglePause();
                }

                if(event.key.keysym.sym == SDLK_SPACE) {
                    if(m_current_simulation) {
                        auto &sim = m_current_simulation.value();
                        if(sim->is_locked()) {
                            sim->unlock();
                        } else {
                            sim->lock();
                        }
                    }
                }

                break;

            case SDL_MOUSEWHEEL:
                m_camera.m_scale *= glm::pow(0.92f, event.wheel.y);
                break;

            // case SDL_MOUSEBUTTONUP:
            //     {
            //         std::cout << event.button.x << ' ' << event.button.y << '\n';
            //         const auto pos = m_camera.screen_position_to_world_position(*this, glm::vec2(event.button.x, event.button.y));
            //         std::cout << pos.x << ' ' << pos.y << '\n';
            //     }
            //     break;
        }
    }

    ProcessInputActors();
}

void Game::ProcessInputActors()
{
    if(mGamePlayState == GamePlayState::Playing)
    {
        // Get actors on camera
        std::vector<Actor*> actorsOnCamera =
                mSpatialHashing->QueryOnCamera(m_camera, mWindowWidth,mWindowHeight);

        const Uint8* state = SDL_GetKeyboardState(nullptr);

        for (auto actor: actorsOnCamera)
        {
            actor->ProcessInput(state);
        }

        if(m_current_simulation) {
            m_current_simulation.value()->process_input(*this, state);
        }
    }
}

void Game::HandleKeyPressActors(const int key, const bool isPressed)
{
    if(mGamePlayState == GamePlayState::Playing)
    {
        // Get actors on camera
        std::vector<Actor*> actorsOnCamera =
                mSpatialHashing->QueryOnCamera(m_camera, mWindowWidth,mWindowHeight);

        for (auto actor: actorsOnCamera) {
            actor->HandleKeyPress(key, isPressed);
        }
    }
}

void Game::TogglePause()
{
    if (mGameScene != GameScene::MainMenu)
    {
        if (mGamePlayState == GamePlayState::Playing)
        {
            mGamePlayState = GamePlayState::Paused;
            mAudio->PauseSound(mMusicHandle);
            mAudio->PlaySound("Coin.wav");
        }
        else if (mGamePlayState == GamePlayState::Paused)
        {
            mGamePlayState = GamePlayState::Playing;
            mAudio->ResumeSound(mMusicHandle);
            mAudio->PlaySound("Coin.wav");
        }
    }
}

void Game::UpdateGame() {
    float delta_t = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();


    if(mGamePlayState != GamePlayState::Paused && mGamePlayState != GamePlayState::GameOver)
    {
        // Reinsert all actors and pending actors
        UpdateActors(delta_t);
    }

    UpdateCamera();

    // Reinsert audio system
    mAudio->Update(delta_t);

    // Reinsert UI screens
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::UIState::Active) {
            ui->Update(delta_t);
        }
    }

    for(auto &el : mUIStack) {
        if(el->GetState() == UIScreen::UIState::Closing) {
            delete el;
            el = nullptr;
        }
    }

    const auto predicate = [](const UIScreen *el) {
        return !!el;
    };
    filter<UIScreen *>(mUIStack, predicate);

    UpdateSceneManager(delta_t);

    if(m_current_simulation) {
        m_current_simulation.value()->run(*this, delta_t);
    }
}

void Game::UpdateCamera(){
    if(mGameScene == GameScene::MainMenu) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        m_camera.m_pos.x -= state[SDL_SCANCODE_A] * m_camera.m_scale / 100;
        m_camera.m_pos.x += state[SDL_SCANCODE_D] * m_camera.m_scale / 100;
        m_camera.m_pos.y -= state[SDL_SCANCODE_S] * m_camera.m_scale / 100;
        m_camera.m_pos.y += state[SDL_SCANCODE_W] * m_camera.m_scale / 100;
    }
    else if(mGameScene == GameScene::Ship and mShipMenu and mShipMenu->GetAlien()){
        const glm::vec2& alienPos = mShipMenu->GetAlien()->GetPosition();
        m_camera.m_pos.x = alienPos.x;
    }
    else m_camera.m_pos = glm::vec2(.0f);
}

void Game::UpdateSceneManager(float deltaTime)
{
    if (mSceneManagerState == SceneManagerState::Exiting)
    {
        mSceneManagerTimer -= deltaTime;
        mSceneManagerAlpha = 1.0f - (mSceneManagerTimer / TRANSITION_TIME);

        if (mSceneManagerTimer <= 0.0f)
        {
            ChangeScene();
            mSceneManagerState = SceneManagerState::Entering;
            mSceneManagerTimer = TRANSITION_TIME;
            mSceneManagerAlpha = 1.0f;
        }
    }
    else if (mSceneManagerState == SceneManagerState::Entering)
    {
        mSceneManagerTimer -= deltaTime;
        mSceneManagerAlpha = (mSceneManagerTimer / TRANSITION_TIME);

        if (mSceneManagerTimer <= 0.0f)
        {
            mSceneManagerState = SceneManagerState::None;
            mSceneManagerAlpha = 0.0f;
        }
    }
}

void Game::UpdateActors(float deltaTime)
{
    // Get actors on camera
    std::vector<Actor*> actorsOnCamera =
            mSpatialHashing->QueryOnCamera(m_camera, mWindowWidth, mWindowHeight);

    for (auto actor : actorsOnCamera)
    {
        actor->Update(deltaTime);
    }

    for (auto actor : actorsOnCamera)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            delete actor;
        }
    }
}

void Game::AddActor(Actor* actor)
{
    mSpatialHashing->Insert(actor);
}

void Game::RemoveActor(Actor* actor)
{
    mSpatialHashing->Remove(actor);
}
void Game::Reinsert(Actor* actor)
{
    mSpatialHashing->Reinsert(actor);
}

std::vector<Actor *> Game::GetNearbyActors(const glm::vec2& position, const int range)
{
    return mSpatialHashing->Query(position, range);
}

std::vector<AABBColliderComponent *> Game::GetNearbyColliders(const glm::vec2& position, const int range)
{
    return mSpatialHashing->QueryColliders(position, range);
}

void Game::draw_ellipsis(const glm::vec2 &pos, const glm::vec2 &dim, const glm::u8vec4 &color, const size_t num_steps) {
    const auto transform = m_camera.get_total_transformation_matrix(*this);

    // Uma elipse preenchida pode ser construída como um triângulo fan
    // onde todos os triângulos compartilham o centro da elipse como um vértice.
    // O número de vértices será num_steps + 2 (centro + num_steps na borda + um extra para fechar o loop).
    std::vector<SDL_Vertex> vertices;
    vertices.reserve(num_steps + 2);

    // Vértice central
    SDL_Vertex center_vertex;
    center_vertex.position.x = pos.x;
    center_vertex.position.y = pos.y;
    center_vertex.color.r = color.r;
    center_vertex.color.g = color.g;
    center_vertex.color.b = color.b;
    center_vertex.color.a = color.a;
    vertices.push_back(center_vertex);

    // Gera os vértices da borda da elipse
    for (size_t i = 0; i <= num_steps; ++i) {
        float angle = static_cast<float>(i) / static_cast<float>(num_steps) * glm::two_pi<float>();

        SDL_Vertex border_vertex;
        border_vertex.position.x = pos.x + dim.x * 0.5f * glm::cos(angle);
        border_vertex.position.y = pos.y + dim.y * 0.5f * glm::sin(angle);
        border_vertex.color = center_vertex.color;
        vertices.push_back(border_vertex);
    }

    // Geralmente isso é feito no vertex shader, mas como não temos tempo
    // pra brincar com OpenGl, vai ser no CPU mesmo.
    for(auto &vertice : vertices) {
        const auto transform_point = glm::vec4(vertice.position.x, vertice.position.y, 0, 1);
        const auto transformed = transform * transform_point;
        vertice.position.x = transformed.x;
        vertice.position.y = transformed.y;
    }

    // SDL_RenderGeometry desenha triângulos usando um array de vértices.
    // Para um triângulo fan, os índices de elementos são:
    // (0, 1, 2), (0, 2, 3), (0, 3, 4), ... (0, num_steps, 1)
    // O vértice 0 é o centro.
    // O último triângulo conecta o vértice central, o último ponto gerado (num_steps) e o primeiro ponto gerado (1).

    // Gerar os índices para o triângulo fan
    std::vector<int> indices;
    indices.reserve(num_steps * 3); // Cada triângulo tem 3 vértices

    for (size_t i = 0; i < num_steps; ++i) {
        indices.push_back(0);        // Vértice central
        indices.push_back(i + 1);    // Vértice atual na borda
        indices.push_back(i + 2 > num_steps + 1 ? 1 : i + 2); // Próximo vértice na borda (volta para o 1º no final)
    }

    // Para simplificar, SDL_RenderGeometry pode desenhar um "strip" ou "fan"
    // sem a necessidade de gerar os índices manualmente se os vértices estiverem na ordem correta.
    // No entanto, para um triângulo fan, o método de enviar vértices e índices explícitos é mais robusto.

    // Desenha a elipse preenchida
    // O segundo parâmetro `texture` é NULL para desenhar apenas com cor.
    // O quarto parâmetro `indices` é o vetor de índices.
    // O quinto parâmetro `num_indices` é o número de índices no vetor.
    SDL_RenderGeometry(mRenderer, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(), static_cast<int>(indices.size()));
}

glm::ivec2 Game::get_window_dimensions() const {
    return {mWindowWidth, mWindowHeight};
}

void Game::GenerateOutput() {
    // Clear frame with background color
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    if(mGameScene == GameScene::Ship)
        mShipMenu->draw(*this);

    // Get actors on camera
    std::vector<Actor*> actorsOnCamera =
            mSpatialHashing->QueryOnCamera(m_camera, mWindowWidth, mWindowHeight);

    // Get list of drawables in draw order
    std::vector<DrawComponent*> drawables;

    for (auto actor : actorsOnCamera)
    {
        auto drawable = actor->GetComponent<DrawComponent>();
        if (drawable && drawable->IsVisible())
        {
            drawables.emplace_back(drawable);
        }
    }

    // Sort drawables by draw order
    std::sort(drawables.begin(), drawables.end(),
                [](const DrawComponent* a, const DrawComponent* b) {
                    return a->GetDrawOrder() < b->GetDrawOrder();
                });

    // Draw all drawables
    for (auto drawable : drawables)
    {
        drawable->Draw(mRenderer);
    }

    // Draw all UI screens
    for (auto ui :mUIStack)
    {
        ui->Draw(*this);
    }

    // TODO 1.: Verifique se o SceneManager está no estado ativo. Se estiver, desenhe um retângulo preto cobrindo
    //  toda a tela.
    if (mSceneManagerState == SceneManagerState::Exiting || mSceneManagerState == SceneManagerState::Entering)
    {
        int alpha = static_cast<int>(mSceneManagerAlpha * 255.0f);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, alpha);
        SDL_Rect fullscreenRect = {0, 0, mWindowWidth, mWindowHeight};
        SDL_RenderFillRect(mRenderer, &fullscreenRect);
    }

    if(m_current_simulation) {
        m_current_simulation.value()->draw(*this);
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::LoadTexture(const std::string& texturePath)
{
    SDL_Surface* surface = IMG_Load(texturePath.c_str());

    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }

    return texture;
}


UIFont* Game::LoadFont(const std::string& fileName)
{
    if(mFonts.count(fileName)) return mFonts[fileName];

    UIFont* font = new UIFont(mRenderer);
    if(font->Load(fileName)) return mFonts[fileName] = font;

    font->Unload();
    delete font;
    return nullptr;
}

void Game::UnloadScene()
{
    // Pause current music
    if (mAudio && mMusicHandle.IsValid())
    {
        mAudio->PauseSound(mMusicHandle);
        mMusicHandle.Reset();
    }

    // Delete actors
    delete mSpatialHashing;
    mSpatialHashing = nullptr;

    // Delete UI screens
    for (auto ui : mUIStack) {
        delete ui;
    }
    mUIStack.clear();
}

void Game::Shutdown()
{
    UnloadScene();

    for (auto font : mFonts) {
        font.second->Unload();
        delete font.second;
    }
    mFonts.clear();

    delete mAudio;
    mAudio = nullptr;

   // Mix_CloseAudio();

   // Mix_Quit();
    TTF_Quit();
    IMG_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

AudioSystem* Game::GetAudio() {
    return mAudio;
}

const Camera &Game::GetCamera() const {
    return m_camera;
}

Camera &Game::GetCamera() {
    return m_camera;
}