//
// Created by Lucas N. Ferreira on 22/05/25.
//

#pragma once

#include <functional>
#include <string>
#include "UIFont.h"
#include "UIText.h"
#include "UIElement.h"
#include "../Math.h"

class Game;

class UIButton : public UIElement
{
public:
    UIButton(const std::string& text, class UIFont* font, std::function<void()> onClick,
             const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color,
             int pointSize, const unsigned wrapLength,
             const glm::vec2 &textPos,
             const glm::vec2 &textSize,
             const glm::vec3& textColor = Color::White);

    ~UIButton();

    // Set the name of the button
    void SetText(const std::string& text);
    void Draw(Game &game, const glm::vec2 &screenPos) override;

    void SetHighlighted(bool sel) { mHighlighted = sel; }
    bool GetHighlighted() const { return mHighlighted; }

    // Returns true if the point is within the button's bounds
    bool ContainsPoint(const glm::vec2& pt) const;

    // Called when button is clicked
    void OnClick();

private:
    // Callback funtion
    std::function<void()> mOnClick;

    // Button name
    UIText mText;

    // Check if the button is highlighted
    bool mHighlighted;
};