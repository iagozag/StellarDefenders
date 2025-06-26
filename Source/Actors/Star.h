//
// Created by Lucas N. Ferreira on 10/09/23.
//

#pragma once
#include "Actor.h"

class Star : public Actor
{
public:
    Star(Game* game, float radius, int numVertices = 10, float forwardForce = 0.0f);
    ~Star();

private:
    static std::vector<glm::vec2> GenerateVertices(int numVertices, float radius);
    static glm::vec2 GenerateRandomStartingForce(float min, float max);

    static float CalculateAverageVerticesLength(std::vector<glm::vec2>& vertices);

    class DrawSpriteComponent* mDrawSpriteComponent;
    class RigidBodyComponent* mRigidBodyComponent;
    class CircleColliderComponent* mCircleColliderComponent;
};
