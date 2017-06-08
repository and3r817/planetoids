#include "Sprite.hpp"

namespace planetoids
{
    Sprite::Sprite() : RenderObject()
    {
    }


    Sprite::~Sprite()
    {
    }


    void Sprite::render(std::shared_ptr<Camera> & camera)
    {
        if(m_Program)
        {
            auto M = glm::scale(matrix(), glm::vec3(50.0f));
            auto MVP = camera->projMatrix() * camera->viewMatrix() * M;

            m_Program->bind();

            m_Program->setMatrix("u_trans_m4ModelView",  camera->viewMatrix() * M);
            m_Program->setMatrix("u_trans_m4Proj", camera->projMatrix());

            m_Program->setVector("u_v3Position", camera->position());

            if(m_MainTexture)
            {
                m_MainTexture->bind();
                m_Program->setInteger("u_s2dMainTex0", m_MainTexture->getUnit());
            }

            OPENGL_CALL(glEnable(GL_BLEND));
            OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));

            if(m_Mesh)
            {
                m_Mesh->render(m_Program);
            }

            OPENGL_CALL(glDisable(GL_BLEND));

            m_Program->unbind();

            m_MainTexture->unbind();
        }
    }


    void Sprite::update()
    {

    }
}