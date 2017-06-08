#pragma once

#ifndef _PARTICLESYSTEM_HPP
#define _PARTICLESYSTEM_HPP

#include "RenderObject.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Timer.hpp"

namespace planetoids
{
#define PARTICLE_SIZE 0.5f
#define NUM_PARTICLES 1000
#define INITIAL_PARTICLE_SPREAD 100
#define SPEED_DECAY 0.00005f

	struct Particle
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec4 color;
		float life;
	};

    class ParticleSystem : public RenderObject
    {
        public:
            ParticleSystem();
            virtual ~ParticleSystem();

            void render(std::shared_ptr<Camera> & camera);
            void update();

        private:
			std::vector<Particle> m_Particles;
			float m_ParticlesCount;
			float m_ParticleLifetime;

			float m_Spread;
			float m_Angle;
    };
}

#endif
