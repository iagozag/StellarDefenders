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
    explicit Ship(Game* game, float height,
                              float forwardForce = 500.0f,
                              float rotationForce = 5.0f,
                              float frictionCoefficient = 0.005f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    ShipState GetShipState(){ return mShipState; }

private:
    float mForwardSpeed;
    float mRotationForce;
    float mFrictionCoefficient;
    float mLaserCooldown;
    float mHeight;

    ShipState mShipState;

    Vector2 mSlingShotPoint;
    Vector2 mDirection;

    class DrawSpriteComponent* mDrawSpriteComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;
};