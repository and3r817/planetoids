#include "IndexBuffer.hpp"

namespace planetoids
{
    IndexBuffer::IndexBuffer() : BufferObject()
    {
        OPENGL_CALL(glGenBuffers(1, &m_Id));
    }


    IndexBuffer::~IndexBuffer()
    {
        unbind();

        OPENGL_CALL(glDeleteBuffers(1, &m_Id));
    }


    void IndexBuffer::bind()
    {
        OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
    }


    void IndexBuffer::unbind()
    {
        OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }


    void IndexBuffer::setData(unsigned int count, unsigned int size, const void * ptr)
    {
        bind();

        m_Count = count;
        OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW));

        unbind();
    }


    unsigned int IndexBuffer::elementsCount()
    {
        return m_Count;
    }
}