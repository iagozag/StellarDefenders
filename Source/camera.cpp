
#include "camera.hpp"

#include "Game.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(): m_pos(0, 0), m_scale(1) {}

// Em camera.cpp

glm::mat4 Camera::get_projection_matrix(const Game &game) const {
    const auto window_dimensions = game.get_window_dimensions();
    const float aspect_ratio = static_cast<float>(window_dimensions.x) / static_cast<float>(window_dimensions.y);

    float view_width;
    float view_height;

    // m_scale define o tamanho da menor dimensão visível na tela.
    const float view_size_min = m_scale; 

    if (aspect_ratio < 1.0f) {
        // --- Modo Portrait (tela mais alta que larga) ---
        // A largura é a dimensão limitante.
        view_width = view_size_min;
        // A altura é calculada para preencher a tela, mantendo a proporção.
        view_height = view_size_min / aspect_ratio;
    } else {
        // --- Modo Landscape (tela mais larga que alta ou quadrada) ---
        // A altura é a dimensão limitante.
        view_height = view_size_min;
        // A largura é calculada para preencher a tela, mantendo a proporção.
        view_width = view_size_min * aspect_ratio;
    }

    // Calcula os limites da visão com base na posição da câmera e na área de visão calculada.
    float left   = m_pos.x - view_width / 2.0f;
    float right  = m_pos.x + view_width / 2.0f;
    float bottom = m_pos.y - view_height / 2.0f;
    float top    = m_pos.y + view_height / 2.0f;

    // Gera a matriz de projeção ortográfica com a nova visão.
    return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

glm::mat4 Camera::get_screen_matrix(const Game &game) const {
    const auto window_dimensions = game.get_window_dimensions();
    const float screen_width = static_cast<float>(window_dimensions.x);
    const float screen_height = static_cast<float>(window_dimensions.y);

    // Começa com a matriz identidade
    glm::mat4 mat = glm::mat4(1.0f);

    // ***************************************************************
    // A ORDEM CORRETA É ESTA: PRIMEIRO ESCALAR, DEPOIS TRANSADAR.
    // ***************************************************************

    // 1. Escala o intervalo de NDC [-1, 1] para o tamanho da tela,
    //    já invertendo o eixo Y com o valor negativo.
    mat = glm::scale(mat, glm::vec3(screen_width / 2.0f, -screen_height / 2.0f, 1.0f));
    
    // 2. Translada a origem, que agora está no canto superior esquerdo,
    //    para o centro da tela.
    mat = glm::translate(mat, glm::vec3(1.0f, -1.0f, 0.0f));


    return mat;
}

// Retorna a matriz de transformação completa: World Space -> Screen Space
glm::mat4 Camera::get_total_transformation_matrix(const Game &game) const {
    return get_screen_matrix(game) * get_projection_matrix(game);
}
