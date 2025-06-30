
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string_view>

#include "planet.hpp"

class Game;

class Simulation {
public:
    Simulation(std::vector<Planet> planets);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

private:
    std::vector<Planet> m_planets;
};
