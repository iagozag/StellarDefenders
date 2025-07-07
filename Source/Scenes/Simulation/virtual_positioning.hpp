
#pragma once

#include <optional>
#include <glm/glm.hpp>

class Game;
class Simulation;

class VirtualPositioning {
public:
    VirtualPositioning();

    void process_input(Game &game, Simulation &simulation);
    void draw(Game &game, const Simulation &simulation) const;

private:
    static glm::vec2 calculate_speed(const glm::vec2 &start, const glm::vec2 &end);

    glm::vec2 m_current_mouse_pos;
    std::optional<glm::vec2> m_current_positioning;
    std::optional<glm::vec2> m_start;
    bool m_last_pressed: 1;
};
