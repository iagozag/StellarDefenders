
#include "three.hpp"

#include <array>
#include "../../sq.hpp"
#include "../../Game.h"

const float VELOCITY_SCALE_FACTOR = 0.3;
const float GM_SCALE_FACTOR = sq(VELOCITY_SCALE_FACTOR);

const std::array<Planet, 5> planets = {{
    Planet({0, 0}, glm::vec2(0,  0.1) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR, 0.12),
    Planet({ 0.6, 0.6}, glm::vec2(0, -0.1) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR),
    Planet({ -0.6, -0.6}, glm::vec2(0, -0.1) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR),
    Planet({ 0.3, 0.3}, glm::vec2(0, -0.1) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR),
    Planet({ -0.3, -0.3}, glm::vec2(0, -0.1) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR)
}};

const std::array<Target, 2> targets = {{
    Target({0.78, 0.6}, glm::vec2(0, -0.67735) * VELOCITY_SCALE_FACTOR),
    Target({-0.78, -0.6}, glm::vec2(0, -0.67735) * VELOCITY_SCALE_FACTOR)
}};

Level3::Level3(UIScreen *screen, Game &game):
    Simulation(
        std::vector(planets.begin(), planets.end()),
        std::vector(targets.begin(), targets.end()),
        60,
        2,
        3,
        screen,
        game
    )
{}
