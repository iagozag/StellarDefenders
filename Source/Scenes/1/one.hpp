
#pragma once

#include "../Simulation/simulation.hpp"
#include "../../UIElements/UIScreen.h"

class Game;

class Level1: public Simulation {
public:
    Level1(UIScreen *screen, Game &game);
};
