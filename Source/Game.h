// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include <optional>

#include "AudioSystem.h"
#include "Math.h"
#include "Camera.hpp"

#include "Actors/Ship.h"
#include "Actors/Alien.h"

#include "Scenes/Simulation/simulation.hpp"
#include "Scenes/ShipMenu/ShipMenu.hpp"

class Game
{
public:
    static const int WORLD_WIDTH = 3000;
    static const int WORLD_HEIGHT = 448;
    static const int TRANSITION_TIME = 1;

    enum class GameScene
    {
        MainMenu,
        Ship,
        Level1,
        Level2
    };

    enum class SceneManagerState
    {
        None,
        Entering,
        Active,
        Exiting
    };

    enum class GamePlayState
    {
        Playing,
        Paused,
        GameOver,
        LevelComplete,
        Leaving
    };

    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    void ProcessInputActors();
    void HandleKeyPressActors(const int key, const bool isPressed);

    // Level functions
    void LoadMainMenu();

    std::vector<Actor *> GetNearbyActors(const glm::vec2& position, const int range = 1);
    std::vector<class AABBColliderComponent *> GetNearbyColliders(const glm::vec2& position, const int range = 2);

    void Reinsert(Actor* actor);

    // Audio functions
    AudioSystem* GetAudio();

    // Camera functions
    const Camera &GetCamera();

    // UI functions
    void PushUI(class UIScreen* screen) { mUIStack.emplace_back(screen); }
    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }

    SDL_Renderer* GetRenderer() const { return mRenderer; }

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    // Loading functions
    class UIFont* LoadFont(const std::string& fileName);
    SDL_Texture* LoadTexture(const std::string& texturePath);

    GameScene GetGameScene(){ return mGameScene; }
    void SetGameScene(GameScene scene, float transitionTime = .0f);
    void ResetGameScene(float transitionTime = .0f);
    void UnloadScene();

    void TogglePause();

    void UpdateCamera();

    void SetGamePlayState(GamePlayState state) { mGamePlayState = state; }
    GamePlayState GetGamePlayState() const { return mGamePlayState; }

    void draw_ellipsis(const glm::vec2 &pos, const glm::vec2 &dim, const size_t num_steps = 16);
    glm::ivec2 get_window_dimensions() const;

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Scene Manager
    void UpdateSceneManager(float deltaTime);
    void ChangeScene();
    SceneManagerState mSceneManagerState;
    float mSceneManagerTimer;
    float mSceneManagerAlpha;

    // HUD functions
    void UpdateLevelTime(float deltaTime);

    // Spatial Hashing for collision detection
    class SpatialHashing* mSpatialHashing;

    // All the UI elements
    std::vector<class UIScreen*> mUIStack;
    std::unordered_map<std::string, class UIFont*> mFonts;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    AudioSystem* mAudio;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track actors state
    bool mIsRunning;
    GamePlayState mGamePlayState;

    // Track level state
    GameScene mGameScene;
    GameScene mNextScene;

    Camera m_camera;

    // Game-specific
    class HUD *mHUD;
    SoundHandle mMusicHandle;

    SDL_Rect mViableAreaRect;
    bool mIsViableAreaActive;

    float mGameTimer;
    int mGameTimeLimit;

    class ShipMenu* mShipMenu;
    std::optional<std::unique_ptr<Simulation>> m_current_simulation;
};
