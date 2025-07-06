
#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../../Actors/Alien.h"

class Alien;
class Game;

class ShipMenu {
public:
    ShipMenu(Game &game);
    ~ShipMenu();

    Alien* GetAlien() const { return mAlien; }

    void draw(Game& game) const;
    void run(const float delta_t);

private:
    Alien* mAlien;
    SDL_Texture *mBackgroundTexture;
    glm::vec2 mBackgroundPosition;
    glm::vec2 mBackgroundSize;
};
