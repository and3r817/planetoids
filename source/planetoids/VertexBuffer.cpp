#include "VertexBuffer.hpp"

namespace planetoids
{
    VertexBuffer::VertexBuffer() : BufferObject()
    {
        OPENGL_CALL(glGenBuffers(1, &m_Id));
    }


    VertexBuffer::~VertexBuffer()
    {
        unbind();

        OPENGL_CALL(glDeleteBuffers(1, &m_Id));
    }


    void VertexBuffer::bind()
    {
        OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
    }


    void VertexBuffer::unbind()
    {
        OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }


    void VertexBuffer::setData(unsigned int count, unsigned int size, const void * ptr)
    {
        bind();

        m_Count = count;
        OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW));

        unbind();
    }


    unsigned int VertexBuffer::elementsCount()
    {
        return m_Count;
    }
}