
#pragma once

#include <glm/glm.hpp>
#include "particle.hpp"

class Game;

class Fragment: public Particle {
public:
    Fragment(const glm::vec2 &position, const glm::vec2 &speed);

    void on_collision(const CollidableIdentifier collided) override;
    CollidableIdentifier get_identifier() const override;
    void draw(Game &game) const;

    bool m_should_delete: 1;
    float m_time_alive;
};
