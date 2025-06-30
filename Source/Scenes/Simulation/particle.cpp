
#include "particle.hpp"

Particle::Particle(const glm::vec2 &position, const glm::vec2 &speed, const float radius):
    KinematicBody(position, speed),
    CollidableSphere(radius) {}

glm::vec2 Particle::get_position() const {
    return m_position;
}
