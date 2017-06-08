#include "Light.hpp"

namespace planetoids
{
    Light::Light() : BaseObject()
    {
        m_Ambient = glm::vec4(0.02f, 0.02f, 0.02f, 1.0f);
        m_Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        m_Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		m_Intensity = 1.0f;
    }


    Light::~Light()
    {
    }


    void Light::render(std::shared_ptr<Shader> & program)
    {
        if(program)
        {
			program->bind();

            program->setVector("u_light_v3Position", m_Position);
			program->setVector("u_light_v3Direction", glm::normalize(m_Position));
			program->setVector("u_light_v4Ambient", m_Ambient);
            program->setVector("u_light_v4Diffuse", m_Diffuse);
			program->setVector("u_light_v4Specular", m_Specular);
            program->setFloat("u_light_fIntensity", m_Intensity);

			program->unbind();
        }
    }


    void Light::setAmbient(glm::vec4 & ambient)
    {
        m_Ambient = ambient;
    }


    void Light::setDiffuse(glm::vec4 & diffuse)
    {
        m_Diffuse = diffuse;
    }


    void Light::setSpecular(glm::vec4 & specular)
    {
        m_Specular = specular;
    }


    void Light::setIntensity(float intensity)
    {
        m_Intensity = intensity;
    }
}