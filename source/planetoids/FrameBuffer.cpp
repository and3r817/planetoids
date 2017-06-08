#include "FrameBuffer.hpp"

namespace planetoids
{
    FrameBuffer::FrameBuffer() : BufferObject()
    {
        OPENGL_CALL(glGenFramebuffersEXT(1, &m_Id));

        unbind();
    }


    FrameBuffer::~FrameBuffer()
    {
        OPENGL_CALL(glDeleteFramebuffersEXT(1, &m_Id));
    }


    void FrameBuffer::bind()
    {
        OPENGL_CALL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_Id));
    }


    void FrameBuffer::unbind()
    {
        OPENGL_CALL(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
    }


    void FrameBuffer::checkStatus()
    {
        bind();

        unsigned int status = 0;
		OPENGL_CALL(status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));

        if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
        {
            switch(status)
            {
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
                    printf("FBO error: Incomplete FBO attachment.\n");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
                    printf("FBO error: Incomplete missing FBO attachment.\n");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
                    printf("FBO error: Incomplete FBO draw buffer.\n");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
                    printf("FBO error: Incomplete FBO read buffer.\n");
                    break;

                case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
                    printf("FBO error: Unsupported internal format.\n");
                    break;
            }
        }

        unbind();
    }


    void FrameBuffer::attachTexture(std::shared_ptr<Texture2D> & texture, unsigned int attachmentType)
    {
        bind();

        OPENGL_CALL(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentType, GL_TEXTURE_2D, texture->getId(), 0));

        unbind();
    }


    void FrameBuffer::detachTexture(unsigned int attachmentType)
    {
        bind();

        OPENGL_CALL(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentType, GL_TEXTURE_2D, 0, 0));

        unbind();
    }


    void FrameBuffer::setDrawBuffer(unsigned int drawMode)
    {
        bind();

        OPENGL_CALL(glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_Id));
        OPENGL_CALL(glDrawBuffer(drawMode));

        unbind();
    }


    void FrameBuffer::setReadBuffer(unsigned int readMode)
    {
        bind();

        OPENGL_CALL(glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_Id));
        OPENGL_CALL(glReadBuffer(readMode));

        unbind();
    }
}