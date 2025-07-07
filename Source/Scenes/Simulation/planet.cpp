
#include "planet.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "../../Game.h"

Planet::Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass, const float positioning_radius):
    Particle(position, speed, radius),
    m_mass(mass),
    m_positioning_radius(positioning_radius) {}

void Planet::draw(Game &game, const bool draw_positioning_radius) const {
    if(draw_positioning_radius) {
        game.draw_ellipsis(m_position, glm::vec2(m_positioning_radius) * 2.f, {100, 200, 0, 100});
    }
    game.draw_ellipsis(m_position, glm::vec2(m_radius) * 2.f, {100, 255, 200, 255});
}

void Planet::on_collision() {}

glm::vec2 Planet::position_ship_accordingly(const glm::vec2 &mouse_position) const {
    const auto direction_to_mouse = mouse_position - m_position;
    const auto dist_sq = length2(direction_to_mouse);
    const auto dist = glm::sqrt(dist_sq);

    if(dist >= m_radius && dist <= m_positioning_radius) {
        return mouse_position;
    }

    if(dist == 0) {
        return m_position + glm::vec2(m_radius, 0);
    }

    const auto norm = direction_to_mouse / dist;
    if(dist < m_radius) {
        return m_position + norm * m_radius;
    } else {
        return m_position + norm * m_positioning_radius;
    }
}
