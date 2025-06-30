
#include "planet.hpp"

#include "../../Game.h"

Planet::Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass):
    m_position(position),
    m_speed(speed),
    m_radius(radius),
    m_mass(mass) {}

void Planet::draw(Game &game) const {
    game.draw_ellipsis(m_position, {m_radius, m_radius});
}