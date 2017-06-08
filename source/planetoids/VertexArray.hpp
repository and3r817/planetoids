#pragma once

#ifndef _VERTEXARRAY_HPP
#define _VERTEXARRAY_HPP

#include "Platform.hpp"

namespace planetoids
{
    class VertexArray
    {
        public:
            VertexArray();
            virtual ~VertexArray();

			void bind();

			void unbind();

			bool isOk();

			unsigned int getId();

        private:
            unsigned int m_Id;
    };
}

#endif
