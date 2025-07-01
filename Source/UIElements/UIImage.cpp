//
// Created by Lucas N. Ferreira on 28/05/25.
//

#include "UIImage.h"
#include "../Game.h"
#include "../rect_transform.hpp"

UIImage::UIImage(SDL_Renderer* renderer, const std::string &imagePath, const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color)
    : UIElement(pos, size, color),
    mTexture(nullptr)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());

    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
        return;
    }

    mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!mTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

UIImage::~UIImage() {
    if(mTexture){
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

void UIImage::Draw(Game &game, const glm::vec2 &screenPos) {
    if(!mTexture) return;

    SDL_FRect rectImage = {mPosition.x+screenPos.x, mPosition.y+screenPos.y, mSize.x, mSize.y};
    const auto transform = game.GetCamera().get_total_transformation_matrix(game);
    const auto transformed_rect = rect_transform(rectImage, transform);

    SDL_RenderCopyF(game.GetRenderer(), mTexture, nullptr, &transformed_rect);
}