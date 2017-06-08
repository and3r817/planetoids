#pragma once

#ifndef _NOISE_HPP
#define _NOISE_HPP

#include "Platform.hpp"
#include "Resource.hpp"

namespace planetoids
{
    float simplexNoise1(glm::vec2 & coord, unsigned int octaves);
    float simplexNoise2(glm::vec2 & coord, float size);
}

#endif
