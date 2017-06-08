#pragma once

#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include "BaseObject.hpp"
#include "Camera.hpp"
#include "Platform.hpp"
#include "Shader.hpp"

namespace planetoids
{
    class Light : public BaseObject
    {
        public:
            Light();
            virtual ~Light();

            void render(std::shared_ptr<Shader> & program);

            void setAmbient(glm::vec4 & ambient);

            void setDiffuse(glm::vec4 & diffuse);

            void setSpecular(glm::vec4 & specular);

            void setIntensity(float intensity);

        protected:
			std::shared_ptr<Camera> m_Camera;

			glm::vec3 m_Direction;

            glm::vec4 m_Ambient;
            glm::vec4 m_Diffuse;
            glm::vec4 m_Specular;

            float m_Intensity;
    };
}

#endif
