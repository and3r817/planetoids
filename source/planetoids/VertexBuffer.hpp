#pragma once

#ifndef _VERTEXBUFFER_HPP
#define _VERTEXBUFFER_HPP

#include "BufferObject.hpp"
#include "Error.hpp"
#include "Platform.hpp"

namespace planetoids
{
    class VertexBuffer : public BufferObject
    {
        public:
            VertexBuffer();
            virtual ~VertexBuffer();

            void bind();
            void unbind();

            void setData(unsigned int count, unsigned int size, const void * ptr);

            unsigned int elementsCount();

        private:
            unsigned int m_Count;
    };
}

#endif
