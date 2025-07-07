
#include "ShipMenu.hpp"
#include "../../rect_transform.hpp"
#include <iostream>

ShipMenu::ShipMenu(Game& game):
    mAlien(new Alien(&game)),
    mBackgroundTexture(game.LoadTexture("../Assets/Sprites/background.png"))
{
    mAlien->SetPosition(glm::vec2(-1, -1));
    mBackgroundPosition = glm::vec2(-1, -1);
    mBackgroundSize = glm::vec2(Game::WORLD_WIDTH, Game::WORLD_HEIGHT) / (float)Game::WORLD_HEIGHT * 2.f;
}

ShipMenu::~ShipMenu(){
    delete mAlien;
    mAlien = nullptr;
    if (mBackgroundTexture) {
        SDL_DestroyTexture(mBackgroundTexture);
        mBackgroundTexture = nullptr;
    }
}

void ShipMenu::draw(Game& game) const{
    auto &cam = game.GetCamera();
    auto viewport = cam.get_viewport_size(game);

    SDL_FRect destRect;
    destRect.x = mBackgroundPosition.x;
    destRect.y = mBackgroundPosition.y;
    destRect.w = mBackgroundSize.x;
    destRect.h = mBackgroundSize.y;

    cam.m_pos.x = std::max(cam.m_pos.x, viewport.x/2.0f-1.0f);
    cam.m_pos.x = std::min(cam.m_pos.x, destRect.w-1.f-viewport.x/2.0f);

    const auto transform = cam.get_total_transformation_matrix(game);
    const auto transformed_dest = rect_transform(destRect, transform);
    SDL_RenderCopyF(game.GetRenderer(), mBackgroundTexture, nullptr, &transformed_dest);
}

void ShipMenu::run(const float delta_t){

}