
#include "rect_transform.hpp"

SDL_FRect rect_transform(const SDL_FRect &rect, const glm::mat4 &transform_matrix) {
    // 1. Obtenha os quatro cantos do SDL_FRect original como glm::vec4 (com z=0, w=1)
    glm::vec4 topLeft     = glm::vec4(rect.x,            rect.y,             0.0f, 1.0f);
    glm::vec4 topRight    = glm::vec4(rect.x + rect.w,   rect.y,             0.0f, 1.0f);
    glm::vec4 bottomLeft  = glm::vec4(rect.x,            rect.y + rect.h,    0.0f, 1.0f);
    glm::vec4 bottomRight = glm::vec4(rect.x + rect.w,   rect.y + rect.h,    0.0f, 1.0f);

    // 2. Transforme cada canto pela matriz
    glm::vec4 transformedTopLeft     = transform_matrix * topLeft;
    glm::vec4 transformedTopRight    = transform_matrix * topRight;
    glm::vec4 transformedBottomLeft  = transform_matrix * bottomLeft;
    glm::vec4 transformedBottomRight = transform_matrix * bottomRight;

    // 3. Encontre os valores mínimos e máximos de X e Y entre os cantos transformados
    // Isso define o novo bounding box alinhado aos eixos.
    float minX = std::min({transformedTopLeft.x, transformedTopRight.x, transformedBottomLeft.x, transformedBottomRight.x});
    float minY = std::min({transformedTopLeft.y, transformedTopRight.y, transformedBottomLeft.y, transformedBottomRight.y});
    float maxX = std::max({transformedTopLeft.x, transformedTopRight.x, transformedBottomLeft.x, transformedBottomRight.x});
    float maxY = std::max({transformedTopLeft.y, transformedTopRight.y, transformedBottomLeft.y, transformedBottomRight.y});

    // 4. Crie e retorne o novo SDL_FRect (bounding box) a partir dos valores min/max
    return {minX, minY, maxX - minX, maxY - minY};
}
