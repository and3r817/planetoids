#pragma once

#ifndef _FRAMEBUFFER_HPP
#define _FRAMEBUFFER_HPP

#include "BufferObject.hpp"
#include "Error.hpp"
#include "Platform.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
    class FrameBuffer : public BufferObject
    {
        public:
            FrameBuffer();
            virtual ~FrameBuffer();

            void bind();
            void unbind();

			void checkStatus();

            void attachTexture(std::shared_ptr<Texture2D> & texture, unsigned int attachmentType);
			void detachTexture(unsigned int attachmentType);

			void setDrawBuffer(unsigned int drawMode);
			void setReadBuffer(unsigned int readMode);
    };
}

#endif
