#pragma once

#ifndef _VERTEX_HPP
#define _VERTEX_HPP

#include "Platform.hpp"

namespace planetoids
{
    typedef struct
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    } Vertex;

    typedef struct
    {
        glm::vec3 position;
        glm::vec2 texcoord;
		//glm::vec3 normal;
        glm::vec3 parent;
    } TerrainVertex;
}

#endif
