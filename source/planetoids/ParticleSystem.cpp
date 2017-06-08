#include "ParticleSystem.hpp"

namespace planetoids
{
    ParticleSystem::ParticleSystem() : RenderObject()
    {
    }


    ParticleSystem::~ParticleSystem()
    {
    }


    void ParticleSystem::render(std::shared_ptr<Camera> & camera)
    {
        m_MainTexture->bind();

        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);

		for(auto i = 0; i < m_ParticlesCount; ++i)
		{
            glPushMatrix();

            // Place the quad and rotate to face the viewer
            glColor4f(m_Particles[i].color.r, m_Particles[i].color.g, m_Particles[i].color.b, m_Particles[i].life);
            glTranslatef(m_Particles[i].position.x, m_Particles[i].position.y, m_Particles[i].position.z);
            //glRotatef(-ViewYaw, 0.0f, 1.0f, 0.0f);
            //glRotatef(-ViewPitch, 1.0f, 0.0f, 0.0f);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
            glEnd();

            glPopMatrix();
        }

        glDepthMask(GL_TRUE);


        /*
        if(m_Program)
        {
            auto MVP = camera->projMatrix() * camera->viewMatrix() * matrix();

            m_Program->bind();

            m_Program->setMatrix("u_trans_m4Model", matrix());
            m_Program->setMatrix("u_trans_m4View", camera->viewMatrix());
            m_Program->setMatrix("u_trans_m4ModelViewProj", MVP);

            m_Program->setVector("u_camera_v3Position", camera->position());

            if(m_MainTexture)
            {
                m_MainTexture->bind();
                m_Program->setInteger("u_s2dMainTex0", m_MainTexture->getUnit());
            }

            if(m_NormalTexture)
            {
                m_NormalTexture->bind();
                m_Program->setInteger("u_s2dNormalTex0", m_NormalTexture->getUnit());
            }

            if(m_Mesh)
            {
                m_Mesh->render(m_Program);
            }

            m_Program->unbind();

            m_MainTexture->unbind();
            m_NormalTexture->unbind();
        }
        */
    }


    void ParticleSystem::update()
    {
        for(auto i = 0; i < m_ParticlesCount; ++i)
        {
            if(m_Particles[i].life > 0.0f)
            {
                m_Particles[i].position += m_Particles[i].direction * Timer::ptr()->deltaTime();
                m_Particles[i].life -= 0.0001f * Timer::ptr()->deltaTime();
            }
            else
            {
                m_Particles[i].position = glm::vec3(0.0f);

                m_Spread = glm::linearRand(90.0f, 110.0f);
                m_Angle = glm::linearRand(90.0f, 110.0f);

                m_Particles[i].direction.x = glm::cos(m_Angle) * m_Spread;
                m_Particles[i].direction.z = glm::sin(m_Angle) * m_Spread;
                m_Particles[i].direction.y = glm::linearRand(50.0f, 150.0f);

                m_Particles[i].life = glm::linearRand(0.0f, m_ParticleLifetime);
                m_Particles[i].color.g = 0.2f + glm::linearRand(0.0f, 0.7f);
            }

        }
    }
}