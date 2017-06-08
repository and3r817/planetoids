#include "Texture2D.hpp"

namespace planetoids
{
    Texture2D::Texture2D() : Resource()
    {
        OPENGL_CALL(glGenTextures(1, &m_Id));
		m_Unit = 0;
    }


    Texture2D::~Texture2D()
    {
        // Освободим занятый индекс текстуры
        unbind();
        // Удалим текстуру
        OPENGL_CALL(glDeleteTextures(1, &m_Id));

        m_Loaded = false;
    }


	void Texture2D::setUnit(unsigned int unit)
	{
		m_Unit = unit;
	}


	unsigned int Texture2D::getUnit()
	{
		return m_Unit;
	}


    void Texture2D::bind()
    {
        if(!m_IsActive)
        {
            // Сделаем текстуру активной
            OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + m_Unit));
            OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, m_Id));

            m_IsActive = true;
        }
    }


    void Texture2D::unbind()
    {
        if(m_IsActive)
        {
            OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

            m_IsActive = false;
        }
    }


    void Texture2D::setFilteringMode(unsigned int filteringMode)
    {
        bind();

        OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringMode));
        OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringMode));

        unbind();
    }


    void Texture2D::setWrapMode(unsigned int wrapMode)
    {
        bind();

        OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
        OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));

        unbind();
    }


    void Texture2D::setCompareMode(unsigned int compareMode)
    {
        bind();

        OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, compareMode));

        unbind();
    }


    /************************************************************************/
    /* Public Static Methods                                                */
    /************************************************************************/


    std::shared_ptr<Texture2D> Texture2D::loadFromFile(std::string fileName)
    {
        auto texture2D = std::make_shared<Texture2D>();

        texture2D->bind();
        glfwLoadTexture2D(fileName.c_str(), 0);
        texture2D->unbind();

        texture2D->setFilteringMode(GL_LINEAR);
        texture2D->setWrapMode(GL_REPEAT);

        return texture2D;
    }


    std::shared_ptr<Texture2D> Texture2D::createEmpty(glm::ivec2 & size, unsigned int internalFormat, unsigned int format)
    {
        auto texture2D = std::make_shared<Texture2D>();

        texture2D->setFilteringMode(GL_LINEAR);
        texture2D->setWrapMode(GL_CLAMP_TO_EDGE);

        texture2D->bind();
        OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, 0));
        texture2D->unbind();

        return texture2D;
    }

	/*
    Texture2D * Texture2D::createDepthTexure(glm::ivec2 & size)
    {
        Texture2D * texture2D = createEmpty(size, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);

        texture2D->setCompareMode(GL_COMPARE_REF_TO_TEXTURE);

        return texture2D;
    }
	*/
}