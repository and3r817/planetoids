#pragma once

#ifndef _CUBEMAP_HPP
#define _CUBEMAP_HPP

#include "Error.hpp"
#include "Platform.hpp"
#include "Resource.hpp"

namespace planetoids
{
    class Cubemap : public Resource
    {
        public:
            Cubemap(std::string fileName);
            ~Cubemap();

			void bind();

			void unbind();
    };
}

#endif
