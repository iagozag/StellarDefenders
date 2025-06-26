
#include "simulation.hpp"
#include "../../Game.h"

Simulation::Simulation(std::vector<Planet> planets):
    m_planets(std::move(planets)) {}

void Simulation::draw(Game &game) const {
    game.draw_ellipsis({0, 0}, {0.5, 0.25});
}

void Simulation::run(Game &game, const float delta_t) {

}
