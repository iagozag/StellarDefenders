#include <random>
#include <glm/gtc/constants.hpp>

#include "kamikaze.hpp"
#include "simulation.hpp"
#include "../../sq.hpp"
#include "../../Game.h"
#include "../../norm_greater.hpp"
#include <iostream>

Kamikaze::Kamikaze(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, KAMIKAZE_RADIUS),
    m_should_delete(false)
{} 

bool Kamikaze::target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const {
    for(size_t i = 0; i < amount_targets; i++) {
        auto &possible_target = targets[i].get();
        const auto dist = m_position - possible_target.m_position;
        const auto dist_sq = sq(dist.x) + sq(dist.y);
        if(dist_sq < sq(KAMIKAZE_DETONATION_RADIUS)) {
            return true;
        }
    }
    return false;
}

void Kamikaze::draw(Game &game) const {
    game.draw_ellipsis(m_position, glm::vec2(KAMIKAZE_RADIUS) * 2.f, {0, 0, 255, 255});
}

void Kamikaze::draw_ghost(Game &game) const {
    game.draw_ellipsis(m_position, glm::vec2(KAMIKAZE_RADIUS) * 2.f, {0, 0, 255, 100}, 16);
}

void Kamikaze::on_collision() {
    m_should_delete = true;
}

std::vector<Fragment> Kamikaze::generate_fragments() const {
    constexpr size_t AMOUNT = 32;
    std::vector<Fragment> fragments;
    fragments.reserve(AMOUNT);

    auto engine = std::default_random_engine();
    auto speed_magnitude_distribution = std::normal_distribution<float>(0.1f, 0.05f);
    auto angle_distribution = std::uniform_real_distribution<float>(0.0f, glm::two_pi<float>());
    auto initial_offset_distribution = std::normal_distribution<float>(m_radius / 2.0f, m_radius / 4.0f);

    for (size_t i = 0; i < AMOUNT; ++i) {
        const float angle = angle_distribution(engine);
        const float speed_magnitude = speed_magnitude_distribution(engine);

        const float sx = speed_magnitude * glm::cos(angle);
        const float sy = speed_magnitude * glm::sin(angle);
        const glm::vec2 speed = {sx, sy};

        const float offset_distance = initial_offset_distribution(engine);

        const glm::vec2 initial_position = m_position + glm::vec2(offset_distance * glm::cos(angle), offset_distance * glm::sin(angle));

        fragments.emplace_back(initial_position, m_speed + speed);
    }
    return fragments;
}
