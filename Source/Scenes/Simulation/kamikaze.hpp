
#pragma once

#include "kinematic_body.hpp"
#include "collidable_sphere.hpp"

class Game;

constexpr float KAMIKAZE_DETONATION_RADIUS = 0.08;
constexpr float KAMIKAZE_RADIUS = 0.01;

class Kamikaze: public KinematicBody, public CollidableSphere {
public:
    Kamikaze(const glm::vec2 &position, const glm::vec2 &speed);

    bool target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const;
    void draw(Game &game) const;

    glm::vec2 get_position() const override;
    void on_collision() override;

    bool m_should_delete: 1;
};
