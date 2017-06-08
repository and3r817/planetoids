#pragma once

#ifndef _SPACESHIP_HPP
#define _SPACESHIP_HPP

#include "RenderObject.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Timer.hpp"

namespace planetoids
{
    class Spaceship : public RenderObject
    {
        public:
            Spaceship(std::shared_ptr<Camera> & playerCamera);
            virtual ~Spaceship();

            void render(std::shared_ptr<Camera> & camera);
			void update();

	private:
		std::shared_ptr<Camera> m_Camera;
		glm::vec3 m_CamTempPos;

		float m_MovementSpeed;
		float m_RotationSpeed;
		float m_MouseSensitivity;

		glm::vec2 m_Angles;
		bool m_Aspect;

		glm::vec3 r;
		int r_Count;
		int r_Current;
    };
}

#endif
