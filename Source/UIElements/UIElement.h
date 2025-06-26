//
// Created by Lucas N. Ferreira on 22/05/25.
//

#pragma once

#include "../Math.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include "../Color.h"

class UIElement {
public:
    UIElement(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color);
    virtual ~UIElement() = default;

    // Getters/setters
    const glm::vec2& GetPosition() const { return mPosition; }
    void SetPosition(const glm::vec2 &pos) { mPosition = pos; }

    const glm::vec2& GetSize() const { return mSize; }
    void SetSize(const glm::vec2 &size) { mSize = size; }

    const glm::vec3& GetColor() const { return mColor; }
    void SetColor(const glm::vec3 &color) { mColor = color; }

    virtual void Draw(SDL_Renderer* renderer, const glm::vec2 &screenPos) {};

protected:
    glm::vec2 mPosition;
    glm::vec2 mSize;
    glm::vec3 mColor;
};
