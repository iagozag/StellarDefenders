
#include "simulation.hpp"
#include "../../Game.h"
#include "../../sq.hpp"
#include "../../filter_vector.hpp"

#include <iostream>

Simulation::Simulation(std::vector<Planet> planets, std::vector<Target> targets):
    m_planets(std::move(planets)),
    m_targets(std::move(targets)),
    m_locked(false) {}

void Simulation::draw(Game &game) const {
    for(auto &planet : m_planets) {
        planet.draw(game);
    }

    for(auto &kamikaze : m_kamikaze) {
        kamikaze.draw(game);
    }

    for(auto &target : m_targets) {
        target.draw(game);
    }

    for(auto &fragment : m_fragments) {
        fragment.draw(game);
    }
}

std::vector<glm::vec2> Simulation::simulate(Game &game, const glm::vec2 &position, const glm::vec2 &speed) const {
    auto copy = Simulation(m_planets, {});
    copy.add_kamikaze(position, speed);
    copy.unlock();

    constexpr float DELTA_T = 1 / 60.f;
    constexpr float DURATION = 10;
    constexpr size_t NUM_STEPS = DURATION / DELTA_T;

    std::vector<glm::vec2> res;
    res.reserve(NUM_STEPS);
    for(size_t i = 0; i < NUM_STEPS && copy.m_kamikaze.size(); i++) {
        res.push_back(copy.m_kamikaze[0].m_position);
        copy.run(game, DELTA_T);
    }

    return res;
}

glm::vec2 calculate_acceleration(const glm::vec2 &body_pos, const Planet &attractor) {
    const auto diff = attractor.m_position - body_pos;
    const auto dist_sq = sq(diff.x) + sq(diff.y);
    const auto force_dir = glm::normalize(diff);
    return attractor.m_mass * force_dir / dist_sq;
}

void Simulation::run(Game &game, float delta_t) {
    if(m_locked) {
        return;
    }

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

    glm::vec2 target_accelerations[m_targets.size()];
    for(auto &acel : target_accelerations) {
        acel.x = 0;
        acel.y = 0;
    }

    glm::vec2 fragment_accelerations[m_fragments.size()];
    for(auto &acel : fragment_accelerations) {
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

        for(size_t j = 0; j < m_targets.size(); j++) {
            target_accelerations[j] += calculate_acceleration(m_targets[j].m_position, m_planets[i]);
        }

        for(size_t j = 0; j < m_fragments.size(); j++) {
            fragment_accelerations[j] += calculate_acceleration(m_fragments[j].m_position, m_planets[i]);
        }
    }

    for(size_t i = 0; i < m_planets.size(); i++) {
        m_planets[i].apply_acceleration(planet_accelerations[i], delta_t);
    }

    for(size_t i = 0; i < m_kamikaze.size(); i++) {
        m_kamikaze[i].apply_acceleration(kamikaze_accelerations[i], delta_t);
    }

    for(size_t i = 0; i < m_targets.size(); i++) {
        m_targets[i].apply_acceleration(target_accelerations[i], delta_t);
    }
    
    for(size_t i = 0; i < m_fragments.size(); i++) {
        m_fragments[i].apply_acceleration(fragment_accelerations[i], delta_t);
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
    for(auto &target : m_targets) {
        collidables.push_back(target);
    }
    for(auto &fragment : m_fragments) {
        collidables.push_back(fragment);
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


    std::vector<std::reference_wrapper<KinematicBody>> targets;
    targets.reserve(m_targets.size());
    for(size_t i = 0; i < m_targets.size(); i++) {
        targets.push_back(m_targets[i]);
    }

    for(size_t i = 0; i < m_kamikaze.size(); i++) {
        if(m_kamikaze[i].target_inside_radius(targets.data(), targets.size())) {
            m_kamikaze[i].m_should_delete = true;
            add_fragments(m_kamikaze[i].generate_fragments());
        }
    }


    delete_dead();
}

void Simulation::delete_dead() {
    const auto kamikaze_predicate = std::function([](const Kamikaze &kamikaze) {
        return !kamikaze.m_should_delete;
    });

    const auto target_predicate = std::function([](const Target &target) {
        return !target.m_should_delete;
    });

    const auto fragment_predicate = std::function([](const Fragment &fragment) {
        return !fragment.m_should_delete;
    });
    
    filter(m_kamikaze, kamikaze_predicate);
    filter(m_targets, target_predicate);
    filter(m_fragments, fragment_predicate);
}

void Simulation::add_fragments(const std::vector<Fragment> &fragments) {
    for(auto &fragment : fragments) {
        m_fragments.push_back(fragment);
    }
}

void Simulation::lock() {
    m_locked = true;
}

void Simulation::unlock() {
    m_locked = false;
}

void Simulation::add_kamikaze(const glm::vec2 &position, const glm::vec2 &speed) {
    m_kamikaze.emplace_back(position, speed);
}
