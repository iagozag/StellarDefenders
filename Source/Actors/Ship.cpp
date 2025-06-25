//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Ship.h"
#include "Star.h"
#include "../Game.h"
// #include "../Actors/Laser.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Ship::Ship(Game* game,
           const float height,
           const float forwardForce,
           const float rotationForce,
           const float frictionCoefficient)
        : Actor(game)
        , mLaserCooldown(0.f)
        , mHeight(height)
        , mRigidBodyComponent(nullptr)
        , mDrawSpriteComponent(nullptr)
        , mForwardSpeed(forwardForce)
        , mRotationForce(rotationForce)
        , mFrictionCoefficient(frictionCoefficient)
        , mShipState(ShipState::Positioning)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (~3 linhas): Crie 3 vértices (Vector2) considerando o centro da nave como origem e o
    //  atributo mHeight como altura do triângulo. Por exemplo: v1 = (-h,h/2), v2 = (h, 0) e v3 = (-h/2)
    Vector2 v1(-mHeight, mHeight/2), v2(mHeight, 0), v3(-mHeight, -mHeight/2);

    // TODO 1.2 (~4 linhas): Adicione esses 3 vértices end um container std::vector.
    std::vector<Vector2> vertices = {v1, v2, v3};

    // TODO 1.3 (3 linhas): Instancie os componentes DrawComponent, RigidBodyComponent e CircleColliderComponent.
    //  Armazene esses componentes nos ponteiros mDrawComponent, mRigidBodyComponent e mCircleColliderComponent,
    //  respectivamente. O container de vértices criado na etapa anterior será passado como parâmetro para o
    //  DrawComponente. E, para o CircleColliderComponent, passe a metade da altura da nave como raio de colisão.
    mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Idle.png", mHeight, mHeight);
    mRigidBodyComponent = new RigidBodyComponent(this, 1, 0, false);
    mCircleColliderComponent = new CircleColliderComponent(this, mHeight/2);
}

void Ship::OnProcessInput(const uint8_t* state)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2.1 (~4 linhas): Verifique se o jogador está pressionando a tecla W e, se estiver,
    //  aplique uma força para frente com magnitude dada pelo atributo mForwardSpeed. Utilize o método
    //  GetForward() para obter o vetor da frente e a função ApplyForce do componente mRigidBodyComponent
    //  para aplicar a força.
    if(state[SDL_SCANCODE_W]) mRigidBodyComponent->ApplyForce(GetForward()*mForwardSpeed);

    // TODO 2.2 (~4 linhas): Inicialize uma variável local angularSpeed com zero e verifique se o jogador
    //  está pressionando a tecla A. Se estiver, some à velocidade angular (angularSpeed) a velocidade de
    //  rotação mRotationForce.
    float angularSpeed = .0f;
    if(state[SDL_SCANCODE_A]) angularSpeed += mRotationForce;

    // TODO 2.3 (~4 linhas): Verifique se o jogador está pressionando a tecla D. Se estiver,
    //  subtraia da velocidade angular (angularSpeed) a velocidade de rotação (mRotationForce).
    if(state[SDL_SCANCODE_D]) angularSpeed -= mRotationForce;

    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mShipState == ShipState::Positioning and (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)))
        mShipState = ShipState::SlingShot;
    else if(mShipState == ShipState::SlingShot and mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)){
        mShipState = ShipState::Ready;
        mDirection = mPosition-mSlingShotPoint;
    }
}

void Ship::OnUpdate(float deltaTime)
{
    if(mShipState == ShipState::Positioning){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX,&mouseY);
        SetPosition(Vector2(mouseX, mouseY));
    }

    // TODO 3.4 (~6 linhas): Percorra a lista de asteroides do jogo e verifique, para cada asteroide,
    //  se ele está colidindo com a nave. Lembre-se que o método GetGame()->GetAsteroids() retorna a lista
    //  de asteroides. Além disso, você já implementou o método Intersect do CircleColliderComponent. Tanto a
    //  nave quanto os asteroides possuem esse componente, então basta utilizar essa função para verificar
    //  a colisão. Se houver colisão da nave com algum asteroide, termine o jogo (GetGame()->Quit()).
    for(auto star: GetGame()->GetStars()){
        auto starCollider = star->GetComponent<CircleColliderComponent>();

        if(mCircleColliderComponent->Intersect(*starCollider)){
            // mGame->Quit();
        }
    }
}
