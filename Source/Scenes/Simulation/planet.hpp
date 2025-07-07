
#pragma once

#include <glm/glm.hpp>
#include "particle.hpp"

class Game;

class Planet: public Particle {
public:
    Planet(const glm::vec2 &position, const glm::vec2 &speed, const float radius, const float mass, const float positioning_radius = 0);

    void draw(Game &game, const bool draw_positioning_radius) const;

    // Mouse position deve corresponder às coordenadas já normalizadas (de -1 até 1)
    glm::vec2 position_ship_accordingly(const glm::vec2 &mouse_position) const;

    void on_collision(const CollidableIdentifier collided) override;
    CollidableIdentifier get_identifier() const override;

    float m_mass;
    float m_positioning_radius;
};
