#ifndef MAINMENUACTOR_H
#define MAINMENUACTOR_H

#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"


class EmergencyLight : public Actor{
    public:
        explicit EmergencyLight(Game* game);
        void OnUpdate(float deltaTime) override;

    private:
        void ManageAnimations();

        class DrawAnimatedComponent* mDrawComponent;
};



#endif //MAINMENUACTOR_H
