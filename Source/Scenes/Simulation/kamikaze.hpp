
#pragma once

#include "particle.hpp"
#include "fragment.hpp"

class Game;

constexpr float KAMIKAZE_DETONATION_RADIUS = 0.08;
constexpr float KAMIKAZE_RADIUS = 0.01;

enum class ShipState
{
    Positioning,
    SlingShot,
    Ready,
    Simulating
};

class Kamikaze: public Particle {
public:
    Kamikaze(const glm::vec2 &position, const glm::vec2 &speed);

    bool target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const;
    void draw(Game &game) const;
    void draw_ghost(Game &game) const;

    std::vector<Fragment> generate_fragments() const;

    void on_collision(const CollidableIdentifier collided) override;
    CollidableIdentifier get_identifier() const override;

    bool m_should_delete: 1;
};
