//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Ship.h"
#include "Star.h"
#include "../Game.h"
// #include "../Actors/Laser.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/constants.hpp>
#include <glm/gtx/vector_angle.hpp>

Ship::Ship(
    Game* game,
    const float height
):
    Actor(game),
    mHeight(height),
    mShipState(ShipState::Positioning),
    mDrawSpriteComponent(nullptr),
    mRigidBodyComponent(nullptr),
    mCircleColliderComponent(nullptr)
{
    glm::vec2 v1(-mHeight, mHeight/2), v2(mHeight, 0), v3(-mHeight, -mHeight/2);

    std::vector<glm::vec2> vertices = {v1, v2, v3};

    mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Idle.png", mHeight, mHeight);
    mRigidBodyComponent = new RigidBodyComponent(this, 1, 0, false);
    mCircleColliderComponent = new CircleColliderComponent(this, mHeight/2);
}

void Ship::DrawSlingShotLine()
{
    if (mShipState == ShipState::SlingShot or mShipState == ShipState::Ready) {
        SDL_SetRenderDrawColor(mGame->GetRenderer(), 255, 255, 255, 255);
        glm::vec2 pos = mPosition + mHeight/2.0f;
        glm::vec2 v = pos-mSlingShotMousePos;
        glm::vec2 end = pos+v;
        SDL_RenderDrawLine(mGame->GetRenderer(), pos.x, pos.y, end.x, end.y);

        const float headSize = 15.0f;
        const float headAngle = glm::pi<float>() / 6.0f;

        if (glm::length(v) == 0) return;
        
        v = glm::normalize(v);

        glm::vec2 point1 = end - headSize * glm::rotate(v, headAngle);
        glm::vec2 point2 = end - headSize * glm::rotate(v, -headAngle);

        SDL_RenderDrawLine(mGame->GetRenderer(), end.x, end.y, point1.x, point1.y);
        SDL_RenderDrawLine(mGame->GetRenderer(), end.x, end.y, point2.x, point2.y);
    }
}

void Ship::OnProcessInput(const uint8_t* state)
{
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mShipState == ShipState::Positioning and (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))){
        mShipState = ShipState::SlingShot;
        mGame->DisableViableArea();
    }
    else if(mShipState == ShipState::SlingShot){
        mSlingShotMousePos = glm::vec2(mouseX, mouseY);
        if(!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))){
            mShipState = ShipState::Ready;
            mDirection = mPosition-mSlingShotMousePos;
        }
    }
}

void Ship::OnUpdate(float deltaTime)
{
    if(mShipState == ShipState::Positioning){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX,&mouseY);

        SDL_Rect area = mGame->GetViableArea();

        if(mouseX < area.x) mouseX = area.x;
        if(mouseX > area.x+area.w-mHeight) mouseX = area.x+area.w-mHeight;
        if(mouseY < area.y) mouseY = area.y;
        if(mouseY > area.y+area.h-mHeight) mouseY = area.y+area.h-mHeight;

        SetPosition(glm::vec2(mouseX, mouseY));
    }

    for(auto star: GetGame()->GetStars()){
        auto starCollider = star->GetComponent<CircleColliderComponent>();

        if(mCircleColliderComponent->Intersect(*starCollider)){
            // mGame->Quit();
        }
    }
}
