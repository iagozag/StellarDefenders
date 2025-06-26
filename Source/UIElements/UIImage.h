//
// Created by Lucas N. Ferreira on 28/05/25.
//

#pragma once

#include <string>
#include <SDL_image.h>
#include "UIElement.h"

class UIImage :  public UIElement
{
public:
    UIImage(SDL_Renderer* renderer, const std::string &imagePath, const glm::vec2 &pos = glm::vec2(.0f),
            const glm::vec2 &size = glm::vec2(100.f, 100.f), const glm::vec3 &color = Color::White);

    ~UIImage();

    void Draw(SDL_Renderer* renderer, const glm::vec2 &screenPos) override;

private:
    SDL_Texture* mTexture; // Texture for the image
};
