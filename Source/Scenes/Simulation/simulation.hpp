
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string_view>

class Game;

// class Ship {

// };

class Body {

};

class Projectile {

};

class Missile {

};

class Laser {

};

struct Planet {
    glm::vec2 position;
    glm::vec2 speed;
    float radius;
    float mass;
    std::string_view textureName;

    void draw() {

    }
};

class Simulation {
public:
    Simulation(std::vector<Planet> planets);

    void draw(Game &game) const;
    void run(Game &game, const float delta_t);

private:
    std::vector<Planet> m_planets;
    std::vector<Laser> m_lasers;
};
