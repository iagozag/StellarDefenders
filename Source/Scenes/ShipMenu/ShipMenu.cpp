
#include "ShipMenu.hpp"
#include "../../rect_transform.hpp"

ShipMenu::ShipMenu(Game& game):
    mAlien(new Alien(&game)),
    mBackgroundTexture(game.LoadTexture("../Assets/Sprites/background.png"))
{
    mAlien->SetPosition(glm::vec2(-1, -1));
    mBackgroundPosition = glm::vec2(-1, -1);
    mBackgroundSize = glm::vec2(2.0*(float)Game::WORLD_WIDTH/(float)game.GetWindowWidth(),2.0);
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
    if (mBackgroundTexture){
        SDL_FRect destRect;
        destRect.x = static_cast<int>(mBackgroundPosition.x-game.GetCamera().m_pos.x);
        destRect.y = static_cast<int>(mBackgroundPosition.y);
        destRect.w = static_cast<int>(mBackgroundSize.x);
        destRect.h = static_cast<int>(mBackgroundSize.y);

        const auto transform = game.GetCamera().get_total_transformation_matrix(game);
        const auto transformed_dest = rect_transform(destRect, transform);
        SDL_RenderCopyF(game.GetRenderer(), mBackgroundTexture, nullptr, &transformed_dest);
    }
}

void ShipMenu::run(const float delta_t){

}