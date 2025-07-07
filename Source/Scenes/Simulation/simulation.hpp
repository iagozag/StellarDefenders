
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
    Simulation(std::vector<Planet> planets, std::vector<Target> targets, const float duration);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

    void add_fragments(const std::vector<Fragment> &fragments);
    void add_kamikaze(const glm::vec2 &position, const glm::vec2 &speed);

    void lock();
    void unlock();

    void ProcessInput(const uint8_t* state);

    std::vector<glm::vec2> simulate(Game &game, const glm::vec2 &position, const glm::vec2 &speed) const;

    bool finished() const;
    
private:

    void run_collision_tests();
    void delete_dead();

    std::vector<Planet> m_planets;
    std::vector<Kamikaze> m_kamikaze;
    std::vector<Target> m_targets;
    std::vector<Fragment> m_fragments;
    float m_time_simulated;
    float m_duration;

    bool m_locked: 1;
};
