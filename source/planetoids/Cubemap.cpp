#include "Cubemap.hpp"

namespace planetoids
{
    Cubemap::Cubemap(std::string fileName) : Resource()
    {
        OPENGL_CALL(glGenTextures(1, &m_Id));

        OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id));

        GLFWimage _image;

        std::string _fileName = fileName;

        glfwReadImage((_fileName.append("_right.tga")).c_str(), &_image, 0);
        OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

        _fileName = fileName;
        glfwReadImage((_fileName.append("_left.tga")).c_str(), &_image, 0);
        OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

		_fileName = fileName;
		glfwReadImage((_fileName.append("_top.tga")).c_str(), &_image, 0);
		OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

		_fileName = fileName;
		glfwReadImage((_fileName.append("_bottom.tga")).c_str(), &_image, 0);
		OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

		_fileName = fileName;
		glfwReadImage((_fileName.append("_front.tga")).c_str(), &_image, 0);
		OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

		_fileName = fileName;
		glfwReadImage((_fileName.append("_back.tga")).c_str(), &_image, 0);
		OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, _image.Width, _image.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.Data));

        OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        m_Loaded = true;
    }


    Cubemap::~Cubemap()
    {
        unbind();

        OPENGL_CALL(glDeleteTextures(1, &m_Id));

        m_Loaded = false;
    }


    void Cubemap::bind()
    {
        OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + m_Id));
        OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id));
    }


    void Cubemap::unbind()
    {
        OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }
}