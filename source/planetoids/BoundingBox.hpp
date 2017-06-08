#pragma once

#ifndef _BOUNDINGBOX_HPP
#define _BOUNDINGBOX_HPP

#include "Platform.hpp"

namespace planetoids
{
    typedef struct
    {
        glm::vec3 center;
        glm::vec3 radius;
    } BoundingBox;
}

#endif
