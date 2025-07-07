#ifndef ALIEN_H
#define ALIEN_H

#include "Actor.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

class UIScreen;

class Alien : public Actor{
    public:
        explicit Alien(Game* game, float forwardSpeed = 80.0f);
        ~Alien();

        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;

        glm::vec2 GetScale(){ return m_size; }

    private:
        void ManageAnimations();

        float mForwardSpeed;
        bool mIsRunning;
        glm::vec2 m_size;

        class UIScreen* mLevelSelectPrompt;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent* mDrawComponent;
};



#endif
