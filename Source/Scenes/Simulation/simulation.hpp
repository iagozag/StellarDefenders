
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "planet.hpp"
#include "kamikaze.hpp"
#include "target.hpp"
#include "fragment.hpp"
#include "virtual_positioning.hpp"

class Game;

class Simulation {
public:
    Simulation(std::vector<Planet> planets, std::vector<Target> targets, const float duration, const uint32_t ships_to_be_positioned);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t, const bool ignore_collision = false);

    void add_fragments(const std::vector<Fragment> &fragments);
    void add_kamikaze(const glm::vec2 &position, const glm::vec2 &speed);

    void lock();
    void unlock();

    void process_input(Game &game, const uint8_t* state);

    std::vector<glm::vec2> simulate(Game &game, const glm::vec2 &position, const glm::vec2 &speed) const;

    bool finished() const;

    const Planet *get_nearest_positionable_planet(const glm::vec2 &position) const;
    
private:

    void run_collision_tests();
    void delete_dead();

    std::vector<Planet> m_planets;
    std::vector<Kamikaze> m_kamikaze;
    std::vector<Target> m_targets;
    std::vector<Fragment> m_fragments;
    float m_time_simulated;
    float m_duration;

    VirtualPositioning m_virtual_positioning;

    uint32_t m_ships_to_be_positioned;

    bool m_locked: 1;
};
