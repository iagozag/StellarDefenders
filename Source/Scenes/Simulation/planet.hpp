
#pragma once

#include <glm/glm.hpp>
#include "kinematic_body.hpp"
#include "collidable_sphere.hpp"

class Game;

class Planet: public KinematicBody, public CollidableSphere {
public:
    Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass);

    void draw(Game &game) const;

    glm::vec2 get_position() const override;
    void on_collision() override;

    float m_mass;
};
