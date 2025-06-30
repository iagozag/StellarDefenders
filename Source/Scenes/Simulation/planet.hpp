
#pragma once

#include <glm/glm.hpp>

class Game;

class Planet {
public:
    Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass);

    void draw(Game &game) const;

    glm::vec2 m_position;
    glm::vec2 m_speed;
    float m_radius;
    float m_mass;
};
