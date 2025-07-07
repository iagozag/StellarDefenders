#include "EmergencyLight.h"
//
// Created by Lucas N. Ferreira on 03/08/23.
//


EmergencyLight::EmergencyLight(Game* game):
    Actor(game)
{
    mDrawComponent = new DrawAnimatedComponent(this,  "../Assets/Sprites/EmergencyLight/texture.png", "../Assets/Sprites/EmergencyLight/texture.json", glm::vec2(0.5, 0.5));
    mDrawComponent->AddAnimation("flash", std::vector<int>({0, 1, 2, 3, 4, 5}));

    mDrawComponent->SetAnimation("flash");
    mDrawComponent->SetAnimFPS(10.0f);
}

void EmergencyLight::OnUpdate(float deltaTime)
{
}