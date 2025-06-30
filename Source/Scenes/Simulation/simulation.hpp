
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "planet.hpp"
#include "kamikaze.hpp"

class Game;

class Simulation {
public:
    Simulation(std::vector<Planet> planets, std::vector<Kamikaze> kamikaze);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

private:

    void run_collision_tests();
    void delete_dead();

    std::vector<Planet> m_planets;
    std::vector<Kamikaze> m_kamikaze;
};
