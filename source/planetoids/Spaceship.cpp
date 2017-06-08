#include "Spaceship.hpp"

namespace planetoids
{
    Spaceship::Spaceship(std::shared_ptr<Camera> & playerCamera) : RenderObject()
    {
        m_MovementSpeed = 0.0f;
        m_RotationSpeed = 0.32f;
        m_MouseSensitivity = 0.007f;

        m_Camera = playerCamera;

        m_Angles = glm::vec2(glm::radians(-90.0f), 0.0f);

        m_Aspect = false;

        r_Count = 18;
        r_Current = 18;
    }


    Spaceship::~Spaceship()
    {
    }


    void Spaceship::render(std::shared_ptr<Camera> & camera)
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


    void Spaceship::update()
    {
        translate(foward() * Timer::ptr()->deltaTime() * m_MovementSpeed);

        if(Input::ptr()->keyPress('f') || Input::ptr()->keyPress('F'))
        {
            if(m_Aspect)
            {
                m_Camera->perspective(60.0f, Renderer::ptr()->aspectRatio(), 1.0f, 100000.0f);
                m_Aspect = false;
            }
            else
            {
                m_Camera->perspective(20.0f, Renderer::ptr()->aspectRatio(), 1.0f, 100000.0f);
                m_Aspect = true;
            }
        }

        if(Input::ptr()->keyPress(GLFW_KEY_UP))
        {
            m_MovementSpeed += 2.0f;
            m_MovementSpeed = glm::clamp(m_MovementSpeed, 1.0f, 600.0f);
        }

        if(Input::ptr()->keyPress(GLFW_KEY_DOWN))
        {
            m_MovementSpeed -= 15.0f;
            m_MovementSpeed = glm::clamp(m_MovementSpeed, 1.0f, 600.0f);
        }

        if(Input::ptr()->mouseButtonPress(GLFW_MOUSE_BUTTON_RIGHT))
        {
            auto mp = Timer::ptr()->deltaTime() * m_MouseSensitivity * glm::vec2(Renderer::ptr()->getWindowSize() / 2 - Input::ptr()->mousePosition());
            m_Angles.x += mp.x;
            m_Angles.y += mp.y;
            /*
            int xpos, ypos;
            glfwGetMousePos(&xpos, &ypos);
            */
            Input::ptr()->lockMousePosition();

            //              auto d = 10.0f * glm::vec3(
            //  				cos(m_Angles.y) * cos(m_Angles.x),
            //  				sin(m_Angles.y),
            //  				cos(m_Angles.y) * sin(m_Angles.x));


            auto d = 7.0f * glm::vec3(
                         cos(m_Angles.y) * cos(m_Angles.x),
                         sin(m_Angles.y),
                         cos(m_Angles.y) * sin(m_Angles.x));

            glm::vec3 r = glm::vec3(
                              sin(m_Angles.x - 3.14f / 2.0f),
                              0,
                              cos(m_Angles.x - 3.14f / 2.0f)
                          );

            // Up vector
            //glm::vec3 u = glm::cross( r, d );

            int u;

            if(m_Angles.y < 0)
                m_Angles.y = 2 * M_PI + m_Angles.y;

            if(m_Angles.y > 2 * M_PI)
                m_Angles.y = m_Angles.y - 2 * M_PI;

            if((m_Angles.y > M_PI / 2) && (m_Angles.y < 3 * M_PI / 2))
                u = -1;
            else
                u = 1;

            m_Camera->setPosition(position() + d);
			m_Camera->setRotation(-glm::vec3(d.yxz));
            //m_Camera->lookAt(position(), glm::vec3(0.0f, u, 0.0f));
            //m_Camera->setRotation(d);
            //m_Camera->setPosition(position() + d);




            /*
            m_Camera->setPosition(position());

            if(Input::ptr()->keyPress('w') || Input::ptr()->keyPress('W'))
            {
            	m_Camera->rotate(-right() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }

            if(Input::ptr()->keyPress('s') || Input::ptr()->keyPress('S'))
            {
            	m_Camera->rotate(right() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }

            if(Input::ptr()->keyPress('d') || Input::ptr()->keyPress('D'))
            {
            	m_Camera->rotate(-up() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }

            if(Input::ptr()->keyPress('a') || Input::ptr()->keyPress('A'))
            {
            	m_Camera->rotate(up() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }

            if(Input::ptr()->keyPress('q') || Input::ptr()->keyPress('Q'))
            {
            	m_Camera->rotate(-foward() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }

            if(Input::ptr()->keyPress('e') || Input::ptr()->keyPress('E'))
            {
            	m_Camera->rotate(foward() * Timer::ptr()->deltaTime() * m_RotationSpeed * 2.0f);
            }
            m_Camera->setPosition(m_Position - m_Camera->rotation() * glm::vec3(-5.0f));
            */
        }
        else
        {
            if(Input::ptr()->keyPress('w') || Input::ptr()->keyPress('W'))
            {
                rotate(-right() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(-right() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            if(Input::ptr()->keyPress('s') || Input::ptr()->keyPress('S'))
            {
                rotate(right() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(right() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            if(Input::ptr()->keyPress('d') || Input::ptr()->keyPress('D'))
            {
                rotate(-up() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(-up() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            if(Input::ptr()->keyPress('a') || Input::ptr()->keyPress('A'))
            {
                rotate(up() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(up() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            if(Input::ptr()->keyPress('q') || Input::ptr()->keyPress('Q'))
            {
                rotate(-foward() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(-foward() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            if(Input::ptr()->keyPress('e') || Input::ptr()->keyPress('E'))
            {
                rotate(foward() * Timer::ptr()->deltaTime() * m_RotationSpeed);
                m_Camera->rotate(foward() * Timer::ptr()->deltaTime() * m_RotationSpeed);
            }

            m_Camera->setPosition(rotation() * glm::vec3(0.0f, 1.0f, -8.0f) + m_Position);
        }
    }
}