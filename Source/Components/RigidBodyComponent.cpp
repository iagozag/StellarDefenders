//
// Created by Lucas N. Ferreira on 08/09/23.
//

#include <SDL.h>
#include "../Actors/Actor.h"
#include "../Game.h"
#include "RigidBodyComponent.h"
#include "ColliderComponents/AABBColliderComponent.h"
#include <iostream>

const float MAX_SPEED_X = 120.0f;
const float MAX_SPEED_Y = 120.0f;
const float GRAVITY = 2000.0f;

RigidBodyComponent::RigidBodyComponent(class Actor* owner, float mass, float friction, bool applyGravity, int updateOrder):
    Component(owner, updateOrder),
    mApplyGravity(applyGravity),
    mApplyFriction(true),
    mFrictionCoefficient(friction),
    mMass(mass),
    mAngularSpeed(.0f),
    mVelocity(glm::vec2(.0f)),
    mAcceleration(glm::vec2(.0f))
{

}

void RigidBodyComponent::ApplyForce(const glm::vec2 &force) {
    mAcceleration += force * (1.f/mMass);
}

void RigidBodyComponent::Update(float deltaTime)
{
    // Apply gravity acceleration
    if(mApplyGravity) {
        ApplyForce(glm::vec2(.0f, 1.0f) * GRAVITY);
    }

    // Apply friction
    if(mApplyFriction && Math::Abs(mVelocity.x) > 0.05f) {
        ApplyForce(glm::vec2(1.0f, .0f) * -mFrictionCoefficient * mVelocity.x);
    }

    // Euler Integration
    mVelocity += mAcceleration * deltaTime;

    mVelocity.x = Math::Clamp<float>(mVelocity.x, -MAX_SPEED_X, MAX_SPEED_X);
    mVelocity.y = Math::Clamp<float>(mVelocity.y, -MAX_SPEED_Y, MAX_SPEED_Y);

    if(Math::NearZero(mVelocity.x, 0.1f)) {
        mVelocity.x = 0.f;
    }

    auto collider = mOwner->GetComponent<AABBColliderComponent>();

    if(mVelocity.x != 0.0f)
    {
        mOwner->SetPosition(glm::vec2(mOwner->GetPosition().x + mVelocity.x * deltaTime,
                                         mOwner->GetPosition().y));

        if (collider) {
            collider->DetectHorizontalCollision(this);
        }
    }

    if(mVelocity.y != 0.0f)
    {
        mOwner->SetPosition(glm::vec2(mOwner->GetPosition().x,
                                         mOwner->GetPosition().y + mVelocity.y * deltaTime));

        if (collider) {
            collider->DetectVertialCollision(this);
        }

    }

    mAcceleration = glm::vec2(0.f, 0.f);

    float rot = mOwner->GetRotation();

    rot += mAngularSpeed*deltaTime;

    mOwner->SetRotation(rot);
}