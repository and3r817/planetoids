#include "Shader.hpp"

namespace planetoids
{
    Shader::Shader(std::string name) : Resource()
    {
        OPENGL_CALL(m_Id = glCreateProgram());

        auto _vsId = load(name + ".vert", GL_VERTEX_SHADER);
        auto _fsId = load(name + ".frag", GL_FRAGMENT_SHADER);

        OPENGL_CALL(glAttachShader(m_Id, _vsId));
        OPENGL_CALL(glAttachShader(m_Id, _fsId));

        OPENGL_CALL(glLinkProgram(m_Id));

        int _linked;
        int _length;
        char _buffer[1024];

        OPENGL_CALL(glGetProgramiv(m_Id, GL_LINK_STATUS, &_linked));

        if(!_linked)
        {
            OPENGL_CALL(glGetProgramInfoLog(m_Id, 1024, &_length, _buffer));
			printf(name.c_str());
            printf(_buffer);
            //throw Error("Error linking shader program.\n");
        }

        OPENGL_CALL(glDeleteShader(_vsId));
        OPENGL_CALL(glDeleteShader(_fsId));

        m_Loaded = true;
    }


    Shader::~Shader()
    {
        unbind();

        OPENGL_CALL(glDeleteProgram(m_Id));

        m_Loaded = false;
    }


    void Shader::bind()
    {
        OPENGL_CALL(glUseProgram(m_Id));
    }


	void Shader::unbind()
	{
		OPENGL_CALL(glUseProgram(0));
	}


    unsigned int Shader::getAttributeId(std::string attribute)
    {
        return glGetAttribLocation(m_Id, attribute.c_str());
    }


    unsigned int Shader::getUniformId(std::string uniform)
    {
        return glGetUniformLocation(m_Id, uniform.c_str());
    }


    void Shader::setInteger(std::string uniform, unsigned int val)
    {
        OPENGL_CALL(glUniform1i(getUniformId(uniform), val));
    }


    void Shader::setFloat(std::string uniform, float val)
    {
        OPENGL_CALL(glUniform1fv(getUniformId(uniform), 1, &val));
    }


    void Shader::setVector(std::string uniform, glm::vec3 & val)
    {
        OPENGL_CALL(glUniform3fv(getUniformId(uniform), 1, glm::value_ptr(val)));
    }


    void Shader::setVector(std::string uniform, glm::vec4 & val)
    {
        OPENGL_CALL(glUniform4fv(getUniformId(uniform), 1, glm::value_ptr(val)));
    }


    void Shader::setMatrix(std::string uniform, glm::mat4 & val)
    {
        OPENGL_CALL(glUniformMatrix4fv(getUniformId(uniform), 1, GL_FALSE, glm::value_ptr(val)));
    }


    unsigned int Shader::load(std::string fileName, unsigned int shaderType)
    {
        std::string _shaderString = readFromFile(fileName);
        const char * _shaderSource = _shaderString.c_str();
        int _shaderLength = _shaderString.size();

        unsigned int _id = glCreateShader(shaderType);

        OPENGL_CALL(glShaderSource(_id, 1, &_shaderSource, &_shaderLength));

        OPENGL_CALL(glCompileShader(_id));

        int _status;
        int _length;
        char _buffer[1024];
        OPENGL_CALL(glGetShaderiv(_id, GL_COMPILE_STATUS, &_status));

        if(_status == 0)
        {
            OPENGL_CALL(glGetShaderInfoLog(_id, 1024, &_length, _buffer));
            printf(_buffer);
            OPENGL_CALL(glDeleteShader(_id));

            return 0;
        }

        return _id;
    }


    std::string Shader::readFromFile(std::string fileName)
    {
        std::ifstream _file(fileName);
        std::string _shaderString;

        if(_file.is_open())
        {
            while(_file.good())
            {
                std::string _line;
                std::getline(_file, _line);
                _shaderString.append(_line + "\n");
            }

            _file.close();
        }
        else
        {
            //throw Error("Error open file.\n");
        }

        return _shaderString;
    }
}