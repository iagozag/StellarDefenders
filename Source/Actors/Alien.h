//
// Created by csxuser on 22/06/2025.
//

#ifndef MAINMENUACTOR_H
#define MAINMENUACTOR_H

#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"


class Alien : public Actor{
    public:
        explicit Alien(Game* game, float forwardSpeed = 1500.0f, float jumpSpeed = -750.0f);

        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;

        void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
        void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

        void Kill() override;

    private:
        void ManageAnimations();

        float mForwardSpeed;
        float mJumpSpeed;
        bool mIsRunning;
        bool mIsDead;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent* mDrawComponent;
        class AABBColliderComponent* mColliderComponent;
};



#endif //MAINMENUACTOR_H
