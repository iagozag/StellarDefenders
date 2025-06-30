
#include "planet.hpp"

#include "../../Game.h"

Planet::Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass):
    KinematicBody(position, speed),
    CollidableSphere(radius),
    m_mass(mass) {}

void Planet::draw(Game &game) const {
    game.draw_ellipsis(m_position, {m_radius * 2, m_radius * 2});
}

glm::vec2 Planet::get_position() const {
    return m_position;
}

void Planet::on_collision() {}
