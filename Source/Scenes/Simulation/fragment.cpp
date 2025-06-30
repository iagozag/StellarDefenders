
#include "fragment.hpp"

#include "../../Game.h"


Fragment::Fragment(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, FRAGMENT_RADIUS),
    m_should_delete(false) {}

void Fragment::on_collision() {}

void Fragment::draw(Game &game) const {
    game.draw_ellipsis(m_position, {m_radius, m_radius});
}
