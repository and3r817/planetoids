#include "Renderer.hpp"

namespace planetoids
{
    Renderer::Renderer()
    {
        if(!glfwInit())
        {
            printf("Failed to init GLFW\n");
        }

        glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
        //glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_FSAAsamples = 0;
        m_FullScreen = false;
        m_VSync = false;

        m_Skybox = 0;

        // 		m_ShadowsEnabled = true;
        // 		if (m_ShadowsEnabled)
        // 		{
        // 			m_ShadowCamera = new Camera();
        // 		}

        //m_FrameBuffer = new FrameBuffer();
    }


    Renderer::~Renderer()
    {
        glfwTerminate();
    }


    void Renderer::showWindow(std::string title, glm::ivec2 & size)
    {
        m_WindowSize = size;
        m_WindowTitle = title;

        if(!glfwOpenWindow(m_WindowSize.x, m_WindowSize.y, 8, 8, 8, 8, 24, 0, GLFW_WINDOW))
        {
            glfwTerminate();
        }

        glewExperimental = GL_TRUE;

        if(!GLEW_VERSION_2_0)
        {
            if(glewInit() != GLEW_OK)
            {
                glfwTerminate();
            }
        }

        glfwSetWindowTitle(m_WindowTitle.c_str());

        glfwEnable(GLFW_STICKY_KEYS);

        OPENGL_CALL(glViewport(0, 0, m_WindowSize.x, m_WindowSize.y));

        OPENGL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        OPENGL_CALL(glClearDepth(1.0f));

        OPENGL_CALL(glEnable(GL_DEPTH_TEST));
        OPENGL_CALL(glEnable(GL_CULL_FACE));
    }


    void Renderer::render(std::shared_ptr<Scene> & scene)
    {
        if(m_Skybox)
        {
            m_Skybox->render(m_MainCamera);
        }

        if(scene->getObjectsCount() > 0)
        {
            for(auto i = 0; i < scene->getObjectsCount(); ++i)
            {
                auto _o = scene->getObjects()[i];

                _o->render(m_MainCamera);

                if(scene->getLightsCount() > 0)
                {
                    auto _l = scene->getLights()[0];

                    _l->render(_o->getShader());
                }
            }
        }

        if(scene->getPlanetsCount() > 0)
        {
            for(auto i = 0; i < scene->getPlanetsCount(); ++i)
            {
                auto _p = scene->getPlanets()[i];

                _p->render(m_MainCamera);

                if(scene->getLightsCount() > 0)
                {
                    auto _l = scene->getLights()[0];

                    _l->render(_p->getPlanetShader());

                    if(_p->hasAtmosphere())
                    {
                        _l->render(_p->getAtmosphereShader());
                    }
                }
            }
        }
    }


    void Renderer::update(std::shared_ptr<Scene> & scene)
    {
        if(scene->getObjectsCount() > 0)
        {
            for(auto i = 0; i < scene->getObjectsCount(); ++i)
            {
                auto _o = scene->getObjects()[i];

                _o->update();
            }
        }
    }


    void Renderer::setMainCamera(std::shared_ptr<Camera> & camera)
    {
        m_MainCamera = camera;
    }


    void Renderer::clearColor(float r, float g, float b, float a)
    {
        OPENGL_CALL(glClearColor(r, g, b, a));
    }


    void Renderer::clearScreen()
    {
        OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }


    float Renderer::aspectRatio()
    {
        return (float)m_WindowSize.x / m_WindowSize.y;
    }


    unsigned int Renderer::getFSAAsamples()
    {
        return m_FSAAsamples;
    }


    bool Renderer::getVsync()
    {
        return m_VSync;
    }


    glm::ivec2 Renderer::getWindowSize()
    {
        return m_WindowSize;
    }


    std::string Renderer::getWindowTitle()
    {
        return m_WindowTitle;
    }


    void Renderer::setFSAAsamples(unsigned int val)
    {
        m_FSAAsamples = val;

        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, val);
    }


    void Renderer::setPolygonMode(bool isWireframe)
    {
        if(isWireframe)
        {
            OPENGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        }
        else
        {
            OPENGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }
    }


    void Renderer::setSkybox(std::shared_ptr<Skybox> & skybox)
    {
        m_Skybox = skybox;
    }


    void Renderer::setVsync(bool val)
    {
        m_VSync = val;

        if(m_VSync == true)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
    }


    void Renderer::setWindowTitle(std::string val)
    {
        m_WindowTitle = val;
    }


    void Renderer::swapBuffers()
    {
        glfwSwapBuffers();
    }
}