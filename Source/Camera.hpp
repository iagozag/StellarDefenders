
#pragma once

#include <glm/glm.hpp>

class Game;

class Camera {
public:
    Camera();

    glm::mat4 get_total_transformation_matrix(const Game &game) const;
    glm::mat4 get_projection_matrix(const Game &game) const;
    glm::mat4 get_screen_matrix(const Game &game) const;
    glm::vec2 get_viewport_size(const Game &game) const;

    glm::vec2 m_pos;
    float m_scale;
};
