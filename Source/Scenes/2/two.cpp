
#include "two.hpp"

#include <array>
#include "../../sq.hpp"
#include "../../Game.h"

const std::array<Planet, 3> planets = {{
    Planet({0, 0}, glm::vec2(0, 0), 0.05, 0.1),
    Planet({0.4f, 0.0f}, glm::vec2(0.0f, 0.5f), 0.02, 0.002, 0.05),
    Planet({0.0f, -0.8f}, glm::vec2(0.35355f, 0.0f), 0.03, 0.004)
}};

const std::array<Target, 1> targets = {{
    Target({0.0f, -0.755f}, {0.05541f, 0.0f})
}};

Level2::Level2(UIScreen *screen, Game &game):
    Simulation(
        std::vector(planets.begin(), planets.end()),
        std::vector(targets.begin(), targets.end()),
        20,
        1,
        2,
        screen,
        game
    )
{}
