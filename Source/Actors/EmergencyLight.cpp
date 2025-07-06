//
// Created by csxuser on 22/06/2025.
//

// #include "EmergencyLight.h"
// //
// // Created by Lucas N. Ferreira on 03/08/23.
// //
// 
// 
// EmergencyLight::EmergencyLight(Game* game):
//     Actor(game),
// {
//     mDrawComponent = new DrawAnimatedComponent(this,  "../Assets/Sprites/ET/texture.png", "../Assets/Sprites/ET/texture.json",200);
//     mDrawComponent->AddAnimation("idle", std::vector<int>({6}));
//     mDrawComponent->AddAnimation("run", std::vector<int>({4,0,2,11,8,5}));
// 
// 
//     // TODO 3.4 (~2 linhas): Utilize a função `SetAnimation` para definir a animação inicial como "idle". Em seguida,
//     //  utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação para 10.0f.
//     mDrawComponent->SetAnimation("idle");
//     mDrawComponent->SetAnimFPS(5.0f);
// }
// 
// void Alien::OnUpdate(float deltaTime)
// {
//     mPosition.y = std::min(mPosition.y, (float)(mGame->GetWindowHeight()-171));
// 
//     if(mPosition.x <= .0f) mPosition.x = .0f;
//     if (mPosition.x > 2800){
//         mGame->SetGameScene(Game::GameScene::Level1, Game::TRANSITION_TIME);
//     }
// 
//     ManageAnimations();
// }
// 
// void Alien::ManageAnimations()
// {
//     // --------------
//     // TODO - PARTE 4
//     // --------------
// 
//     // TODO 5.1 (~7-10 linhas): para implementar a troca de animação, basta utilizar os atributos `mIsDead` para
//     //  verificar se o jogador está morto, `mIsOnGround` se o jogador está no chão e `mIsRunning` se o jogador
//     //  está correndo.
//     //  -5.1.1: Se estiver vivo, no chão e correndo, altere a animação para `run`
//     //  -5.1.2: Se estiver vivo, no chão e não estiver correndo, altere a animação para `idle`
//     //  -5.1.3: Se estiver vivo e não estiver no chão, altere a animação para `jump`
// 
//     if (!mIsDead && mIsRunning) {
//         mDrawComponent->SetAnimation("run");
//         mDrawComponent->SetAnimFPS(10.0f);
//     }
//     if (!mIsDead && !mIsRunning) mDrawComponent->SetAnimation("idle");
// 
// }