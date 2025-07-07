#include <random>
#include <glm/gtc/constants.hpp>

#include "kamikaze.hpp"
#include "simulation.hpp"
#include "../../sq.hpp"
#include "../../Game.h"
#include "../../norm_greater.hpp"
#include <iostream>

Kamikaze::Kamikaze(const glm::vec2 &position, const glm::vec2 &speed):
    Particle(position, speed, KAMIKAZE_RADIUS),
    m_should_delete(false),
    mShipState(ShipState::Positioning)
    {} 

bool Kamikaze::target_inside_radius(const std::reference_wrapper<KinematicBody> targets[], const size_t amount_targets) const {
    for(size_t i = 0; i < amount_targets; i++) {
        auto &possible_target = targets[i].get();
        const auto dist = m_position - possible_target.m_position;
        const auto dist_sq = sq(dist.x) + sq(dist.y);
        if(dist_sq < sq(KAMIKAZE_DETONATION_RADIUS)) {
            return true;
        }
    }
    return false;
}

void Kamikaze::draw(Game &game) const {
    game.draw_ellipsis(m_position, glm::vec2(KAMIKAZE_RADIUS) * 2.f, {0, 0, 255, 255});
}

void Kamikaze::on_collision() {
    m_should_delete = true;
}

std::vector<Fragment> Kamikaze::generate_fragments() const {
    constexpr size_t AMOUNT = 32;
    std::vector<Fragment> fragments;
    fragments.reserve(AMOUNT);

    auto engine = std::default_random_engine();
    auto speed_magnitude_distribution = std::normal_distribution<float>(0.1f, 0.05f);
    auto angle_distribution = std::uniform_real_distribution<float>(0.0f, glm::two_pi<float>());
    auto initial_offset_distribution = std::normal_distribution<float>(m_radius / 2.0f, m_radius / 4.0f);

    for (size_t i = 0; i < AMOUNT; ++i) {
        const float angle = angle_distribution(engine);
        const float speed_magnitude = speed_magnitude_distribution(engine);

        const float sx = speed_magnitude * glm::cos(angle);
        const float sy = speed_magnitude * glm::sin(angle);
        const glm::vec2 speed = {sx, sy};

        const float offset_distance = initial_offset_distribution(engine);

        const glm::vec2 initial_position = m_position + glm::vec2(offset_distance * glm::cos(angle), offset_distance * glm::sin(angle));

        fragments.emplace_back(initial_position, m_speed + speed);
    }
    return fragments;
}

void Kamikaze::DrawSlingShotLine(Game& game, std::vector<glm::vec2> vertices) const
{
    int n = vertices.size();

    SDL_SetRenderDrawColor(game.GetRenderer(), 255, 255, 255, 255);
    for(int i = 0; i < n-1; i++)
        SDL_RenderDrawLine(game.GetRenderer(), vertices[i].x, vertices[i].y, vertices[i+1].x, vertices[i+1].y);

    // glm::vec2 v = vertices[n-1]-vertices[n-2];

    // const float headSize = 0.1f;
    // const float headAngle = glm::pi<float>() / 6.0f;

    // if (glm::length(v) == 0) return;
    
    // v = glm::normalize(v);

    // glm::vec2 point1 = vertices[n-1] - headSize * glm::rotate(v, headAngle);
    // glm::vec2 point2 = end - headSize * glm::rotate(v, -headAngle);

    // SDL_RenderDrawLine(mGame->GetRenderer(), end.x, end.y, point1.x, point1.y);
    // SDL_RenderDrawLine(mGame->GetRenderer(), end.x, end.y, point2.x, point2.y);

    SDL_RenderPresent(game.GetRenderer());
}

void Kamikaze::OnProcessInput(const uint8_t* state)
{
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mShipState == ShipState::Positioning and (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))){
        mShipState = ShipState::SlingShot;
        mOriginPos = m_position;
    }
    else if(mShipState == ShipState::SlingShot){
        mSlingShotMousePos = glm::vec2(mouseX, mouseY);
        if(!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))){
            mShipState = ShipState::Ready;
            mDirection = m_position - mSlingShotMousePos;
        }
    }
}

void Kamikaze::OnUpdate(Game& game, float deltaTime)
{
    if(mShipState == ShipState::Positioning){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX,&mouseY);

        // SDL_Rect area = mGame->GetViableArea();

        // if(mouseX < area.x) mouseX = area.x;
        // if(mouseX > area.x+area.w-mHeight/2.0f) mouseX = area.x+area.w-mHeight/2.0f;
        // if(mouseY < area.y) mouseY = area.y;
        // if(mouseY > area.y+area.h-mHeight/2.0f) mouseY = area.y+area.h-mHeight/2.0f;
    
        auto& cam = game.GetCamera();
        glm::vec2 newPos = glm::vec2((float)mouseX, (float)mouseY);
        glm::vec2 normalized = cam.screen_position_to_world_position(game, newPos);
        m_position = normalized;
    }
    else if(mShipState == ShipState::SlingShot) m_position = mOriginPos;
    else if (mShipState == ShipState::Simulating)
    {
    }
}