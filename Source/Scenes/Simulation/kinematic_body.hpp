
#pragma once

#include <glm/glm.hpp>

class KinematicBody {
public:
    KinematicBody(const glm::vec2 &position, const glm::vec2 &speed);

    void apply_acceleration(const glm::vec2 &accel, const float delta_t);

    glm::vec2 m_position;
    glm::vec2 m_speed;
};
