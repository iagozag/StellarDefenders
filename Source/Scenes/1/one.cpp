
#include "one.hpp"

#include <array>
#include "../../sq.hpp"

const float VELOCITY_SCALE_FACTOR = 0.3;
const float GM_SCALE_FACTOR = sq(VELOCITY_SCALE_FACTOR);

const std::array<Planet, 2> planets = {{
    Planet({-0.5, 0}, glm::vec2(0,  0.1) * VELOCITY_SCALE_FACTOR, 0.07, 0.02 * GM_SCALE_FACTOR, 0.12),
    Planet({ 0.5, 0}, glm::vec2(0, -0.1) * VELOCITY_SCALE_FACTOR, 0.07, 0.02 * GM_SCALE_FACTOR)
}};

const std::array<Target, 1> targets = {{
    Target({0.44, 0}, glm::vec2(0, -0.67735) * VELOCITY_SCALE_FACTOR)
}};

Level1::Level1():
    Simulation(
        std::vector(planets.begin(), planets.end()),
        std::vector(targets.begin(), targets.end()),
        60
    )
{
}
