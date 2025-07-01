//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIText.h"
#include "UIFont.h"
#include "../Game.h"
#include "../rect_transform.hpp"

UIText::UIText(
    const std::string &text, class UIFont* font, int pointSize, const unsigned wrapLength,
    const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color
):
    UIElement(pos, size, color),
    mFont(font),
    mTextTexture(nullptr),
    mPointSize(pointSize),
    mWrapLength(wrapLength)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Utilize o método SetText para definir o texto inicial do UIText.
    SetText(text);
}

UIText::~UIText()
{

}

void UIText::SetText(const std::string &text)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Verifique se a textura atual mTextTexture já foi inicializada. Se sim, destrua-a com SDL_DestroyTexture
    //  e defina mTextTexture como nullptr. Caso contrário, siga para o próximo passo.
    if(mTextTexture){
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = nullptr;
    }

    // TODO 2.: Crie a textura de texto usando o método RenderText do mFont, passando o texto, a cor, o tamanho do
    //  ponto e o comprimento de quebra. Armazene a textura resultante em mTextTexture e a nova string em mText.
    mTextTexture = mFont->RenderText(text, mColor, mPointSize, mWrapLength);
    mText = text;
}

void UIText::Draw(Game &game, const glm::vec2 &screenPos) {
    SDL_FRect titleQuad = {mPosition.x+screenPos.x, mPosition.y+screenPos.y, mSize.x, mSize.y};
    const auto transform = game.GetCamera().get_total_transformation_matrix(game);
    const auto transformed_quad = rect_transform(titleQuad, transform);

    SDL_RenderCopyExF(game.GetRenderer(), mTextTexture, nullptr, &transformed_quad, 0.0, nullptr, SDL_FLIP_NONE);
}
