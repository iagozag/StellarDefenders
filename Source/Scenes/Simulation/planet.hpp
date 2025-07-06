
#pragma once

#include <glm/glm.hpp>
#include "kinematic_body.hpp"
#include "collidable_sphere.hpp"

class Game;

class Planet: public KinematicBody, public CollidableSphere {
public:
    Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass, const float positioning_radius = 0);

    void draw(Game &game) const;

    // Mouse position deve corresponder às coordenadas já normalizadas (de -1 até 1)
    glm::vec2 position_ship_accordingly(const glm::vec2 &mouse_position);

    glm::vec2 get_position() const override;
    void on_collision() override;

    float m_mass;
    float m_positioning_radius;
};
