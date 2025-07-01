
#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

SDL_FRect rect_transform(const SDL_FRect &rect, const glm::mat4 &transform_matrix);
