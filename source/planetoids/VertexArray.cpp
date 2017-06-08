#include "VertexArray.hpp"

namespace planetoids
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_Id);
    }


    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_Id);
    }


    void VertexArray::bind()
    {
        glBindVertexArray(m_Id);
    }


    void VertexArray::unbind()
    {
		glBindVertexArray(0);
    }


    bool VertexArray::isOk()
    {
        return glIsVertexArray(m_Id);
    }


    unsigned int VertexArray::getId()
    {
        return m_Id;
    }
}