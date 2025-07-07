// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "../Math.h"
#include "UIText.h"
#include "UIButton.h"
#include "UIImage.h"

class Game;

class UIScreen
{
public:
    // Tracks if the UI is active or closing
    enum class UIState
    {
        Active,
        Closing
    };

	UIScreen(class Game* game, const std::string& fontName);
	virtual ~UIScreen();

	// UIScreen subclasses can override these
	virtual void Update(float deltaTime);
	virtual void Draw(Game &game);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

    // Set state to closing
	void Close();

	void clear();

    // Get state of UI screen
	UIState GetState() const { return mState; }

    // Game getter
    class Game* GetGame() { return mGame; }

    // Add a button to this screen
	UIButton* AddButton(const std::string& name, const glm::vec2& pos, const glm::vec2& dims, std::function<void()> onClick);
    UIText* AddText(const std::string& name, const glm::vec2& pos, const glm::vec2& dims, const int pointSize = 40, const int unsigned wrapLength = 1024);
    UIImage* AddImage(const std::string& imagePath, const glm::vec2& pos, const glm::vec2& dims, const glm::vec3& color = Color::White);

protected:
    // Sets the mouse mode to relative or not
	class Game* mGame;
	class UIFont* mFont;

	// Configure positions
	glm::vec2 mPos;
	glm::vec2 mSize;

	// State
	UIState mState;

	// List of buttons, texts, and images
    int mSelectedButtonIndex;
	std::vector<UIButton *> mButtons;
    std::vector<UIText *> mTexts;
    std::vector<UIImage *> mImages;
};
