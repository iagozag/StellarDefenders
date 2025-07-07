
#include "virtual_positioning.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "kamikaze.hpp"
#include "../../Game.h"
#include "simulation.hpp"

constexpr float ARROW_MAX_LEGTH = 0.5;
constexpr float SPEED_MULTIPLIER = 1;

VirtualPositioning::VirtualPositioning():
    m_last_pressed(false)
{}

glm::vec2 VirtualPositioning::calculate_speed(const glm::vec2 &start, const glm::vec2 &end) {
    const auto diff = start - end;
    const auto length = glm::length(diff);
    const auto used_length = std::min(length, ARROW_MAX_LEGTH);
    const auto applied_vec = (diff / length) * used_length;
    const auto speed = applied_vec * SPEED_MULTIPLIER;
    return speed;
}

void VirtualPositioning::process_input(Game &game, Simulation &simulation) {
    glm::ivec2 mouse_pos;
    const auto state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    const auto left_pressed = !!(state & SDL_BUTTON_LMASK);
    auto &camera = game.GetCamera();
    m_current_mouse_pos = camera.screen_position_to_world_position(game, mouse_pos);

    const auto nearest_planet = simulation.get_nearest_positionable_planet(m_current_mouse_pos);
    if(nearest_planet) {
        const auto position = nearest_planet->position_ship_accordingly(m_current_mouse_pos);
        m_current_positioning = position;
    }

    if(left_pressed && !m_last_pressed) {
        if(m_current_positioning) {
            m_start = m_current_positioning;
        }
    } else if(!left_pressed && m_last_pressed && m_start) {
        simulation.add_kamikaze(m_start.value(), calculate_speed(m_start.value(), m_current_mouse_pos));
        m_start = std::nullopt;
    }

    m_last_pressed = left_pressed;
}

void VirtualPositioning::draw(Game &game, const Simulation &simulation) const {
    if(m_start) {
        const auto speed = calculate_speed(m_start.value(), m_current_mouse_pos);

        auto positions = simulation.simulate(game, m_start.value(), speed);
        const auto falls = positions.size() != 599;
        auto tmp_kamikaze = Kamikaze(m_start.value(), glm::vec2(0));
        tmp_kamikaze.draw(game);
        for(size_t i = 0; i < positions.size(); i++) {
            tmp_kamikaze.m_position = positions[i];
            tmp_kamikaze.draw_ghost(game, falls && i == positions.size() - 1);
        }

    } else {
        if(m_current_positioning) {
            const auto tmp_kamikaze = Kamikaze(m_current_positioning.value(), glm::vec2(0));
            tmp_kamikaze.draw_ghost(game, false);
        }
    }
}