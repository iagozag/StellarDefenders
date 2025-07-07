
#pragma once

#include "particle.hpp"

class Game;

class Target: public Particle {
public:
    Target(const glm::vec2 &position, const glm::vec2 &speed);

    void draw(Game &game) const;

    void on_collision(const CollidableIdentifier collided) override;
    CollidableIdentifier get_identifier() const override;

    bool m_should_delete: 1;
};
