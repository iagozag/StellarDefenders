
#include "one.hpp"

#include <array>
#include "../../sq.hpp"
#include "../../Game.h"

const float VELOCITY_SCALE_FACTOR = 0.3;
const float GM_SCALE_FACTOR = sq(VELOCITY_SCALE_FACTOR);

const std::array<Planet, 2> planets = {{
    Planet({-0.25, 0}, glm::vec2(0,  0.15) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR, 0.12),
    Planet({ 0.25, 0}, glm::vec2(0, -0.15) * VELOCITY_SCALE_FACTOR, 0.03, 0.02 * GM_SCALE_FACTOR)
}};

const std::array<Target, 1> targets = {{
    Target({0.32, 0}, glm::vec2(0, -0.67735) * VELOCITY_SCALE_FACTOR)
}};

Level1::Level1(UIScreen *screen, Game &game):
    Simulation(
        std::vector(planets.begin(), planets.end()),
        std::vector(targets.begin(), targets.end()),
        60,
        1,
        1,
        screen,
        game
    )
{}
