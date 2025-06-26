//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Component.h"
#include "../Math.h"

class RigidBodyComponent : public Component
{
public:
    RigidBodyComponent(class Actor* owner, float mass = 1.0f, float friction = 0.0f,
                        bool applyGravity = true, int updateOrder = 10);

    void Update(float deltaTime) override;

    const glm::vec2& GetVelocity() const { return mVelocity; }
    void SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }

    const glm::vec2& GetAcceleration() const { return mAcceleration; }
    void SetAcceleration(const glm::vec2& acceleration) { mAcceleration = acceleration; }

    void SetApplyGravity(const bool applyGravity) { mApplyGravity = applyGravity; }
    void SetApplyFriction(const bool applyFriction) { mApplyFriction = applyFriction;  }

    void SetAngularSpeed(const float speed) { mAngularSpeed = speed; }
    float GetAngularSpeed() const { return mAngularSpeed; }

    void ApplyForce(const glm::vec2 &force);

private:
    bool mApplyGravity;
    bool mApplyFriction;

    // Physical properties
    float mFrictionCoefficient;
    float mMass;
    float mAngularSpeed;

    glm::vec2 mVelocity;
    glm::vec2 mAcceleration;
};
