//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

DrawAnimatedComponent::DrawAnimatedComponent(class Actor* owner, const std::string &spriteSheetPath, const std::string &spriteSheetData, glm::vec2 scale, int drawOrder)
        :DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder),
         mScale(scale)
{
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent() {
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string& texturePath, const std::string& dataPath)
{
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    std::ifstream spriteSheetFile(dataPath);
    if (!spriteSheetFile.is_open()) {
        return;
    }

    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    mSpriteSheetData.clear();
    for(const auto& frame : spriteSheetData["frames"]) {
        SDL_Rect rect;
        rect.x = frame["frame"]["x"].get<int>();
        rect.y = frame["frame"]["y"].get<int>();
        rect.w = frame["frame"]["w"].get<int>();
        rect.h = frame["frame"]["h"].get<int>();
        
        mSpriteSheetData.emplace_back(rect);
    }
}

void DrawAnimatedComponent::Draw(SDL_Renderer* renderer)
{
    int spriteIdx = mAnimations[mAnimName][static_cast<int>(mAnimTimer)];
    
    SDL_Rect* srcRect = &mSpriteSheetData[spriteIdx];

    float ratio = (float)srcRect->w/(float)srcRect->h;

    SDL_FRect dstRect = {
            mOwner->GetPosition().x,
            mOwner->GetPosition().y,
            ratio*mScale.x,
            ratio*mScale.y
    };

    SDL_RendererFlip flip = (mOwner->GetRotation() == Math::Pi) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_SetTextureBlendMode(mSpriteSheetSurface, SDL_BLENDMODE_BLEND);

    const auto transform = mOwner->GetGame()->GetCamera().get_total_transformation_matrix(*mOwner->GetGame());
    const auto transformed_dest = rect_transform(dstRect, transform);

    SDL_RenderCopyExF(renderer, mSpriteSheetSurface, srcRect, &transformed_dest, mOwner->GetRotation(), nullptr, flip);
}

void DrawAnimatedComponent::Update(float deltaTime)
{
    if (mIsPaused) {
        return;
    }

    mAnimTimer += mAnimFPS * deltaTime;
    if (mAnimTimer >= mAnimations[mAnimName].size()) {
        while (mAnimTimer >= mAnimations[mAnimName].size()) {
            mAnimTimer -= mAnimations[mAnimName].size();
        }
    }
}

void DrawAnimatedComponent::SetAnimation(const std::string& name)
{
    mAnimName = name;
    Update(0.0f);
}

void DrawAnimatedComponent::AddAnimation(const std::string& name, const std::vector<int>& spriteNums)
{
    mAnimations.emplace(name, spriteNums);
}

