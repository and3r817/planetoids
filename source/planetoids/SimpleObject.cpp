#include "SimpleObject.hpp"

namespace planetoids
{
    SimpleObject::SimpleObject() : RenderObject()
    {
    }


    SimpleObject::~SimpleObject()
    {
    }
	
	
    void SimpleObject::render(std::shared_ptr<Camera> & camera)
    {
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
    }


	void SimpleObject::update()
	{

	}
}