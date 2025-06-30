
#include "simulation.hpp"
#include "../../Game.h"

Simulation::Simulation(std::vector<Planet> planets):
    m_planets(std::move(planets)) {}

void Simulation::draw(Game &game) const {
    for(auto &planet : m_planets) {
        planet.draw(game);
    }
}

#include <iostream>

void Simulation::run(Game &game, float delta_t) {

    glm::vec2 accelerations[m_planets.size()];
    for(auto &acel : accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    for(size_t i = 0; i < m_planets.size(); i++) {
        for(size_t j = i + 1; j < m_planets.size(); j++) {
            const auto diff = m_planets[i].m_position - m_planets[j].m_position;
            const auto dist_sq = diff.x * diff.x + diff.y * diff.y;

            const auto force_dir = glm::normalize(diff);

            const glm::vec2 accel_on_i = m_planets[j].m_mass * force_dir / dist_sq;
            const glm::vec2 accel_on_j = m_planets[i].m_mass * force_dir / dist_sq;

            accelerations[i] -= accel_on_i;
            accelerations[j] += accel_on_j;
        }
    }

    for(auto &ac : accelerations) {
        std::cout << ac.x << ' ' << ac.y << '\n';
    }
    std::cout << '\n';

    for(size_t i = 0; i < m_planets.size(); i++) {
        m_planets[i].m_speed += accelerations[i] * delta_t;
        m_planets[i].m_position += m_planets[i].m_speed * delta_t;
    }
}
