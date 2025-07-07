
#include "fragment.hpp"

#include "../../Game.h"

constexpr glm::vec3 INITIAL_COLOR = {1.0f, 1.0f, 1.0f};
constexpr float DECAY_RED   = 0.2f;
constexpr float DECAY_GREEN = 0.3f;
constexpr float DECAY_BLUE  = 0.8f;
constexpr float FRAGMENT_RADIUS = 0.002;

Fragment::Fragment(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, FRAGMENT_RADIUS),
    m_should_delete(false),
    m_time_alive(0) {}

void Fragment::on_collision() {}

void Fragment::draw(Game &game) const {

    const float r = INITIAL_COLOR.r * std::exp(-DECAY_RED * m_time_alive);
    const float g = INITIAL_COLOR.g * std::exp(-DECAY_GREEN * m_time_alive);
    const float b = INITIAL_COLOR.b * std::exp(-DECAY_BLUE * m_time_alive);

    const glm::u8vec4 final_color = {
        uint8_t(r * 255),
        uint8_t(g * 255),
        uint8_t(b * 255),
        255
    };

    game.draw_ellipsis(m_position, glm::vec2(m_radius) * 2.f, final_color);
}
