
#include "two.hpp"

#include <array>
#include "../../sq.hpp"
#include "../../Game.h"

const std::array<Planet, 1> planets = {{
    Planet({0, 0}, glm::vec2(0, 0), 0.05, 0.05)
}};

const std::array<Target, 0> targets = {{

}};

Level2::Level2(UIScreen *screen, Game &game):
    Simulation(
        std::vector(planets.begin(), planets.end()),
        std::vector(targets.begin(), targets.end()),
        60,
        1,
        2,
        screen,
        game
    )
{}
