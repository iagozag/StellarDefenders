//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawSpriteComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor* owner, const std::string &texturePath, const int width, const int height, const int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
{
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);
}

DrawSpriteComponent::~DrawSpriteComponent()
{
    DrawComponent::~DrawComponent();

    if (mSpriteSheetSurface) {
        SDL_DestroyTexture(mSpriteSheetSurface);
        mSpriteSheetSurface = nullptr;
    }
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer, const glm::vec3 &modColor)
{
    SDL_FRect dstRect = {
        mOwner->GetPosition().x - mOwner->GetGame()->GetCamera().m_pos.x,
        mOwner->GetPosition().y - mOwner->GetGame()->GetCamera().m_pos.y,
        mWidth,
        mHeight
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (mOwner->GetRotation() == Math::Pi) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_SetTextureBlendMode(mSpriteSheetSurface, SDL_BLENDMODE_BLEND);

    SDL_SetTextureColorMod(mSpriteSheetSurface,
                           static_cast<Uint8>(modColor.x),
                           static_cast<Uint8>(modColor.y),
                           static_cast<Uint8>(modColor.z));

    const auto transform = GetGame()->GetCamera().get_total_transformation_matrix(*GetGame());
    const auto transformed_dest = rect_transform(dstRect, transform);

    SDL_RenderCopyExF(renderer, mSpriteSheetSurface, nullptr, &transformed_dest, mOwner->GetRotation(), nullptr, flip);
}
