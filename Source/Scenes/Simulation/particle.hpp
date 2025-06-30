
#pragma once

#include <glm/glm.hpp>
#include "kinematic_body.hpp"
#include "collidable_sphere.hpp"

class Particle: public KinematicBody, public CollidableSphere {
public:
    Particle(const glm::vec2 &position, const glm::vec2 &speed, const float radius);

    glm::vec2 get_position() const override;
};
