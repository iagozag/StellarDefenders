
#include "target.hpp"

#include "../../Game.h"

constexpr float TARGET_RADIUS = 0.005;

Target::Target(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, TARGET_RADIUS),
    m_should_delete(false) {}

void Target::draw(Game &game) const {
    game.draw_ellipsis(m_position, glm::vec2(m_radius) * 2.f, {200, 30, 40, 255});
}

void Target::on_collision() {
    m_should_delete = true;
}