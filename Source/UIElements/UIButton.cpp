//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIButton.h"

#include "../Game.h"
#include "../rect_transform.hpp"

UIButton::UIButton(const std::string& text, class UIFont* font, std::function<void()> onClick,
                    const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color,
                    int pointSize , unsigned wrapLength,
                    const glm::vec2 &textPos, const glm::vec2 &textSize, const glm::vec3& textColor):
    UIElement(pos, size, color),
    mOnClick(onClick),
    mText(text, font, pointSize, wrapLength, textPos, textSize, textColor),
    mHighlighted(false)
{

}

UIButton::~UIButton()
{

}


void UIButton::Draw(Game &game, const glm::vec2 &screenPos) {
    auto renderer = game.GetRenderer();
    
    SDL_FRect titleQuad = {mPosition.x+screenPos.x, mPosition.y+screenPos.y, mSize.x, mSize.y};
    const auto transform_matrix = game.GetCamera().get_total_transformation_matrix(game);
    const auto transformed = rect_transform(titleQuad, transform_matrix);

    if(mHighlighted) SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255), SDL_RenderFillRectF(renderer, &transformed);    
    mText.Draw(game, mPosition+(mSize*0.5f)-(mText.GetSize()*0.5f));
}

void UIButton::OnClick()
{
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Verifique se o ponteiro mOnClick não é nulo. Se não for, chame a função mOnClick().
    if(mOnClick) mOnClick();
}
