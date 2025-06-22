//
// Created by Lucas N. Ferreira on 10/09/23.
//

#define DB_PERLIN_IMPL

#include "Star.h"
#include "../Game.h"
#include "../Random.h"
// #include "../Actors/Laser.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Star::Star(Game* game, const float radius, const int numVertices, const float forwardForce)
        :Actor(game)
        ,mRigidBodyComponent(nullptr)
        ,mDrawSpriteComponent(nullptr)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // Create a circle with numVertices
    std::vector<Vector2> vertices = GenerateVertices(numVertices, radius);

    // Calculate average vertices length to be the collider radius
    float averageLength = CalculateAverageVerticesLength(vertices);

    // Generate random starting force
    Vector2 randStartingForce = GenerateRandomStartingForce(0.0f, 0.0f);

    // TODO 1.2 (~3 linhas): Utilize a função Random::GetVector para gerar uma posição aleatória inicial para
    //  o asteroide. Garanta que essa posição inicial não resultará em uma colisão com a configuração inicial
    //  da nave. Utilize a função SetPosition para alterar a posição inicial do asteroide com a posição gerada.
    Vector2 pos;
    do{
        pos = Random::GetVector(Vector2::Zero, Vector2(mGame->GetWindowWidth(), mGame->GetWindowHeight()));
    } while((pos - mGame->GetShip()->GetPosition()).Length() < radius + mGame->GetShip()->GetComponent<CircleColliderComponent>()->GetRadius());

    SetPosition(pos);

    // TODO 1.3 (3 linhas): Instancie os componentes DrawComponent, RigidBodyComponent e CircleColliderComponent.
    //  Armazene esses componentes nos ponteiros mDrawComponent, mRigidBodyComponent e mCircleColliderComponent,
    //  respectivamente. O container de vértices criado na etapa anterior será passado como parâmetro para o
    //  DrawComponente. E, para o CircleColliderComponent, passe a média dos comprimentos dos vértices gerados
    //  (averageLength) como raio de colisão.
    mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/earth.png");
    mRigidBodyComponent = new RigidBodyComponent(this);
    mCircleColliderComponent = new CircleColliderComponent(this, averageLength);

    // TODO 1.4 (1 linha): Aplique a força aleatória gerada anteriormente (randStartingForce) para mover o
    //  asteroide. Utilize a função ApplyForce do componente mRigidBodyComponent
    mRigidBodyComponent->ApplyForce(forwardForce*randStartingForce);

    // TODO 1.5 (1 linha): Adicione (game->AddStar()) esse asteroide (this) à lista de asteroides do jogo.
    mGame->AddStar(this);
}

Star::~Star()
{
    // --------------
    // TODO - PARTE 3
    // --------------

    for(int i = 0; i < 3; i++){
        auto ast = new Star(mGame, std::max(10.0f, mCircleColliderComponent->GetRadius()/3.0f), Random::GetIntRange(5, 10), 4.0f);
        ast->SetPosition(mPosition);
    }

    // for(int i = 0; i < 20; i++){
    //     auto particle = new Laser(mGame, 1.0f, 0.5f, false);

    //     particle->SetPosition(mPosition);

    //     float randomAngle = Random::GetFloatRange(0.0f, Math::TwoPi);
    //     particle->SetRotation(randomAngle);

    //     Vector2 direction = Vector2(Math::Cos(randomAngle), Math::Sin(randomAngle));
    //     particle->GetComponent<RigidBodyComponent>()->ApplyForce(direction*2000.0f);
    // }

    // TODO 4.1 (1 linha): Remova (game->RemoveStar()) esse asteroide (this) da lista de asteroides do jogo.
    mGame->RemoveStar(this);
}


std::vector<Vector2> Star::GenerateVertices(const int numVertices, const float radius)
{
    // Gerar um conjunto de vértices em uma circunferência, adicionando um pequeno ruído a cada um deles.

    std::vector<Vector2> vertices;

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.1 (1 linha): Inicialize variável angle (float) com zero. Ela será
    //  utilizada para percorrer o arco de uma circunferência em intervalos angulares de tamanho fixo.
    float angle = .0f;

    // TODO 5.2 (1 linha): Repita o seguinte procedimento para um dado número de vértices (numVertices):
    //  (a) Gere um número real entre 0.5 e 1.0 e multiplique pelo raio da circunferência (radius). Armazene o
    //  resultado em uma variável randLength;
    //  (b) Calcule a coordenada x do novo vértice multiplicando randLength pelo cosseno do ângulo corrente (angle);
    //  (b) Calcule a coordenada y da mesma forma, porém multiplicando pelo seno do ângulo corrente;
    //  (c) Adicione o vetor (x,y) ao conjunto de vértices (vertices);
    //  (d) Incremente o ângulo corrente por 2*PI dividido pelo número de vértices (numVertices).
    for(int i = 0; i < numVertices; i++){
        float randLength = Random::GetFloatRange(0.5, 1.0)*radius;
        Vector2 v(randLength*cos(angle), randLength*sin(angle));
        vertices.emplace_back(v);
        angle += 2.0*Math::Pi/numVertices;
    }

    return vertices;
}

float Star::CalculateAverageVerticesLength(std::vector<Vector2>& vertices)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 6.1 (~4 linhas): Calcule e retorne a média de comprimento dos vértices em (vertices) .
    float mean = .0f;
    for(auto v: vertices) mean += v.Length();
    mean /= (float)(vertices.size());

    return mean;
}

Vector2 Star::GenerateRandomStartingForce(const float min, const float max)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 7.1 (~3 linhas): Escolha aleatóriamente (com probabilidade uniforme) uma direção horizontal (-1.0 ou 1.0).
    //  Dica: utilize a função Random::GetFloat() e verifique se o resultado é menor do que 0.5. Se for, a direção
    //  é 1.0, se não é -1.0. Armazene o resultado em uma variável randDirX.
    float randDirX = (Random::GetFloat() <= 0.5f ? -1.0f : 1.0f);

    // TODO 7.2 (~3 linhas): Escolha aleatóriamente (com probabilidade uniforme) uma direção vertical (-1.0 ou 1.0).
    //  Dica: utilize a função Random::GetFloat() e verifique se o resultado é menor do que 0.5. Se for, a direção
    //  é 1.0, se não é -1.0. Armazene o resultado em uma variável randDirY.
    float randDirY = (Random::GetFloat() <= 0.5f ? -1.0f : 1.0f);

    // TODO 7.3 (~3 linhas): Gere um vetor aleatório chamado randForce com a função Random::GetVector. Utilize os
    //  parâmetros min e max desse método para restringir os limites inferior e superior do vetor.
    Vector2 minForce(min, min);
    Vector2 maxForce(max, max);
    Vector2 randForce = Random::GetVector(minForce, maxForce);

    // TODO 7.4 (~3 linhas): Multiplique a componente horizontal de randForce por randDirX e a vertical por randDirY.
    //  Depois, retorn randForce.
    randForce.x *= randDirX;
    randForce.y *= randDirY;

    return randForce;
}