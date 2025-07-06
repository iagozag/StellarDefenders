//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "DrawComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawComponent::DrawComponent(class Actor* owner, int drawOrder):
    Component(owner),
    mIsVisible(true),
    mDrawOrder(drawOrder)
{}

DrawComponent::~DrawComponent()
{}
