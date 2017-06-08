#pragma once

#ifndef _BUFFEROBJECT_HPP
#define _BUFFEROBJECT_HPP

#include "Platform.hpp"

namespace planetoids
{
    class BufferObject
    {
        public:
            BufferObject();
            virtual ~BufferObject();

            unsigned int getId();

        protected:
            unsigned int m_Id;
    };
}

#endif
