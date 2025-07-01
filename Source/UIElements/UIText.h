//
// Created by Lucas N. Ferreira on 22/05/25.
//

#pragma once

#include <string>
#include <SDL.h>
#include "../Math.h"
#include "UIElement.h"

class Game;

class UIText : public UIElement {
public:
    UIText(const std::string& text, class UIFont* font, int pointSize = 40, const unsigned wrapLength = 1024,
           const glm::vec2 &pos = glm::vec2(.0f), const glm::vec2 &size = glm::vec2(100.f, 20.0f), const glm::vec3& color = Color::White);

    ~UIText();

    void SetText(const std::string& name);
    void Draw(Game &game, const glm::vec2 &screenPos) override;

protected:
    std::string mText;
    class UIFont* mFont;
    SDL_Texture *mTextTexture;

    unsigned int mPointSize;
    unsigned int mWrapLength;
};
