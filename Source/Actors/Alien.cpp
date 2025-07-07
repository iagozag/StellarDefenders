#include "Alien.h"
#include "../norm_greater.hpp"

Alien::Alien(Game* game, const float forwardSpeed, const float jumpSpeed):
    Actor(game),
    mForwardSpeed(forwardSpeed),
    mJumpSpeed(jumpSpeed),
    mIsRunning(false),
    mIsDead(false)
{
    mRigidBodyComponent = new RigidBodyComponent(this, 1,10, false);

    // mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f, Game::TILE_SIZE, ColliderLayer::Player);

    m_size = glm::vec2(1, 1.5);
    mDrawComponent = new DrawAnimatedComponent(this,  "../Assets/Sprites/ET/texture.png", "../Assets/Sprites/ET/texture.json", m_size, 200);
    // m_size = norm_greater(m_size);

    mDrawComponent->AddAnimation("idle", std::vector<int>({6}));
    mDrawComponent->AddAnimation("run", std::vector<int>({4,0,2,11,8,5}));

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
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

    if(mPosition.x > mGame->GetShipMenu()->GetBackgroundSize().x - 3.f and state[SDL_SCANCODE_E]){
        mGame->SetGameScene(Game::GameScene::Level1, Game::TRANSITION_TIME);
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
    if (!mIsDead && mIsRunning) {
        mDrawComponent->SetAnimation("run");
        mDrawComponent->SetAnimFPS(10.0f);
    }
    if (!mIsDead && !mIsRunning) mDrawComponent->SetAnimation("idle");

}

void Alien::Kill()
{
    mDrawComponent->SetAnimation("idle");
    mIsDead = true;
    SetState(ActorState::Destroy);
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);

}

void Alien::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) {}

void Alien::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) {}
