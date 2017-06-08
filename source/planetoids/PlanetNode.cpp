#include "PlanetNode.hpp"

namespace planetoids
{
    PlanetNode::PlanetNode(unsigned int face, std::shared_ptr<Shader> & textureShader, std::shared_ptr<FrameBuffer> & textureBuffer)
    {
        m_NodeFace = face;
        m_TextureShader = textureShader;
        m_TextureBuffer = textureBuffer;

        m_ParentNode = 0;

        for(auto i = 0; i < 4; ++i)
        {
            m_ChildNodes[i] = 0;
        }

        m_NodeBounding[NODE_BOUNDING_LEFT] = -1.0f;
        m_NodeBounding[NODE_BOUNDING_BOTTOM] = -1.0f;
        m_NodeBounding[NODE_BOUNDING_RIGHT] = 1.0f;
        m_NodeBounding[NODE_BOUNDING_TOP] = 1.0f;

        m_GeoBounding[NODE_BOUNDING_LEFT] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_BOTTOM] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_RIGHT] = -10000.0f;
        m_GeoBounding[NODE_BOUNDING_TOP] = -10000.0f;
        m_GeoBounding[NODE_BOUNDING_FRONT] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_BACK] = -10000.0f;

        m_TexcoordOffset = glm::vec2(0.0f);

        init();
    }


    PlanetNode::~PlanetNode()
    {
        if(hasChildren())
        {
            merge();
        }

        if(m_VertexBuffer)
        {
            m_VertexBuffer.reset();
        }

        m_Vertices.clear();
    }


    void PlanetNode::init()
    {
        renderNode = false;

        m_MaxScreenSpaceError = std::powf(2.0f, m_MaxDepth) * (m_MaxDepth - 2);

        m_VertexBufferCreated = false;
    }


    void PlanetNode::render(std::shared_ptr<Camera> & camera, std::shared_ptr<Shader> & program, std::shared_ptr<IndexBuffer> & buffer)
    {
        if(camera->frustum()->intersects(m_BoundingBox))
        {
            renderNode = true;

            auto _maxGeometricError = std::powf((float)2, (m_MaxDepth - depth()));
            auto _distanceToCamera = distanceToBoundingBox(camera->position());
            auto _maxScreenSpaceVertexError = (_maxGeometricError / _distanceToCamera) * 1024.0f / 0.019f;

            if(_maxScreenSpaceVertexError <= m_MaxScreenSpaceError)
            {
                if(!m_VertexBufferCreated)
                {
                    if(!hasChildren())
                    {
                        createTexture();
                        createVertexBuffer();
                    }
                }

                auto pos_id = program->getAttributeId("attr_v3Position");
                auto tex_id = program->getAttributeId("attr_v2Texcoord");

                m_VertexBuffer->bind();
                buffer->bind();

                m_Texture->bind();
                program->setInteger("u_s2dPlanetTex0", m_Texture->getUnit());

                OPENGL_CALL(glEnableVertexAttribArray(pos_id));
                OPENGL_CALL(glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0));

                OPENGL_CALL(glEnableVertexAttribArray(tex_id));
                OPENGL_CALL(glVertexAttribPointer(tex_id, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(sizeof(glm::vec3))));

                OPENGL_CALL(glDrawElements(GL_TRIANGLES, buffer->elementsCount(), GL_UNSIGNED_SHORT, (void*)0));

                OPENGL_CALL(glDisableVertexAttribArray(pos_id));
                OPENGL_CALL(glDisableVertexAttribArray(tex_id));

                m_Texture->unbind();

                buffer->unbind();
                m_VertexBuffer->unbind();

                if(hasChildren())
                {
                    merge();
                }
            }
            else
            {
                if(hasChildren())
                {
                    for(auto i = 0; i < 4; ++i)
                    {
                        m_ChildNodes[i]->render(camera, program, buffer);
                    }
                }
                else
                {
                    if(!m_VertexBufferCreated)
                    {
                        createTexture();
                        createVertexBuffer();
                    }

                    auto pos_id = program->getAttributeId("attr_v3Position");
                    auto tex_id = program->getAttributeId("attr_v2Texcoord");

                    m_VertexBuffer->bind();
                    buffer->bind();

                    m_Texture->bind();
                    program->setInteger("u_s2dPlanetTex0", m_Texture->getUnit());

                    OPENGL_CALL(glEnableVertexAttribArray(pos_id));
                    OPENGL_CALL(glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)0));

                    OPENGL_CALL(glEnableVertexAttribArray(tex_id));
                    OPENGL_CALL(glVertexAttribPointer(tex_id, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(sizeof(glm::vec3))));

                    OPENGL_CALL(glDrawElements(GL_TRIANGLES, buffer->elementsCount(), GL_UNSIGNED_SHORT, (void*)0));

                    OPENGL_CALL(glDisableVertexAttribArray(pos_id));
                    OPENGL_CALL(glDisableVertexAttribArray(tex_id));

                    m_Texture->unbind();

                    buffer->unbind();
                    m_VertexBuffer->unbind();

                    if(depth() < m_MaxDepth)
                    {
                        split();
                    }
                }
            }
        }
        else
        {
            renderNode = false;
        }
    }


    unsigned int PlanetNode::verticesCount()
    {
        if(hasChildren())
        {
            return m_ChildNodes[0]->verticesCount() + m_ChildNodes[1]->verticesCount() + m_ChildNodes[2]->verticesCount() + m_ChildNodes[3]->verticesCount();
        }
        else
        {
            return m_Vertices.size();
        }
    }


    unsigned int PlanetNode::trianglesCount()
    {
        if(hasChildren())
        {
            return m_ChildNodes[0]->trianglesCount() + m_ChildNodes[1]->trianglesCount() + m_ChildNodes[2]->trianglesCount() + m_ChildNodes[3]->trianglesCount();
        }
        else
        {
            return 0;
        }
    }


    unsigned int PlanetNode::nodesCount()
    {
        if(hasChildren())
        {
            return m_ChildNodes[0]->nodesCount() + m_ChildNodes[1]->nodesCount() + m_ChildNodes[2]->nodesCount() + m_ChildNodes[3]->nodesCount() + 1;
        }
        else
        {
            return 1;
        }
    }


    unsigned int PlanetNode::renderedNodesCount()
    {
        if(renderNode)
        {
            if(hasChildren())
            {
                return m_ChildNodes[0]->renderedNodesCount() + m_ChildNodes[1]->renderedNodesCount() + m_ChildNodes[2]->renderedNodesCount() + m_ChildNodes[3]->renderedNodesCount() + 1;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }


    PlanetNode::PlanetNode(unsigned int sector, PlanetNode * parent)
    {
        m_NodeSector = sector;
        m_ParentNode = parent;

        m_NodeFace = m_ParentNode->m_NodeFace;
        m_TextureShader = m_ParentNode->m_TextureShader;
        m_TextureBuffer = m_ParentNode->m_TextureBuffer;

        for(auto i = 0; i < 4; ++i)
        {
            m_ChildNodes[i] = 0;
        }

        auto splitX = (m_ParentNode->m_NodeBounding[NODE_BOUNDING_RIGHT] + m_ParentNode->m_NodeBounding[NODE_BOUNDING_LEFT]) * 0.5f;
        auto splitY = (m_ParentNode->m_NodeBounding[NODE_BOUNDING_BOTTOM] + m_ParentNode->m_NodeBounding[NODE_BOUNDING_TOP]) * 0.5f;

        auto d = powf(0.5f, depth());

        switch(sector)
        {
            case NODE_SECTOR_TOP_LEFT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_LEFT];
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = splitY;
                m_NodeBounding[NODE_BOUNDING_RIGHT] = splitX;
                m_NodeBounding[NODE_BOUNDING_TOP] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_TOP];
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y + d;
                break;

            case NODE_SECTOR_TOP_RIGHT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = splitX;
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = splitY;
                m_NodeBounding[NODE_BOUNDING_RIGHT] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_RIGHT];
                m_NodeBounding[NODE_BOUNDING_TOP] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_TOP];
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x + d;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y + d;
                break;

            case NODE_SECTOR_BOTTOM_LEFT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_LEFT];
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_BOTTOM];
                m_NodeBounding[NODE_BOUNDING_RIGHT] = splitX;
                m_NodeBounding[NODE_BOUNDING_TOP] = splitY;
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y;
                break;

            case NODE_SECTOR_BOTTOM_RIGHT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = splitX;
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_BOTTOM];
                m_NodeBounding[NODE_BOUNDING_RIGHT] = m_ParentNode->m_NodeBounding[NODE_BOUNDING_RIGHT];
                m_NodeBounding[NODE_BOUNDING_TOP] = splitY;
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x + d;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y;
                break;
        }

        m_GeoBounding[NODE_BOUNDING_LEFT] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_BOTTOM] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_RIGHT] = -10000.0f;
        m_GeoBounding[NODE_BOUNDING_TOP] = -10000.0f;
        m_GeoBounding[NODE_BOUNDING_FRONT] = 10000.0f;
        m_GeoBounding[NODE_BOUNDING_BACK] = -10000.0f;

        init();
    }


    void PlanetNode::merge()
    {
        for(auto i = 0; i < 4; ++i)
        {
            delete m_ChildNodes[i];

            m_ChildNodes[i] = 0;
        }
    }


    void PlanetNode::split()
    {
        m_ChildNodes[NODE_SECTOR_TOP_LEFT] = new PlanetNode(NODE_SECTOR_TOP_LEFT, this);
        m_ChildNodes[NODE_SECTOR_TOP_RIGHT] = new PlanetNode(NODE_SECTOR_TOP_RIGHT, this);
        m_ChildNodes[NODE_SECTOR_BOTTOM_LEFT] = new PlanetNode(NODE_SECTOR_BOTTOM_LEFT, this);
        m_ChildNodes[NODE_SECTOR_BOTTOM_RIGHT] = new PlanetNode(NODE_SECTOR_BOTTOM_RIGHT, this);
    }


    void PlanetNode::createVertexBuffer()
    {
        m_Vertices.clear();
        m_VertexBuffer = std::make_shared<VertexBuffer>();

        auto factorX = (m_NodeBounding[NODE_BOUNDING_RIGHT] - m_NodeBounding[NODE_BOUNDING_LEFT]) / m_GridSize;
        auto factorY = (m_NodeBounding[NODE_BOUNDING_TOP] - m_NodeBounding[NODE_BOUNDING_BOTTOM]) / m_GridSize;

        auto index = 0;
        auto heightValue = 0.0f;
        auto oneDivGridSize = 1.0f / m_GridSize;

        auto cubeCoords = glm::vec3(0.0f);

        for(auto y = m_NodeBounding[NODE_BOUNDING_BOTTOM], b = 0.0f; y <= m_NodeBounding[NODE_BOUNDING_TOP]; y += factorY, b += oneDivGridSize)
        {
            for(auto x = m_NodeBounding[NODE_BOUNDING_LEFT], a = 0.0f; x <= m_NodeBounding[NODE_BOUNDING_RIGHT]; x += factorX, a += oneDivGridSize)
            {
                TerrainVertex v;
                cubeCoords = coordsToFace(m_NodeFace, glm::vec2(x, y));
                heightValue = 1.0f;//heightValue(glm::vec2(a, b));
                v.position = sphereCoords(cubeCoords) * m_PlanetRadius * heightValue;
                v.texcoord = glm::vec2(a, b);
                m_Vertices.push_back(v);

                // AABB
                if(m_Vertices[index].position.x < m_GeoBounding[NODE_BOUNDING_LEFT])
                {
                    m_GeoBounding[NODE_BOUNDING_LEFT] = m_Vertices[index].position.x;
                }
                else if(m_Vertices[index].position.x > m_GeoBounding[NODE_BOUNDING_RIGHT])
                {
                    m_GeoBounding[NODE_BOUNDING_RIGHT] = m_Vertices[index].position.x;
                }

                if(m_Vertices[index].position.y < m_GeoBounding[NODE_BOUNDING_FRONT])
                {
                    m_GeoBounding[NODE_BOUNDING_FRONT] = m_Vertices[index].position.y;
                }
                else if(m_Vertices[index].position.y > m_GeoBounding[NODE_BOUNDING_BACK])
                {
                    m_GeoBounding[NODE_BOUNDING_BACK] = m_Vertices[index].position.y;
                }

                if(m_Vertices[index].position.z < m_GeoBounding[NODE_BOUNDING_BOTTOM])
                {
                    m_GeoBounding[NODE_BOUNDING_BOTTOM] = m_Vertices[index].position.z;
                }
                else if(m_Vertices[index].position.z > m_GeoBounding[NODE_BOUNDING_TOP])
                {
                    m_GeoBounding[NODE_BOUNDING_TOP] = m_Vertices[index].position.z;
                }

                index++;
            }
        }

        // build skirts vertex data
        for(auto y = m_NodeBounding[NODE_BOUNDING_BOTTOM], b = 0.0f; y <= m_NodeBounding[NODE_BOUNDING_TOP]; y += factorY, b += oneDivGridSize)
        {
            for(auto x = m_NodeBounding[NODE_BOUNDING_LEFT], a = 0.0f; x <= m_NodeBounding[NODE_BOUNDING_RIGHT]; x += factorX, a += oneDivGridSize)
            {
                if(y == m_NodeBounding[NODE_BOUNDING_BOTTOM] || y == m_NodeBounding[NODE_BOUNDING_TOP] || x == m_NodeBounding[NODE_BOUNDING_LEFT]	|| x == m_NodeBounding[NODE_BOUNDING_RIGHT])
                {
                    TerrainVertex v;
                    cubeCoords = coordsToFace(m_NodeFace, glm::vec2(x, y));
                    heightValue = 1.0f;//heightValue(glm::vec2(a, b));
                    v.position = sphereCoords(cubeCoords) * m_PlanetRadius * heightValue;
                    v.position *= 0.99f + (depth() * (0.01f / m_MaxDepth));
                    v.texcoord = glm::vec2(a, b);
                    v.parent = v.position;
                    m_Vertices.push_back(v);
                    index++;
                }
            }
        }

        index = 0;

        for(float y = m_NodeBounding[NODE_BOUNDING_BOTTOM], j = 0; y <= m_NodeBounding[NODE_BOUNDING_TOP]; y += factorY, j++)
        {
            for(float x = m_NodeBounding[NODE_BOUNDING_LEFT], i = 0; x <= m_NodeBounding[NODE_BOUNDING_RIGHT]; x += factorX, i++)
            {
                if((int)j % 2 == 0 && (int)i % 2 == 0)
                {
                    m_Vertices[index].parent = m_Vertices[index].position.xyz;
                }
                else if((int)j % 2 == 1 && (int)i % 2 == 1)
                {
                    m_Vertices[index].parent = (m_Vertices[index + m_GridSize].position + m_Vertices[index - m_GridSize].position) * 0.5f;
                }
                else if((int)i % 2 == 1)
                {
                    m_Vertices[index].parent = (m_Vertices[index + 1].position + m_Vertices[index - 1].position) * 0.5f;
                }
                else if((int)j % 2 == 1)
                {
                    m_Vertices[index].parent = (m_Vertices[index + m_GridSize + 1].position + m_Vertices[index - m_GridSize - 1].position) * 0.5f;
                }

                index++;
            }
        }

        glm::vec3 bound_lfb(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        //glm::vec3 bound_rfb(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        //glm::vec3 bound_rft(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_TOP]);
        //glm::vec3 bound_lft(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_TOP]);
        //glm::vec3 bound_lbb(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        //glm::vec3 bound_rbb(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        glm::vec3 bound_rbt(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_TOP]);
        //glm::vec3 bound_lbt(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_TOP]);

        m_BoundingBox.radius.x = glm::abs(m_GeoBounding[NODE_BOUNDING_LEFT] + glm::abs((glm::sqrt(3.0f) / 2.0f) * (m_GeoBounding[NODE_BOUNDING_RIGHT] - m_GeoBounding[NODE_BOUNDING_LEFT]) / 2.0f));
        m_BoundingBox.center = bound_lfb + (bound_rbt - bound_lfb) / 2.0f;

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(TerrainVertex), &m_Vertices[0]);

        m_VertexBufferCreated = true;
        m_Vertices.clear();
    }


    void PlanetNode::createTexture()
    {
        auto texSize = 0;

        switch(depth())
        {
            case 0:
                texSize = 512;
                break;

            case 1:
                texSize = 256;
                break;

            default:
                texSize = 128;
                break;
        }

        std::vector<glm::vec2> texcoords;

        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_TOP]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_TOP]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_TOP]));

        auto fullscreenQuad = Mesh::createFullscreenQuad(texcoords);

        OPENGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

        auto oldProgram = 0;
        OPENGL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgram));

        m_TextureShader->bind();
        m_TextureShader->setInteger("u_iFace", m_NodeFace);

        m_Texture = Texture2D::createEmpty(glm::ivec2(texSize), GL_RGBA, GL_RGBA);
        m_Texture->setUnit(0);

        m_TextureBuffer->attachTexture(m_Texture, GL_COLOR_ATTACHMENT0_EXT);
        m_TextureBuffer->setDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
        m_TextureBuffer->checkStatus();

        m_TextureBuffer->bind();

        OPENGL_CALL(glViewport(0, 0, texSize, texSize));
        OPENGL_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
        OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        // Квад здесь
        fullscreenQuad->render(m_TextureShader);

        OPENGL_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
        OPENGL_CALL(glViewport(0, 0, 1024, 576));

        m_TextureBuffer->unbind();
        m_TextureBuffer->detachTexture(GL_COLOR_ATTACHMENT0_EXT);

        m_TextureShader->unbind();

        OPENGL_CALL(glUseProgram(oldProgram));

        /*
        m_Texture->bind();
        std::vector<char> pixels(texSize * texSize, 0.0f);
        OPENGL_CALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, &pixels[0]));
        m_Texture->unbind();

        m_Heights = std::make_shared<Heightmap>(pixels, texSize);
        pixels.clear();
        */
        /*
        unsigned int texSize = 128;

        std::vector<glm::vec2> texcoords;

        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_TOP]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_BOTTOM]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_TOP]));
        texcoords.push_back(glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_TOP]));

        auto fullscreenQuad = Mesh::createFullscreenQuad(texcoords);

        OPENGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

        int oldProgram = 0;
        OPENGL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgram));

        m_TextureShader->bind();
        m_TextureShader->setInteger("u_iFace", m_NodeFace);

        m_Texture = Texture2D::createEmpty(glm::ivec2(texSize), GL_RGBA, GL_RGBA);
        m_Texture->setUnit(0);

        m_TextureBuffer->attachTexture(m_Texture, GL_COLOR_ATTACHMENT0_EXT);
        m_TextureBuffer->setDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
        m_TextureBuffer->checkStatus();

        m_TextureBuffer->bind();

        OPENGL_CALL(glViewport(0, 0, texSize, texSize));
        OPENGL_CALL(glClearColor(0.3, 0.3, 0.3, 1.0));
        OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        fullscreenQuad->render(m_TextureShader);

        OPENGL_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
        OPENGL_CALL(glViewport(0, 0, 1024, 576));

        m_TextureBuffer->unbind();
        m_TextureBuffer->detachTexture(GL_COLOR_ATTACHMENT0_EXT);

        m_TextureShader->unbind();

        OPENGL_CALL(glUseProgram(oldProgram));

        m_Texture->bind();
        std::vector<char> pixels(texSize * texSize);
        OPENGL_CALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, &pixels[0]));
        m_Texture->unbind();

        m_Heights = std::make_shared<Heightmap>(pixels, texSize);
        pixels.clear();
        */
    }


    BoundingBox PlanetNode::boundingBox()
    {
        return m_BoundingBox;
    }


    float PlanetNode::distanceToBoundingBox(glm::vec3 & viewPoint)
    {
        return glm::distance(m_BoundingBox.center, viewPoint);
    }


    bool PlanetNode::hasChildren()
    {
        return (m_ChildNodes[0] != 0) && (m_ChildNodes[1] != 0) && (m_ChildNodes[2] != 0) && (m_ChildNodes[3] != 0);
    }


    bool PlanetNode::hasParent()
    {
        return m_ParentNode != 0;
    }


    unsigned int PlanetNode::depth()
    {
        if(hasParent())
        {
            return m_ParentNode->depth() + 1;
        }

        return 0;
    }


    glm::vec3 PlanetNode::coordsToFace(unsigned int face, glm::vec2 & coords)
    {
        glm::vec3 _faceCoords;

        switch(face)
        {
            case NODE_FRONT:
                _faceCoords.x = coords.x;
                _faceCoords.y = coords.y;
                _faceCoords.z = -1.0f;
                break;

            case NODE_BACK:
                _faceCoords.x = coords.y;
                _faceCoords.y = coords.x;
                _faceCoords.z = 1.0f;
                break;

            case NODE_LEFT:
                _faceCoords.x = -1.0f;
                _faceCoords.y = coords.x;
                _faceCoords.z = coords.y;
                break;

            case NODE_RIGHT:
                _faceCoords.x = 1.0f;
                _faceCoords.y = coords.y;
                _faceCoords.z = coords.x;
                break;

            case NODE_BOTTOM:
                _faceCoords.x = coords.y;
                _faceCoords.y = -1.0f;
                _faceCoords.z = coords.x;
                break;

            case NODE_TOP:
                _faceCoords.x = coords.x;
                _faceCoords.y = 1.0f;
                _faceCoords.z = coords.y;
                break;
        }

        return _faceCoords;
    }


    glm::vec3 PlanetNode::sphereCoords(glm::vec3 & position)
    {
        return glm::normalize(position);
    }


    float PlanetNode::heightValue(glm::vec2 & coord)
    {
        coord.x *= 128;
        coord.y *= 128;

        auto heightValue = m_Heights->getHeightAt(coord);
        heightValue = (heightValue * m_HeightScale) + m_HeightBias;

        return heightValue;
    }
}
