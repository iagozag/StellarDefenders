//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

enum class ShipState
{
    Positioning,
    SlingShot,
    Ready,
    Simulating
};

class Ship : public Actor
{
public:
    explicit Ship(Game* game, float height);

    void DrawSlingShotLine();
    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    ShipState GetShipState(){ return mShipState; }

private:
    float mHeight;

    ShipState mShipState;

    glm::vec2 mSlingShotMousePos;
    glm::vec2 mDirection;

    class DrawSpriteComponent* mDrawSpriteComponent;
};