
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../../Game.h"

struct Planet {
    glm::vec2 position;
    glm::vec2 speed;
    float radius;
    float mass;
};

class Simulation {
public:
    Simulation(std::vector<Planet> planets);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

private:
    std::vector<Planet> m_planets;
};
