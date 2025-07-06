//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "DrawComponent.h"

class DrawPolygonComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    DrawPolygonComponent(class Actor* owner, std::vector<glm::vec2> &vertices, int drawOrder = 100);

    void Draw(SDL_Renderer* renderer) override;
    std::vector<glm::vec2>& GetVertices() { return mVertices; }
protected:
    int mDrawOrder;
    std::vector<glm::vec2> mVertices;
};