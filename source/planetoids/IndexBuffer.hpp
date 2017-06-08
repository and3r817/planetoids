#pragma once

#ifndef _INDEXBUFFER_HPP
#define _INDEXBUFFER_HPP

#include "BufferObject.hpp"
#include "Error.hpp"
#include "Platform.hpp"

namespace planetoids
{
    class IndexBuffer : public BufferObject
    {
        public:
            IndexBuffer();
            virtual ~IndexBuffer();

            void bind();
            void unbind();

            void setData(unsigned int count, unsigned int size, const void * ptr);

            unsigned int elementsCount();

        private:
            unsigned int m_Count;
    };
}

#endif
