
#pragma once

#include "../Simulation/simulation.hpp"
#include "../../UIElements/UIScreen.h"

class Game;

class Level2: public Simulation {
public:
    Level2(UIScreen *screen, Game &game);
};
