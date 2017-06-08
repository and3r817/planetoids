#pragma once

#ifndef _HEIGHTMAP_HPP
#define _HEIGHTMAP_HPP

#include "Platform.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
    class Heightmap
    {
        public:
            Heightmap(std::vector<char> & data, unsigned int size);
            ~Heightmap();

            float getHeightAt(glm::vec2 & coord);

        private:
            std::vector<std::vector<float> > m_Heights;

			unsigned int m_Size;
    };
}

#endif