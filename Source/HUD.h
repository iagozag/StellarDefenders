//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>

#include "UIElements/UIScreen.h"

class HUD : public UIScreen
{
public:
    constexpr static int POINT_SIZE = 48;
    constexpr static int WORD_HEIGHT = 20;
    constexpr static int WORD_OFFSET = 25;
    constexpr static int CCHAR_WIDTH = 20;
    constexpr static int HUD_POS_Y = 10;

    HUD(class Game* game, const std::string& fontName);
    ~HUD();

    // Reinsert the HUD elements
    void SetTime(int time);

    void SetLevelName(const std::string& levelName);

private:
    // HUD elements
    UIText* mScoreCounter;
    UIText* mLevelName;
    UIText* mLivesCount;
    UIText* mTimeText;
    UIText* mCoinCountText;
};
