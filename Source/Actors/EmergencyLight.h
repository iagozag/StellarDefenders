#ifndef EMERGENCYLIGHT_H
#define EMERGENCYLIGHT_H

#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

class EmergencyLight : public Actor{
    public:
        explicit EmergencyLight(Game* game);
        void OnUpdate(float deltaTime) override;

    private:
        class DrawAnimatedComponent* mDrawComponent;
};



#endif
