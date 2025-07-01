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
#include "CSV.h"
#include "Random.h"
#include "Game.h"
#include "HUD.h"
#include "SpatialHashing.h"
#include "Actors/Actor.h"
#include "UIElements/UIScreen.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include "Components/DrawComponents/DrawAnimatedComponent.h"
#include "Components/DrawComponents/DrawPolygonComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "SpatialHashing.h"
#include "Scenes/Simulation/simulation.hpp"
#include "Scenes/Simulation/target.hpp"
#include <glm/gtc/constants.hpp>

const Planet planets[] = {
    Planet({-0.5, -0.5}, {0.2, -0.2}, 0.025, 0.01),
    Planet({ 0.5,  0.5}, {-0.2, 0.2}, 0.025, 0.01),
    Planet({ 0,  0}, {0, 0}, 0.06, 0.1),
};

const Kamikaze kamikaze[] = {
    Kamikaze({ 0.5, -0.5}, {0.2, 0.2}),
    Kamikaze({-0.5,  0.5}, {-0.22, -0.22})
};

const Target targets[] = {
    Target({0.2, 0}, {0.85, 0})
};

#define ARR_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

constexpr auto NUM_PLANETS = ARR_LEN(planets);
constexpr auto NUM_KAMIKAZE = ARR_LEN(kamikaze);
constexpr auto NUM_TARGETS = ARR_LEN(targets);

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
    mModColor(255, 255, 255),
    mHUD(nullptr),
    mIsViableAreaActive(false),
    mGameTimer(0.0f),
    mGameTimeLimit(0),
    mBackgroundTexture(nullptr),
    mBackgroundSize(glm::vec2(.0f)),
    mBackgroundPosition(glm::vec2(.0f)),
    m_simulation(
        std::vector(planets, &planets[NUM_PLANETS]),
        std::vector(kamikaze, &kamikaze[NUM_KAMIKAZE]),
        std::vector(targets, &targets[NUM_TARGETS])
    )
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

    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // Initialize SDL_mixer
   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        SDL_Log("Failed to initialize SDL_mixer");
        return false;
    }

    // Start random number generator
    Random::Init();

    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 1. Instancie um AudioSystem.
    mAudio = new AudioSystem();


    mSpatialHashing = new SpatialHashing(TILE_SIZE * 4.0f,
                                         WORLD_WIDTH,
                                         WORLD_HEIGHT);
    mTicksCount = SDL_GetTicks();

    // Init all game actors
    SetGameScene(GameScene::MainMenu);

    return true;
}

void Game::SetGameScene(Game::GameScene scene, float transitionTime)
{
    if (mSceneManagerState == SceneManagerState::None)
    {
        if (scene == GameScene::MainMenu || scene == GameScene::Ship || scene == GameScene::Level1 || scene == GameScene::Level2)
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
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.: Chame SetGameScene passando o mGameScene atual e o tempo de transição.
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
    mSpatialHashing = new SpatialHashing(TILE_SIZE * 4.0f, WORLD_WIDTH, WORLD_HEIGHT);

    // Scene Manager FSM: using if/else instead of switch
    if (mNextScene == GameScene::MainMenu)
    {
        // Initialize main menu actors
        LoadMainMenu();
    }
    else if (mNextScene == GameScene::Ship)
    {
        if(!mAlien) mAlien = new Alien(this);
        mAlien->SetPosition(glm::vec2(0, mWindowHeight-TILE_SIZE));

        SetBackgroundImage("../Assets/Sprites/background.png", glm::vec2(0,0), glm::vec2(WORLD_WIDTH,WORLD_HEIGHT));
    }
    else if (mNextScene == GameScene::Level1)
    {
        mShips.emplace_back(new Ship(this, 50, "corvette.png"));
        // --------------
        // TODO - PARTE 3
        // --------------

        // TODO 1.: Crie um novo objeto HUD, passando o ponteiro do Game e o caminho para a fonte SMB.ttf.
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
        mHUD->SetLevelName("Fase 1");

        // Set Viable Area for ships
        SDL_Rect viableArea = {0, 0, mWindowWidth/2, mWindowHeight};
        SetViableArea(viableArea);

        // --------------
        // TODO - PARTE 4
        // --------------

        // TODO 1. Toque a música de fundo "MusicMain.ogg" em loop e armaze o SoundHandle retornado em mMusicHandle.

        // Set background color
        //SetBackgroundImage("../Assets/Sprites/background.png", glm::vec2(TILE_SIZE,0), glm::vec2(6784,WORLD_HEIGHT));

        // Initialize actors
        // LoadLevel("../Assets/Levels/level1-1.csv", LEVEL_WIDTH, LEVEL_HEIGHT);
    }
    else if (mNextScene == GameScene::Level2)
    {
        // --------------
        // TODO - PARTE 3
        // --------------

        // TODO 1.: Crie um novo objeto HUD, passando o ponteiro do Game e o caminho para a fonte SMB.ttf. Como
        //  feito no nível 1-1.
        mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");

        // TODO 2.: Altere o atributo mGameTimeLimit para 400 (400 segundos) e ajuste o HUD com esse tempo inicial. Como
        //  feito no nível 1-1.
        mHUD->SetLevelName("Fase 2");

        // --------------
        // TODO - PARTE 4
        // --------------

        // TODO 1. Toque a música de fundo "MusicUnderground.ogg" em loop e armaze o SoundHandle retornado em mMusicHandle.
        // mMusicHandle = mAudio->PlaySound("MusicUnderground.ogg", 1);


        // Initialize actors
        // LoadLevel("../Assets/Levels/level1-2.csv", LEVEL_WIDTH, LEVEL_HEIGHT);
    }

    // Set new scene
    mGameScene = mNextScene;
}

void Game::SetViableArea(const SDL_Rect& rect)
{
    mViableAreaRect = rect;
    mIsViableAreaActive = true;
}

void Game::DisableViableArea()
{
    mIsViableAreaActive = false;
}

void Game::LoadMainMenu()
{

    // SetBackgroundImage("../Assets/Sprites/Background.png", glm::vec2(TILE_SIZE,-TILE_SIZE), glm::vec2(6784,WORLD_HEIGHT));

    // --------------
    // TODO - PARTE 1
    // --------------

    // Esse método será usado para criar uma tela de UI e adicionar os elementos do menu principal.
    auto mainMenu = new UIScreen(this, "../Assets/Fonts/SMB.ttf");

    const glm::vec2 titleSize = glm::vec2(500.0f, 88.0f);
    const glm::vec2 titlePos = glm::vec2(mWindowWidth/2.0f - titleSize.x/2.0f, 50.0f);
    mainMenu->AddText("Stellar Defenders", titlePos, titleSize);

    const glm::vec2 button1Pos = glm::vec2(0, titlePos.y)+glm::vec2(mWindowWidth/2.0f - 100.0f, 200.0f);
    const glm::vec2 button2Pos = glm::vec2(0, titlePos.y)+glm::vec2(mWindowWidth/2.0f - 100.0f, 250.0f);
    const glm::vec2 buttonSize = glm::vec2(200.0f, 40.0f);

    mainMenu->AddButton("Play", button1Pos, buttonSize, [this]() {
                                SetGameScene(GameScene::Level1, TRANSITION_TIME);
                            });

    mainMenu->AddButton("Exit", button2Pos, buttonSize, [this]() {
                                Shutdown();
                            });
}

void Game::LoadLevel(const std::string& levelName, const int levelWidth, const int levelHeight)
{
    // Load level data
    int **mLevelData = ReadLevelData(levelName, levelWidth, levelHeight);

    if (!mLevelData) {
        SDL_Log("Failed to load level data");
        return;
    }

    // Instantiate level actors
    BuildLevel(mLevelData, levelWidth, levelHeight);
}

void Game::BuildLevel(int** levelData, int width, int height) {}

int **Game::ReadLevelData(const std::string& fileName, int width, int height)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("Failed to load paths: %s", fileName.c_str());
        return nullptr;
    }

    // Create a 2D array of size width and height to store the level data
    int** levelData = new int*[height];
    for (int i = 0; i < height; ++i)
    {
        levelData[i] = new int[width];
    }

    // Read the file line by line
    int row = 0;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        if(!line.empty())
        {
            auto tiles = CSVHelper::Split(line);

            if (tiles.size() != width) {
                SDL_Log("Invalid level data");
                return nullptr;
            }

            for (int i = 0; i < width; ++i) {
                levelData[row][i] = tiles[i];
            }
        }

        ++row;
    }

    // Close the file
    file.close();

    return levelData;
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

                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    TogglePause();
                }
                break;

            case SDL_MOUSEWHEEL:
                m_camera.m_scale *= glm::pow(0.92f, event.wheel.y);
                break;
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

            // --------------
            // TODO - PARTE 4
            // --------------

            // TODO 1.: Pare a música de fundo atual usando PauseSound() e toque o som "Coin.wav" para indicar a pausa.
            // mAudio->PauseSound(mMusicHandle);
            // mAudio->PlaySound("Coin.wav");
        }
        else if (mGamePlayState == GamePlayState::Paused)
        {
            mGamePlayState = GamePlayState::Playing;

            // --------------
            // TODO - PARTE 4
            // --------------

            // TODO 1.: Retome a música de fundo atual usando ResumeSound() e toque o som "Coin.wav" para
            //  indicar a retomada do jogo.
            // mAudio->ResumeSound(mMusicHandle);
            // mAudio->PlaySound("Coin.wav");
        }
    }
}

void Game::UpdateGame() {
    float delta_t = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();
    
    m_simulation.run(*this, delta_t);



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

    // Delete any UIElements that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::UIState::Closing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.: Chame UpdateSceneManager passando o deltaTime.
    UpdateSceneManager(delta_t);
}

void Game::UpdateCamera(){
    if(mGameScene == GameScene::MainMenu) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        m_camera.m_pos.x -= state[SDL_SCANCODE_A] * m_camera.m_scale / 100;
        m_camera.m_pos.x += state[SDL_SCANCODE_D] * m_camera.m_scale / 100;
        m_camera.m_pos.y -= state[SDL_SCANCODE_W] * m_camera.m_scale / 100;
        m_camera.m_pos.y += state[SDL_SCANCODE_S] * m_camera.m_scale / 100;

    } else if (mGameScene != GameScene::Ship) {
        m_camera.m_pos = glm::vec2(.0f);
    } else {
        float horizontalCameraPos = mAlien->GetPosition().x - (mWindowWidth / 2.0f);

        float maxCameraPos = WORLD_WIDTH - mWindowWidth;
        horizontalCameraPos = Math::Clamp(horizontalCameraPos, 0.0f, maxCameraPos);

        m_camera.m_pos.x = horizontalCameraPos;
    }
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

void Game::draw_ellipsis(const glm::vec2 &pos, const glm::vec2 &dim, const size_t num_steps) {
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
    center_vertex.color.r = 255; // Cor vermelha por padrão, você pode parametrizar
    center_vertex.color.g = 0;
    center_vertex.color.b = 0;
    center_vertex.color.a = 255;
    vertices.push_back(center_vertex);

    // Gera os vértices da borda da elipse
    for (size_t i = 0; i <= num_steps; ++i) {
        float angle = static_cast<float>(i) / static_cast<float>(num_steps) * glm::two_pi<float>();

        SDL_Vertex border_vertex;
        border_vertex.position.x = pos.x + dim.x * 0.5f * glm::cos(angle);
        border_vertex.position.y = pos.y + dim.y * 0.5f * glm::sin(angle);
        border_vertex.color = {0, Uint8((i & 1) * 255), Uint8((1 - (i & 1)) * 255), 255}; // Usa a mesma cor do centro
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

    m_simulation.draw(*this);



    if (mBackgroundTexture)
    {
        // Cria um retângulo de destino na tela com a posição e o tamanho do fundo
        SDL_Rect destRect;
        destRect.x = static_cast<int>(mBackgroundPosition.x-m_camera.m_pos.x);
        destRect.y = static_cast<int>(mBackgroundPosition.y);
        destRect.w = static_cast<int>(mBackgroundSize.x);
        destRect.h = static_cast<int>(mBackgroundSize.y);

        // Copia a textura de fundo para o renderizador na posição/tamanho especificados
        SDL_RenderCopy(mRenderer, mBackgroundTexture, nullptr, &destRect);
    }

    // Draw viable area
    if (mGameScene != GameScene::MainMenu and mGameScene != GameScene::Ship and mIsViableAreaActive)
    {
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer, 135, 206, 250, 64);
        SDL_RenderFillRect(mRenderer, &mViableAreaRect);

        SDL_Rect inviableAreaRect = {mViableAreaRect.x+mViableAreaRect.w, 0, mWindowWidth-(mViableAreaRect.x+mViableAreaRect.w), mWindowHeight};
        SDL_SetRenderDrawColor(mRenderer, 255, 100, 100, 64);
        SDL_RenderFillRect(mRenderer, &inviableAreaRect);
    }

    if (mShips.size()){
        for(auto ship: mShips) ship->DrawSlingShotLine();
    }

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
        drawable->Draw(mRenderer, mModColor);
    }

    // Draw all UI screens
    for (auto ui :mUIStack)
    {
        ui->Draw(mRenderer);
    }

    // --------------
    // TODO - PARTE 2
    // --------------

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




    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

void Game::SetBackgroundImage(const std::string& texturePath, const glm::vec2 &position, const glm::vec2 &size)
{
    if (mBackgroundTexture) {
        SDL_DestroyTexture(mBackgroundTexture);
        mBackgroundTexture = nullptr;
    }

    // Load background texture
    mBackgroundTexture = LoadTexture(texturePath);
    if (!mBackgroundTexture) {
        SDL_Log("Failed to load background texture: %s", texturePath.c_str());
    }

    // Set background position
    mBackgroundPosition = position;

    // Set background size
    mBackgroundSize = size;
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
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Verifique se o arquivo de fonte já está carregado no mapa mFonts.
    //  Se sim, retorne o ponteiro para a fonte carregada.
    //  Se não, crie um novo objeto UIFont, carregue a fonte do arquivo usando o método Load,
    //  e se o carregamento for bem-sucedido, adicione a fonte ao mapa mFonts.
    //  Se o carregamento falhar, descarregue a fonte com Unload e delete o objeto UIFont, retornando nullptr.
    if(mFonts.count(fileName)) return mFonts[fileName];

    UIFont* font = new UIFont(mRenderer);
    if(font->Load(fileName)) return mFonts[fileName] = font;

    font->Unload();
    delete font;
    return nullptr;
}

void Game::UnloadScene()
{
    // Delete actors
    delete mSpatialHashing;
    mSpatialHashing = nullptr;

    // Delete UI screens
    for (auto ui : mUIStack) {
        delete ui;
    }
    mUIStack.clear();

    // Delete background texture
    if (mBackgroundTexture) {
        SDL_DestroyTexture(mBackgroundTexture);
        mBackgroundTexture = nullptr;
    }
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
