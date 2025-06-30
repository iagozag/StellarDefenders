
#include "simulation.hpp"
#include "../../Game.h"
#include "../../sq.hpp"
#include "../../filter_vector.hpp"

Simulation::Simulation(std::vector<Planet> planets, std::vector<Kamikaze> kamikaze):
    m_planets(std::move(planets)),
    m_kamikaze(std::move(kamikaze)) {}

void Simulation::draw(Game &game) const {
    for(auto &planet : m_planets) {
        planet.draw(game);
    }

    for(auto &kamikaze : m_kamikaze) {
        kamikaze.draw(game);
    }
}

glm::vec2 calculate_acceleration(const glm::vec2 &body_pos, const Planet &attractor) {
    const auto diff = attractor.m_position - body_pos;
    const auto dist_sq = sq(diff.x) + sq(diff.y);
    const auto force_dir = glm::normalize(diff);
    return attractor.m_mass * force_dir / dist_sq;
}

void Simulation::run(Game &game, float delta_t) {

    glm::vec2 kamikaze_accelerations[m_kamikaze.size()];
    for(auto &acel : kamikaze_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }
    glm::vec2 planet_accelerations[m_planets.size()];
    for(auto &acel : planet_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    for(size_t i = 0; i < m_planets.size(); i++) {
        for(size_t j = i + 1; j < m_planets.size(); j++) {
            const auto diff = m_planets[i].m_position - m_planets[j].m_position;
            const auto dist_sq = sq(diff.x) + sq(diff.y);

            const auto force_dir = glm::normalize(diff);

            const glm::vec2 accel_on_i = m_planets[j].m_mass * force_dir / dist_sq;
            const glm::vec2 accel_on_j = m_planets[i].m_mass * force_dir / dist_sq;

            planet_accelerations[i] -= accel_on_i;
            planet_accelerations[j] += accel_on_j;
        }

        for(size_t j = 0; j < m_kamikaze.size(); j++) {
            kamikaze_accelerations[j] += calculate_acceleration(m_kamikaze[j].m_position, m_planets[i]);
        }
    }

    for(size_t i = 0; i < m_planets.size(); i++) {
        m_planets[i].apply_acceleration(planet_accelerations[i], delta_t);
    }

    for(size_t i = 0; i < m_kamikaze.size(); i++) {
        m_kamikaze[i].apply_acceleration(kamikaze_accelerations[i], delta_t);
    }

    run_collision_tests();
}

void Simulation::run_collision_tests() {
    std::vector<std::reference_wrapper<CollidableSphere>> collidables;
    
    for(auto &planet : m_planets) {
        collidables.push_back(planet);
    }
    for(auto &kamikaze : m_kamikaze) {
        collidables.push_back(kamikaze);
    }

    for(size_t i = 0; i < collidables.size(); i++) {
        for(size_t j = i + 1; j < collidables.size(); j++) {
            auto &coli = collidables[i].get();
            auto &colj = collidables[j].get();
            if(coli.collides(colj)) {
                coli.on_collision();
                colj.on_collision();
            }
        }
    }

    delete_dead();
}

void Simulation::delete_dead() {
    const auto kamikaze_predicate = std::function([](const Kamikaze &kamikaze) {
        return !kamikaze.m_should_delete;
    });
    
    filter(m_kamikaze, kamikaze_predicate);
}
