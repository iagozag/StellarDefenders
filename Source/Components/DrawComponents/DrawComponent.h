//
// Created by Lucas N. Ferreira on 03/08/23.
//

#pragma once
#include "../Component.h"
#include "../../Color.h"
#include "../../Math.h"
#include <vector>
#include <SDL.h>

class DrawComponent : public Component
{
public:
    explicit DrawComponent(class Actor* owner, int drawOrder = 100);
    ~DrawComponent() override;

    virtual void Draw(SDL_Renderer* renderer) = 0;

    bool IsVisible() const { return mIsVisible; }
    void SetIsVisible(const bool isVisible) { mIsVisible = isVisible; }
    int GetDrawOrder() const { return mDrawOrder; }

protected:
    bool mIsVisible;
    int mDrawOrder;
};
