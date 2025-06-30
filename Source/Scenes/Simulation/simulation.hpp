
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "planet.hpp"
#include "kamikaze.hpp"
#include "target.hpp"
#include "fragment.hpp"

class Game;

class Simulation {
public:
    Simulation(std::vector<Planet> planets, std::vector<Kamikaze> kamikaze, std::vector<Target> targets);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

    void add_fragments(const std::vector<Fragment> &fragments);

private:

    void run_collision_tests();
    void delete_dead();

    std::vector<Planet> m_planets;
    std::vector<Kamikaze> m_kamikaze;
    std::vector<Target> m_targets;
    std::vector<Fragment> m_fragments;
};
