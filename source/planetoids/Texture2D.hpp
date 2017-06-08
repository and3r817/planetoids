#pragma once

#ifndef _TEXTURE2D_HPP
#define _TEXTURE2D_HPP

#include "Error.hpp"
#include "Platform.hpp"
#include "Resource.hpp"

namespace planetoids
{
    class Texture2D : public Resource
    {
        public:
            Texture2D();
            ~Texture2D();

			void setUnit(unsigned int unit);
			unsigned int getUnit();

            void bind();
            void unbind();

            void setFilteringMode(unsigned int filteringMode);
            void setWrapMode(unsigned int wrapMode);
            void setCompareMode(unsigned int compareMode);

	private:
		unsigned int m_Unit;

        public:
            static std::shared_ptr<Texture2D> loadFromFile(std::string fileName);
			static std::shared_ptr<Texture2D> createEmpty(glm::ivec2 & size, unsigned int internalFormat, unsigned int format);
            //static Texture2D * createDepthTexure(glm::ivec2 & size);
    };
}

#endif
