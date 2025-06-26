//
// Created by csxuser on 22/06/2025.
//

#include "Alien.h"
//
// Created by Lucas N. Ferreira on 03/08/23.
//


Alien::Alien(Game* game, const float forwardSpeed, const float jumpSpeed):
    Actor(game),
    mForwardSpeed(forwardSpeed),
    mJumpSpeed(jumpSpeed),
    mIsRunning(false),
    mIsDead(false)
{
    // --------------
    // TODO - PARTES 1 e 4
    // --------------

    // TODO 3 (~1 linha): Até que você termine a implementação do componente de animação `DrawAnimatedComponent`,
    //  utilize o componente para desenho de sprites estáticos `DrawSpriteComponent`. Crie um desses
    //  componentes com a textura `Assets/Sprites/Mario/Idle.png` e dimensões (Game::TILE_SIZE, Game::TILE_SIZE).

    //new DrawSpriteComponent(this, "../Assets/Sprites/Mario/Idle.png",Game::TILE_SIZE, Game::TILE_SIZE);

    // --------------
    // TODO - PARTES 2 e 3
    // --------------

    // TODO 2 (~1 linha): Para que o jogador possa se mover, crie também um componente `RigidBodyComponent`
    //  com massa `1.0` e coeficiente de atrito `5.0`. Note que a classe `Mario` já tem um atributo mRigidBodyComponent
    //  para armazenar esse componente. Utilize a função `SetApplyGravity` para desabilitar temporariamente a gravidade
    //  no personagem.
    mRigidBodyComponent = new RigidBodyComponent(this, 1,5, true);
    mRigidBodyComponent->SetApplyGravity(true);

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 8 (~1 linhas): Crie um componente AABBColliderComponent passando as dimensões
    //  (0,0,Game::TILE_SIZE - 4.0f, Game::TILE_SIZE) e o tipo ColliderLayer::Player da AABB. Se quiser desenhar
    //  a AABB do Mario para testar as colisões, crie um componente `DrawPolygonComponent` com os vértices da AABB.
    //  Armazene o ponteiro desse componente no atributo `mColliderComponent` da classe Mario.

    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f, Game::TILE_SIZE, ColliderLayer::Player);

    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 4.1 (~1 linhas): Crie um componente DrawAnimatedComponent passando os caminhos da imagem (.png) e
    //  dos dados (.json) do sprite sheet do Mário que você criou com o FreeTexturePacker. Armaze o ponteiro
    //  desse componente no atributo `mDrawComponent` da classe Mario.
    mDrawComponent = new DrawAnimatedComponent(this,  "../Assets/Sprites/ET/texture.png", "../Assets/Sprites/ET/texture.json",200);



    // TODO 4.2 (~4 linhas): Utilize a função `AddAnimation` para adicionar as animações "dead", "idle",
    //  "jump" e "run".

    mDrawComponent->AddAnimation("idle", std::vector<int>({6}));
    mDrawComponent->AddAnimation("run", std::vector<int>({4,0,2,11,8,5}));


    // TODO 3.4 (~2 linhas): Utilize a função `SetAnimation` para definir a animação inicial como "idle". Em seguida,
    //  utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação para 10.0f.
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(5.0f);
}

void Alien::OnProcessInput(const uint8_t* state)
{
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 3.1 (~2 linhas): Verifique se o jogador pressionou a tecla `d`. Se sim, aplique uma força para
    //  a direita com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `0`. Além disso, altere
    //  a variável `mIsRunning` para verdadeiro. Isso será importante para controlar as animações na Parte 4.

    if (state[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->ApplyForce(glm::vec2(mForwardSpeed,0));
        mRotation = 0.0f;
        mIsRunning = true;
    }

    // TODO 3.2 (~2 linhas): Verifique se o jogador pressionou a tecla `a`. Se sim, aplique uma força para a
    //  esquerda com magnitude `mForwardSpeed` e altere a rotação `mRotation` para `Math::Pi`. Além disso,
    //  altere a variável `mIsRunning` para verdadeiro.


    if (state[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->ApplyForce(glm::vec2(-mForwardSpeed, 0.0f));
        mRotation = Math::Pi;
        mIsRunning = true;
    }
    // TODO 3.3 (~2 linhas): Caso o jogador não estiver pressionando nem `d`, nem `a`,
    //  altere essa variável para falso. Isso será importante para controlar as animações na Parte 4.

    if (!state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D])
    {
        mIsRunning = false;
    }
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 9 (~3 linhas): Verifique se o jogador está no chão (`mIsOnGround`) e se a tecla `space` foi pressionada.
    //  Se sim, aplique uma força vertical com magnitude `mJumpSpeed` e altere a variável `mIsOnGround` para falso.
}


void Alien::OnUpdate(float deltaTime)
{

    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 4 (~2 linhas): Para evitar que o jogador ultrapasse o limite inferior (esquerdo) da câmera,
    //  limite sua posição horizontal para ser sempre maior ou igual a posição horizontal da câmera.

    mPosition.y = std::min(mPosition.y, (float)(mGame->GetWindowHeight()-171));

    if (mPosition.x > 2800) mGame->SetGameScene(Game::GameScene::Level1, Game::TRANSITION_TIME);

    ManageAnimations();
}

void Alien::ManageAnimations()
{
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 5.1 (~7-10 linhas): para implementar a troca de animação, basta utilizar os atributos `mIsDead` para
    //  verificar se o jogador está morto, `mIsOnGround` se o jogador está no chão e `mIsRunning` se o jogador
    //  está correndo.
    //  -5.1.1: Se estiver vivo, no chão e correndo, altere a animação para `run`
    //  -5.1.2: Se estiver vivo, no chão e não estiver correndo, altere a animação para `idle`
    //  -5.1.3: Se estiver vivo e não estiver no chão, altere a animação para `jump`

    if (!mIsDead && mIsRunning) {
        mDrawComponent->SetAnimation("run");
        mDrawComponent->SetAnimFPS(10.0f);
    }
    if (!mIsDead && !mIsRunning) mDrawComponent->SetAnimation("idle");

}

void Alien::Kill()
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 8 (~4 linhas): altere a animação para "dead" e o valor da variável `mIsDead` para verdadeiro.
    //  Além disso, desabilite os componentes `mRigidBodyComponent` e `mColliderComponent`

    mDrawComponent->SetAnimation("dead");
    mIsDead = true;
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);

}

void Alien::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 9 (~2-4 linhas): Verifique se a colisão ocorreu horizontalmente com um objeto do tipo
    //  `CollisionSide::Enemy`. Se sim, mate o jogador com o método `Kill()` do jogador.
    if (other->GetLayer() == ColliderLayer::Enemy) {
        Kill();
    }
}

void Alien::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 10 (~2-4 linhas): Verifique se a colisão ocorreu verticalmente com um objeto do tipo
    //  `CollisionSide::Enemy`. Se sim, mate o inimigo com o método `Kill()` do inimigo e aplique
    //  um meio pulo no jogador (ou seja, altere a velocidade vertical do jogador para `mJumpSpeed / 2`).
    if (other->GetLayer() == ColliderLayer::Enemy) {
        other->GetOwner()->Kill();
        mRigidBodyComponent->SetVelocity(glm::vec2(0.0f, mJumpSpeed/2));
    }

}
