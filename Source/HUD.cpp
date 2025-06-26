//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "HUD.h"
#include "Game.h"
#include "UIElements/UIText.h"

HUD::HUD(class Game* game, const std::string& fontName)
    :UIScreen(game, fontName)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 4.: Adicione um texto com a string "1-1" logo abaixo do texto "World".
    mLevelName = AddText("Fase 1", Vector2(mGame->GetWindowWidth() - (CCHAR_WIDTH * 6 + WORD_OFFSET) + CCHAR_WIDTH, HUD_POS_Y), Vector2(CCHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE, 1024);
}

HUD::~HUD()
{

}

void HUD::SetLevelName(const std::string &levelName)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Utilize o método SetText() do mLevelName para atualizar o texto com o nome do nível.
    mLevelName->SetText(levelName);
}