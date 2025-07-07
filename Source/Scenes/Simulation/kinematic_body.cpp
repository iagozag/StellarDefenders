
#include "kinematic_body.hpp"


KinematicBody::KinematicBody(const glm::vec2 &position, const glm::vec2 &speed):
    m_position(position), m_speed(speed) {}

void KinematicBody::apply_acceleration(const glm::vec2 &accel, const float delta_t) {
    m_speed += accel * delta_t;
    m_position += m_speed * delta_t;
}