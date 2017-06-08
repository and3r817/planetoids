#include "Planet.hpp"

namespace planetoids
{
    Planet::Planet() : BaseObject()
    {
        m_TextureShader = std::make_shared<Shader>("data\\shaders\\PlanetTexture");
        m_TextureBuffer = std::make_shared<FrameBuffer>();

        genIndexBuffer();

        for(auto i = 0; i < 6; ++i)
        {
            m_ChildNodes.push_back(new PlanetNode(i, m_TextureShader, m_TextureBuffer));
        }

        m_PlanetCurrProgram = 0;
        m_PlanetFromSpaceProgram = std::make_shared<Shader>("data\\shaders\\GroundFromSpace");
        m_PlanetFromAtmProgram = std::make_shared<Shader>("data\\shaders\\GroundFromAtm");
        m_HasAtmosphere = true;

        if(m_HasAtmosphere)
        {
            m_AtmCurrProgram = 0;
            m_SkyFromSpaceProgram = std::make_shared<Shader>("data\\shaders\\SkyFromSpace");
            m_SkyFromAtmProgram = std::make_shared<Shader>("data\\shaders\\SkyFromAtm");

            m_InvWavelength = glm::vec3(1 / glm::pow(0.650f, 4.0f), 1 / glm::pow(0.570f, 4.0f), 1 / glm::pow(0.475f, 4.0f));
            m_AtmosphereRadius = m_PlanetRadius * 1.025f;
            m_RayleighConst = 0.0015f;
            m_MieConst = 0.0005f;
            m_Scale = 1.0f / (m_AtmosphereRadius - m_PlanetRadius);
            m_ScaleDepth = 0.25f;
            m_Samples = 3;
            m_AsymmetryFactor = -0.99f;

            for(auto i = 0; i < 6; ++i)
            {
                m_AtmosphereMesh.push_back(Mesh::createSpherePart(m_AtmosphereRadius, 64, i));
            }
        }
		/*
        m_HasWater = true;

        if(m_HasWater)
        {
            m_WaterProgram = std::make_shared<Shader>("data\\shaders\\Water");
            m_WaterTexture = Texture2D::loadFromFile("data\\textures\\water0.tga");
            m_WaterTexture->setWrapMode(GL_CLAMP_TO_EDGE);
            m_WaterLevelRadius = 5978.0f;
            m_WaterTime = 0.0f;

            for(auto i = 0; i < 6; ++i)
            {
                m_WaterMesh.push_back(Mesh::createSpherePart(m_WaterLevelRadius, 64, i));
            }
        }
		*/

    }


    Planet::~Planet()
    {
        if(m_ChildNodes.size() > 0)
        {
            for(auto i = 0; i < 6; ++i)
            {
                delete m_ChildNodes[i];
            }
        }

        m_Textures.clear();

        if(m_HasAtmosphere)
        {
            m_AtmosphereMesh.clear();
        }

        if(m_HasWater)
        {
            m_WaterMesh.clear();
        }
    }


    void Planet::render(std::shared_ptr<Camera> & camera)
    {
        auto MVP = camera->projMatrix() * camera->viewMatrix() * m_TransformMatrix;

        camera->frustum()->extract(camera->projMatrix(), camera->viewMatrix() * m_TransformMatrix);

        if(glm::length(camera->position()) >= m_AtmosphereRadius)
        {
            m_PlanetCurrProgram = m_PlanetFromSpaceProgram;
        }
        else
        {
            m_PlanetCurrProgram = m_PlanetFromAtmProgram;
        }

        m_PlanetCurrProgram->bind();

        m_PlanetCurrProgram->setMatrix("u_trans_m4Model", m_TransformMatrix);
        m_PlanetCurrProgram->setMatrix("u_trans_m4View", camera->viewMatrix());
        m_PlanetCurrProgram->setMatrix("u_trans_m4ModelViewProj", MVP);

        m_PlanetCurrProgram->setVector("u_camera_v3Position", camera->position());
        m_PlanetCurrProgram->setFloat("u_camera_fHeight", glm::length(camera->position()));
        m_PlanetCurrProgram->setFloat("u_camera_fHeight2", glm::length(camera->position()) * glm::length(camera->position()));

        m_PlanetCurrProgram->setVector("u_atm_v3InvWavelength", m_InvWavelength);
        m_PlanetCurrProgram->setFloat("u_atm_fPlanetRadius", m_PlanetRadius);
        m_PlanetCurrProgram->setFloat("u_atm_fPlanetRadius2", m_PlanetRadius * m_PlanetRadius);
        m_PlanetCurrProgram->setFloat("u_atm_fAtmosphereRadius", m_AtmosphereRadius);
        m_PlanetCurrProgram->setFloat("u_atm_fAtmosphereRadius2", m_AtmosphereRadius * m_AtmosphereRadius);
        m_PlanetCurrProgram->setFloat("u_atm_fRayleighConst", m_RayleighConst);
        m_PlanetCurrProgram->setFloat("u_atm_fRayleighConst4PI", m_RayleighConst * 4.0f * M_PI);
        m_PlanetCurrProgram->setFloat("u_atm_fMieConst", m_MieConst);
        m_PlanetCurrProgram->setFloat("u_atm_fMieConst4PI", m_MieConst * 4.0f * M_PI);
        m_PlanetCurrProgram->setFloat("u_atm_fScale", m_Scale);
        m_PlanetCurrProgram->setFloat("u_atm_fScaleOverScaleDepth", m_Scale / m_ScaleDepth);
        m_PlanetCurrProgram->setFloat("u_atm_fSamples", (float)m_Samples);
        m_PlanetCurrProgram->setInteger("u_atm_nSamples", m_Samples);
        m_PlanetCurrProgram->setFloat("u_atm_fAsymmetryFactor", m_AsymmetryFactor);
        m_PlanetCurrProgram->setFloat("u_atm_fAsymmetryFactor2", m_AsymmetryFactor * m_AsymmetryFactor);

        for(auto i = 0; i < m_ChildNodes.size(); ++i)
        {
            m_ChildNodes[i]->render(camera, m_PlanetCurrProgram, m_IndexBuffer);
        }

        m_PlanetCurrProgram->unbind();

        if(m_HasAtmosphere)
        {
            if(glm::length(camera->position()) >= m_AtmosphereRadius)
            {
                m_AtmCurrProgram = m_SkyFromSpaceProgram;
            }
            else
            {
                m_AtmCurrProgram = m_SkyFromAtmProgram;
            }

            m_AtmCurrProgram->bind();

            m_AtmCurrProgram->setMatrix("u_trans_m4Model", m_TransformMatrix);
            m_AtmCurrProgram->setMatrix("u_trans_m4View", camera->viewMatrix());
            m_AtmCurrProgram->setMatrix("u_trans_m4ModelViewProj", MVP);

            m_AtmCurrProgram->setVector("u_camera_v3Position", camera->position());
            m_AtmCurrProgram->setFloat("u_camera_fHeight", glm::length(camera->position()));
            m_AtmCurrProgram->setFloat("u_camera_fHeight2", glm::length(camera->position()) * glm::length(camera->position()));

            m_AtmCurrProgram->setVector("u_atm_v3InvWavelength", m_InvWavelength);
            m_AtmCurrProgram->setFloat("u_atm_fPlanetRadius", m_PlanetRadius);
            m_AtmCurrProgram->setFloat("u_atm_fPlanetRadius2", m_PlanetRadius * m_PlanetRadius);
            m_AtmCurrProgram->setFloat("u_atm_fAtmosphereRadius", m_AtmosphereRadius);
            m_AtmCurrProgram->setFloat("u_atm_fAtmosphereRadius2", m_AtmosphereRadius * m_AtmosphereRadius);
            m_AtmCurrProgram->setFloat("u_atm_fRayleighConst", m_RayleighConst);
            m_AtmCurrProgram->setFloat("u_atm_fRayleighConst4PI", m_RayleighConst * 4.0f * M_PI);
            m_AtmCurrProgram->setFloat("u_atm_fMieConst", m_MieConst);
            m_AtmCurrProgram->setFloat("u_atm_fMieConst4PI", m_MieConst * 4.0f * M_PI);
            m_AtmCurrProgram->setFloat("u_atm_fScale", m_Scale);
            m_AtmCurrProgram->setFloat("u_atm_fScaleDepth", m_ScaleDepth);
            m_AtmCurrProgram->setFloat("u_atm_fScaleOverScaleDepth", m_Scale / m_ScaleDepth);
            m_AtmCurrProgram->setFloat("u_atm_fSamples", (float)m_Samples);
            m_AtmCurrProgram->setInteger("u_atm_nSamples", m_Samples);
            m_AtmCurrProgram->setFloat("u_atm_fAsymmetryFactor", m_AsymmetryFactor);
            m_AtmCurrProgram->setFloat("u_atm_fAsymmetryFactor2", m_AsymmetryFactor * m_AsymmetryFactor);

            OPENGL_CALL(glFrontFace(GL_CW));
            OPENGL_CALL(glEnable(GL_BLEND));
            OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));

            for(auto i = 0; i < m_AtmosphereMesh.size(); ++i)
            {
                m_AtmosphereMesh[i]->render(m_AtmCurrProgram);
            }

            OPENGL_CALL(glDisable(GL_BLEND));
            OPENGL_CALL(glFrontFace(GL_CCW));

            m_AtmCurrProgram->unbind();
        }
		/*
        if(m_HasWater)
        {
            //if(glm::length(camera->position()) < m_AtmosphereRadius)
            {
                m_WaterProgram->bind();

                m_WaterTexture->bind();
                m_WaterProgram->setInteger("u_s2dWaterTex", m_WaterTexture->getUnit());

                m_WaterProgram->setFloat("u_fTime", m_WaterTime);
                m_WaterTime += Timer::ptr()->deltaTime() * 0.1f;

                m_WaterProgram->setMatrix("u_trans_m4Model", m_TransformMatrix);
                m_WaterProgram->setMatrix("u_trans_m4View", camera->viewMatrix());
                m_WaterProgram->setMatrix("u_trans_m4ModelViewProj", MVP);

                //OPENGL_CALL(glEnable(GL_BLEND));
                //OPENGL_CALL(glBlendFunc(GL_ONE, GL_ONE));

                for(auto i = 0; i < 6; ++i)
                {
                    //m_WaterMesh[i]->render(m_WaterProgram);
                }

                //OPENGL_CALL(glDisable(GL_BLEND));

                m_WaterTexture->unbind();
                m_WaterProgram->unbind();
            }
        }
		*/
    }


    void Planet::genCloudsTexture()
    {
    }


    void Planet::genIndexBuffer()
    {
        m_IndexBuffer = std::make_shared<IndexBuffer>();

        std::vector<unsigned short> indices;

        for(auto b = 0; b < m_GridSize; ++b)
        {
            for(auto a = 0; a < m_GridSize; ++a)
            {
                indices.push_back(a + b * (m_GridSize + 1));
                indices.push_back(a + (b + 1) * (m_GridSize + 1));
                indices.push_back((a + 1) + (b + 1) * (m_GridSize + 1));

                indices.push_back(a + b * (m_GridSize + 1));
                indices.push_back((a + 1) + (b + 1) * (m_GridSize + 1));
                indices.push_back((a + 1) + b * (m_GridSize + 1));
            }
        }

        auto _base = (m_GridSize + 1) * (m_GridSize + 1);

        for(auto y = 0; y <= m_GridSize; ++y)
        {
            for(auto x = 0; x < m_GridSize; ++x)
            {
                if(y == 0)
                {
                    if(x == 0)
                    {
                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);

                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(x + (y + 1) * (m_GridSize + 1));

                        _base++;
                    }

                    if(x != 0)
                    {
                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);

                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(_base - 1);

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(_base - 1);

                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base + 2);
                        indices.push_back(_base);

                        _base++;
                    }
                }
                else if(y == m_GridSize)
                {
                    if(x == 0)
                    {
                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base - 2);
                        indices.push_back(_base);

                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(_base + 1);

                        _base++;
                    }

                    if(x != 0)
                    {
                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back((x - 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);

                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(_base + 1);

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);

                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back((x + 1) + (y - 1) * (m_GridSize + 1));

                        _base++;
                    }
                }
                else
                {
                    if(x == 0)
                    {
                        indices.push_back(x + y * (m_GridSize + 1));

                        if(y == 1)
                        {
                            indices.push_back(_base - m_GridSize - 1);
                        }
                        else
                        {
                            indices.push_back(_base - 2);
                        }

                        indices.push_back(_base);

                        indices.push_back(x + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back(x + (y + 1) * (m_GridSize + 1));

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        indices.push_back((x + 1) + y * (m_GridSize + 1));
                        indices.push_back(_base);
                        indices.push_back((x + 1) + (y - 1) * (m_GridSize + 1));

                        indices.push_back((x + 1) + y * (m_GridSize + 1));

                        if(y == m_GridSize - 1)
                        {
                            indices.push_back(_base + m_GridSize + 1);
                        }
                        else
                        {
                            indices.push_back(_base + 2);
                        }

                        indices.push_back(_base);

                        _base++;
                    }
                }
            }
        }

        m_IndexBuffer->setData(indices.size(), indices.size() * sizeof(unsigned short), &indices[0]);
    }


    std::shared_ptr<Shader> Planet::getPlanetShader()
    {
        return m_PlanetCurrProgram;
    }


    bool Planet::hasAtmosphere()
    {
        return m_HasAtmosphere;
    }


    std::shared_ptr<Shader> Planet::getAtmosphereShader()
    {
        return m_AtmCurrProgram;
    }


    void Planet::addTexture(std::shared_ptr<Texture2D> & texture)
    {
        m_Textures.push_back(texture);
    }


    unsigned int Planet::verticesCount()
    {
        auto n = 0;

        for(auto i = 0; i < 6; i++)
        {
            n += m_ChildNodes[i]->verticesCount();
        }

        return n;
    }


    unsigned int Planet::trianglesCount()
    {
        auto n = 0;

        for(auto i = 0; i < 6; i++)
        {
            n += m_ChildNodes[i]->trianglesCount();
        }

        return n;
    }


    unsigned int Planet::nodesCount()
    {
        auto n = 0;

        for(auto i = 0; i < 6; i++)
        {
            n += m_ChildNodes[i]->nodesCount();
        }

        return n;
    }


    unsigned int Planet::renderedNodesCount()
    {
        auto n = 0;

        for(auto i = 0; i < 6; i++)
        {
            n += m_ChildNodes[i]->renderedNodesCount();
        }

        return n;
    }
}