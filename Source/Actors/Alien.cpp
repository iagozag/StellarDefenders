#include "Alien.h"
#include "../norm_greater.hpp"
#include "../UIElements/UIScreen.h"

Alien::Alien(Game* game, const float forwardSpeed):
    Actor(game),
    mForwardSpeed(forwardSpeed),
    mIsRunning(false),
    mLevelSelectPrompt(nullptr)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1,10, false);

    m_size = glm::vec2(1, 1.5);
    mDrawComponent = new DrawAnimatedComponent(this,  "../Assets/Sprites/ET/texture.png", "../Assets/Sprites/ET/texture.json", m_size, 200);

    mDrawComponent->AddAnimation("idle", std::vector<int>({6}));
    mDrawComponent->AddAnimation("run", std::vector<int>({4,0,2,11,8,5}));

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

Alien::~Alien()
{
    if (mLevelSelectPrompt)
    {
        mLevelSelectPrompt->Close();
        mLevelSelectPrompt = nullptr;
    }
}

void Alien::OnProcessInput(const uint8_t* state)
{
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        mRigidBodyComponent->ApplyForce(glm::vec2(mForwardSpeed,0));
        mRotation = 0.0f;
        mIsRunning = true;
    }

    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        mRigidBodyComponent->ApplyForce(glm::vec2(-mForwardSpeed, 0.0f));
        mRotation = Math::Pi;
        mIsRunning = true;
    }
    
    if (!state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
    {
        mIsRunning = false;
    }

    if(mPosition.x > mGame->GetShipMenu()->GetBackgroundSize().x - 3.f){
        if (!mLevelSelectPrompt)
        {
            mLevelSelectPrompt = new UIScreen(mGame, "../Assets/Fonts/SMB.ttf");
            mLevelSelectPrompt->AddText("Pressione de 1 a 3 para selecionar a fase (deve concluir a fase anterior)", glm::vec2(mPosition.x, -0.25f), glm::vec2(2.f, 0.3f));
        }

        int currentLevel = mGame->GetCurrentLevel();
        if(state[SDL_SCANCODE_1]) mGame->SetGameScene(Game::GameScene::Level1, Game::TRANSITION_TIME);
        if(state[SDL_SCANCODE_2] and currentLevel >= 2) mGame->SetGameScene(Game::GameScene::Level2, Game::TRANSITION_TIME);
        if(state[SDL_SCANCODE_3] and currentLevel >= 3) mGame->SetGameScene(Game::GameScene::Level3, Game::TRANSITION_TIME);
    }
    else{
        if (mLevelSelectPrompt)
        {
            mLevelSelectPrompt->Close();
            mLevelSelectPrompt = nullptr;
        }
    }
}

void Alien::OnUpdate(float deltaTime)
{
    mPosition.x = std::max(mPosition.x, -1.f);
    mPosition.x = std::min(mPosition.x, mGame->GetShipMenu()->GetBackgroundSize().x-0.5f-m_size.x);

    ManageAnimations();
}

void Alien::ManageAnimations()
{
    if (mIsRunning) {
        mDrawComponent->SetAnimation("run");
        mDrawComponent->SetAnimFPS(10.0f);
    }
    else mDrawComponent->SetAnimation("idle");

}