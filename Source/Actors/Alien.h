//
// Created by csxuser on 22/06/2025.
//

#ifndef ALIEN_H
#define ALIEN_H

#include "Actor.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

class Alien : public Actor{
    public:
        explicit Alien(Game* game, float forwardSpeed = 15.0f, float jumpSpeed = -750.0f);

        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;

        glm::vec2 GetScale(){ return m_size; }

        void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
        void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

        void Kill() override;

    private:
        void ManageAnimations();

        float mForwardSpeed;
        float mJumpSpeed;
        bool mIsRunning;
        bool mIsDead;
        glm::vec2 m_size;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent* mDrawComponent;
        class AABBColliderComponent* mColliderComponent;
};



#endif //MAINMENUACTOR_H
