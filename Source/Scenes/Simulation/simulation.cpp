
#include "simulation.hpp"

Simulation::Simulation(std::vector<Planet> planets):
    planets(std::move(planets)) {}

void Simulation::draw(Game &game) const {

}

void Simulation::run(Game &game, const float delta_t) {

}
