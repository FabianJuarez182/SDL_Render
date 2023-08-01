#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "color.h"
#include <SDL2/SDL.h>

void drawTriangle(SDL_Renderer* renderer, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const Color& currentColor);

#endif // TRIANGLE_H
