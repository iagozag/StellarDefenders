
#include "fragment.hpp"

#include "../../Game.h"

constexpr float FRAGMENT_RADIUS = 0.005;

Fragment::Fragment(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, FRAGMENT_RADIUS),
    m_should_delete(false) {}

void Fragment::on_collision() {}

void Fragment::draw(Game &game) const {
    game.draw_ellipsis(m_position, glm::vec2(m_radius) * 2.f, {200, 10, 10, 255});
}
