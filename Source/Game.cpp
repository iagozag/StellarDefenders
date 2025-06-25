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

Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mShip(nullptr)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mHUD(nullptr)
        ,mBackgroundColor(0, 0, 0)
        ,mModColor(255, 255, 255)
        ,mCameraPos(Vector2::Zero)
        ,mAudio(nullptr)
        ,mGameTimer(0.0f)
        ,mGameTimeLimit(0)
        ,mSceneManagerTimer(0.0f)
        ,mSceneManagerState(SceneManagerState::None)
        ,mGameScene(GameScene::MainMenu)
        ,mNextScene(GameScene::MainMenu)
        ,mBackgroundTexture(nullptr)
        ,mBackgroundSize(Vector2::Zero)
        ,mBackgroundPosition(Vector2::Zero)
        ,mIsViableAreaActive(false)
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
                                         LEVEL_WIDTH * TILE_SIZE,
                                         LEVEL_HEIGHT * TILE_SIZE);
    mTicksCount = SDL_GetTicks();

    // Init all game actors
    SetGameScene(GameScene::MainMenu);

    return true;
}

void Game::AddStar(Star* star)
{
    mStars.emplace_back(star);
}

void Game::RemoveStar(Star* star)
{
    auto it = std::find(mStars.begin(), mStars.end(), star);
    if(it != mStars.end()) mStars.erase(it);
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

    // Reset camera position
    mCameraPos.Set(0.0f, 0.0f);

    // Reset game timer
    mGameTimer = 0.0f;

    // Reset gameplau state
    mGamePlayState = GamePlayState::Playing;

    // Reset scene manager state
    mSpatialHashing = new SpatialHashing(TILE_SIZE * 4.0f, LEVEL_WIDTH * TILE_SIZE, LEVEL_HEIGHT * TILE_SIZE);

    // Scene Manager FSM: using if/else instead of switch
    if (mNextScene == GameScene::MainMenu)
    {
        // Initialize main menu actors
        LoadMainMenu();
    }
    else if (mNextScene == GameScene::Ship)
    {
        mAlien = new Alien(this);
        mAlien->SetPosition(Vector2(0, mWindowHeight-TILE_SIZE));

        SetBackgroundImage("../Assets/Sprites/background.png", Vector2(0,0), Vector2(3000,448));
    }
    else if (mNextScene == GameScene::Level1)
    {
        mShip = new Ship(this, 50);
        mShip->SetPosition(Vector2(0, 0));
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
        mBackgroundColor.Set(0.0f, 0.0f, 0.0f);

        // Set background color
        //SetBackgroundImage("../Assets/Sprites/background.png", Vector2(TILE_SIZE,0), Vector2(6784,448));

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


        // Set background color
        mBackgroundColor.Set(0.0f, 0.0f, 0.0f);

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
    // Set background color
    mBackgroundColor.Set(0.0f, 0.0f, 0.0f);

    // SetBackgroundImage("../Assets/Sprites/Background.png", Vector2(TILE_SIZE,-TILE_SIZE), Vector2(6784,448));

    // --------------
    // TODO - PARTE 1
    // --------------

    // Esse método será usado para criar uma tela de UI e adicionar os elementos do menu principal.
    auto mainMenu = new UIScreen(this, "../Assets/Fonts/SMB.ttf");

    const Vector2 titleSize = Vector2(500.0f, 88.0f);
    const Vector2 titlePos = Vector2(mWindowWidth/2.0f - titleSize.x/2.0f, 50.0f);
    mainMenu->AddText("Stellar Defenders", titlePos, titleSize);

    const Vector2 button1Pos = Vector2(0, titlePos.y)+Vector2(mWindowWidth/2.0f - 100.0f, 200.0f);
    const Vector2 button2Pos = Vector2(0, titlePos.y)+Vector2(mWindowWidth/2.0f - 100.0f, 250.0f);
    const Vector2 buttonSize = Vector2(200.0f, 40.0f);

    auto button1 = mainMenu->AddButton("Play", button1Pos, buttonSize, [this]() {
                                SetGameScene(GameScene::Ship, TRANSITION_TIME);
                            });

    auto button2 = mainMenu->AddButton("Exit", button2Pos, buttonSize, [this]() {
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

void Game::BuildLevel(int** levelData, int width, int height)
{

    // Const map to convert tile ID to block type
    const std::map<int, const std::string> tileMap = {
    };

    for (int y = 0; y < LEVEL_HEIGHT; ++y)
    {
        for (int x = 0; x < LEVEL_WIDTH; ++x)
        {
            int tile = levelData[y][x];


        }
    }
}

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
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
            case SDL_KEYDOWN:
                // Handle key press for UI screens
                if (!mUIStack.empty()) {
                    mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                }

                HandleKeyPressActors(event.key.keysym.sym, event.key.repeat == 0);

                // Check if the Return key has been pressed to pause/unpause the game
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    TogglePause();
                }
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
                mSpatialHashing->QueryOnCamera(mCameraPos,mWindowWidth,mWindowHeight);

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
                mSpatialHashing->QueryOnCamera(mCameraPos,mWindowWidth,mWindowHeight);

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

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if(mGamePlayState != GamePlayState::Paused && mGamePlayState != GamePlayState::GameOver)
    {
        // Reinsert all actors and pending actors
        UpdateActors(deltaTime);
    }

    // Reinsert audio system
    mAudio->Update(deltaTime);

    // Reinsert UI screens
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::UIState::Active) {
            ui->Update(deltaTime);
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

    // ---------------------
    // Game Specific Updates
    // ---------------------
    if (mGameScene == GameScene::Ship) UpdateCamera();

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 1.: Chame UpdateSceneManager passando o deltaTime.
    UpdateSceneManager(deltaTime);
}

void Game::UpdateCamera()
{
    Vector2 posJog = mAlien->GetPosition();
    Vector2 posCam =  GetCameraPos();

    posJog.x -= mWindowWidth / 2;

    mCameraPos.x = posJog.x;
    if (mCameraPos.x < 0) mCameraPos.x = 0;
    if (mCameraPos.x > 3000-mWindowWidth) mCameraPos.x = 3000-mWindowWidth;
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
            mSpatialHashing->QueryOnCamera(mCameraPos,mWindowWidth,mWindowHeight);

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

std::vector<Actor *> Game::GetNearbyActors(const Vector2& position, const int range)
{
    return mSpatialHashing->Query(position, range);
}

std::vector<AABBColliderComponent *> Game::GetNearbyColliders(const Vector2& position, const int range)
{
    return mSpatialHashing->QueryColliders(position, range);
}

void Game::GenerateOutput()
{
    // Clear frame with background color
    SDL_SetRenderDrawColor(mRenderer, mBackgroundColor.x, mBackgroundColor.y, mBackgroundColor.z, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draw background texture considering camera position
    if (mBackgroundTexture)
    {
        SDL_Rect dstRect = { static_cast<int>(mBackgroundPosition.x - mCameraPos.x),
                             static_cast<int>(mBackgroundPosition.y - mCameraPos.y),
                             static_cast<int>(mBackgroundSize.x),
                             static_cast<int>(mBackgroundSize.y) };

        SDL_RenderCopy(mRenderer, mBackgroundTexture, nullptr, &dstRect);
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

    // Get actors on camera
    std::vector<Actor*> actorsOnCamera =
            mSpatialHashing->QueryOnCamera(mCameraPos,mWindowWidth,mWindowHeight);

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

void Game::SetBackgroundImage(const std::string& texturePath, const Vector2 &position, const Vector2 &size)
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
    mBackgroundPosition.Set(position.x, position.y);

    // Set background size
    mBackgroundSize.Set(size.x, size.y);
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
