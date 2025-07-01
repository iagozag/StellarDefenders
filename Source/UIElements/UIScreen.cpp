// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "../Game.h"
#include "UIFont.h"

UIScreen::UIScreen(Game* game, const std::string& fontName)
	:mGame(game)
	,mPos(0.f, 0.f)
	,mSize(0.f, 0.f)
	,mState(UIState::Active)
    ,mSelectedButtonIndex(-1)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Adicione a UIScreen à lista de telas de UI do jogo (mGame) com PushUI(this).
    mGame->PushUI(this);

    // TODO 2.: Utilize o método LoadFont do jogo (mGame) para carregar a fonte padrão,
    //  passada como parâmetro (fontName).
    mFont = mGame->LoadFont(fontName);
}

UIScreen::~UIScreen()
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Percorra a listas de textos (mTexts) e delete cada UIText. Limpe a lista ao final do laço.
    for(auto t: mTexts) delete t;
    mTexts.clear();

    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Percorra a lista de botões (mButtons) e delete cada UIButton. Limpe a lista ao final do laço.
    for(auto b: mButtons) delete b;
    mButtons.clear();

    // --------------
    // TODO - PARTE 1-3
    // --------------

    // TODO 1.: Percorra a lista de imagens (mImages) e delete cada UIImage. Limpe a lista ao final do laço.
    for(auto i: mImages) delete i;
    mImages.clear();
}

void UIScreen::Update(float deltaTime)
{
	
}

void UIScreen::Draw(Game &game) {

    for(auto t: mTexts) t->Draw(game, mPos);
    for(auto b: mButtons) b->Draw(game, mPos);
    for(auto i: mImages) i->Draw(game, mPos);
}

void UIScreen::ProcessInput(const uint8_t* keys)
{

}

void UIScreen::HandleKeyPress(int key)
{
    if(mGame->GetGameScene() != Game::GameScene::MainMenu) return;
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Verifique se a tecla pressionada é W (SDLK_w), S (SDLK_s) ou Enter (SDLK_RETURN). Se a tecla for W,
    //  diminua o índice do botão selecionado (mSelectedButtonIndex) e destaque o botão anterior. Se o índice for
    //  menor que 0, defina-o como o último botão da lista. Se a tecla for S, aumente o índice do botão selecionado
    //  e destaque o próximo botão. Se o índice for maior ou igual ao tamanho da lista, defina-o como 0 (o primeiro botão).
    //  Se a tecla for Enter, verifique se o índice do botão selecionado é válido (maior ou igual a 0 e menor que
    //  o tamanho da lista). Se for, chame o método OnClick do botão selecionado.
    if(key == SDLK_UP){
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if(mSelectedButtonIndex < 0) mSelectedButtonIndex = (int)mButtons.size()-1;
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    }
    else if(key == SDLK_DOWN){
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if(mSelectedButtonIndex >= (int)mButtons.size()) mSelectedButtonIndex = 0;
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    }
    else if(key == SDLK_RETURN and mSelectedButtonIndex >= 0 and mSelectedButtonIndex < (int)mButtons.size())
        mButtons[mSelectedButtonIndex]->OnClick();
}

void UIScreen::Close()
{
	mState = UIState::Closing;
}

UIText* UIScreen::AddText(const std::string &name, const glm::vec2 &pos, const glm::vec2 &dims, const int pointSize, const int unsigned wrapLength)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Crie um novo UIText com o nome, fonte (mFont), tamanho do ponto (pointSize), comprimento de quebra (wrapLength),
    //  posição (pos), dimensões (dims) e cor branca. Armazene o ponteiro em uma variável t. A seguir, adicione o UIText
    //  à lista de textos (mTexts) e retorne o ponteiro t.
    mTexts.emplace_back(new UIText(name, mFont, pointSize, wrapLength, pos, dims, glm::vec3(1.0f, 1.0f, 1.0f)));
    return mTexts.back();
}

UIButton* UIScreen::AddButton(const std::string& name, const glm::vec2 &pos, const glm::vec2& dims, std::function<void()> onClick)
{
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Crie um novo UIButton com o nome, fonte (mFont), função de clique (onClick),
    //  posição (pos), dimensões (dims) e cor laranja. Adicione o botão à lista de botões (mButtons).
    mButtons.emplace_back(new UIButton(name, mFont, onClick, pos, dims, glm::vec3(200.0f/255.0f, 100.0f/255.0f, 0.0f/255.0f)));

    // TODO 2.: Se a lista de botões (mButtons) tiver apenas um botão, defina o índice do botão
    //  selecionado (mSelectedButtonIndex) como 0 e destaque o botão (b->SetHighlighted(true)).
    if((int)mButtons.size() == 1) mSelectedButtonIndex = 0, mButtons[0]->SetHighlighted(true);

    // TODO 3.: Retorne o ponteiro do botão criado (b).
    return mButtons.back();
}

UIImage* UIScreen::AddImage(const std::string &imagePath, const glm::vec2 &pos, const glm::vec2 &dims, const glm::vec3 &color)
{
    // --------------
    // TODO - PARTE 1-3
    // --------------

    // TODO 1.: Crie um novo UIImage com o caminho da imagem (imagePath), posição (pos), dimensões (dims) e cor (color).
    //  Armazene o ponteiro em uma variável img. A seguir, adicione o UIImage à lista de imagens (mImages) e retorne
    //  o ponteiro img.
    mImages.emplace_back(new UIImage(mGame->GetRenderer(), imagePath, pos, dims, color));
    return mImages.back();
}