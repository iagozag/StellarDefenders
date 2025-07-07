
#pragma once

#include "../Simulation/simulation.hpp"
#include "../../UIElements/UIScreen.h"

class Game;

class Level3: public Simulation {
public:
    Level3(UIScreen *screen, Game &game);
};
