#include "RenderObject.hpp"

namespace planetoids
{
    RenderObject::RenderObject() : BaseObject()
    {
        m_Mesh = 0;
        m_Program = 0;
        m_MainTexture = 0;
        m_NormalTexture = 0;
    }


    RenderObject::~RenderObject()
    {
    }


    std::shared_ptr<Shader> RenderObject::getShader()
    {
        return m_Program;
    }


    void RenderObject::setGeometry(std::shared_ptr<Mesh> & geometry)
    {
        m_Mesh = geometry;
    }


    void RenderObject::setShader(std::shared_ptr<Shader> & program)
    {
        m_Program = program;
    }


    void RenderObject::setMainTexture(std::shared_ptr<Texture2D> & mainTexture)
    {
        m_MainTexture = mainTexture;
		m_MainTexture->setUnit(0);
    }


    void RenderObject::setNormalTexture(std::shared_ptr<Texture2D> & normalTexture)
    {
        m_NormalTexture = normalTexture;
		m_NormalTexture->setUnit(1);
    }
}