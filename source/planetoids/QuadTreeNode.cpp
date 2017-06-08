#include "QuadTreeNode.hpp"

namespace planetoids
{
    QuadTreeNode::QuadTreeNode(unsigned int face)
    {
        m_NodeFace = face;

        m_ParentNode = NULL;

        for(auto i = 0; i < 4; ++i)
        {
            m_ChildNodes[i] = NULL;
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

        m_MaxScreenSpaceError = std::powf(2.0f, m_MaxDepth) * (m_MaxDepth - 1);

        m_TexcoordOffset = glm::vec2(0.0f);

        m_VertexArray = new VertexArray();
        m_VertexBuffer = new VertexBuffer();
        m_IndexBuffer = new IndexBuffer();

        init();
    }


    QuadTreeNode::~QuadTreeNode()
    {
        if(hasChildren())
        {
            merge();
        }

        delete m_VertexBuffer;

        m_Vertices.clear();

        if(!hasParent())
        {
            delete m_VertexArray;

            delete m_IndexBuffer;

            m_Indices.clear();
        }
    }


    void QuadTreeNode::init()
    {
        renderNode = false;

        if(!hasChildren())
        {
            createIndexBuffer();
            createVertexBuffer();
        }
    }


    void QuadTreeNode::render(Camera * camera, Shader * program)
    {
        glm::vec3 points[8];

        getBBox(points);

        if(camera->getFrustum()->intersects(points))
        {
            renderNode = true;

            auto _maxGeometricError = (unsigned int)std::powf((float)2, (m_MaxDepth - getDepth()));
            auto _distanceToCamera = getDistanceToBB(camera->getPosition(), m_GeoBounding);
            auto _maxScreenSpaceVertexError = (_maxGeometricError / _distanceToCamera) * 800 / 0.035295f;

            if(_maxScreenSpaceVertexError <= m_MaxScreenSpaceError)
            {
				m_VertexArray->bind();
				m_VertexBuffer->bind();
				m_IndexBuffer->bind();	
				
				glEnableVertexAttribArray(program->getAttributeId("msPosition"));
				glVertexAttribPointer(program->getAttributeId("msPosition"), 4, GL_FLOAT, GL_FALSE,	sizeof(TerrainVertex), (void*)0);

				glEnableVertexAttribArray(program->getAttributeId("msTexcoord"));
				glVertexAttribPointer(program->getAttributeId("msTexcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(sizeof(glm::vec4)));

				glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_SHORT, (void*)0);

				glDisableVertexAttribArray(program->getAttributeId("msPosition"));
				glDisableVertexAttribArray(program->getAttributeId("msTexcoord"));

				m_IndexBuffer->unbind();
				m_VertexArray->unbind();
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
                        m_ChildNodes[i]->render(camera, program);
                    }
                }
                else
                {
                    m_VertexArray->bind();
                    m_VertexBuffer->bind();
                    m_IndexBuffer->bind();					

					glEnableVertexAttribArray(program->getAttributeId("msPosition"));
					glVertexAttribPointer(program->getAttributeId("msPosition"), 4, GL_FLOAT, GL_FALSE,	sizeof(TerrainVertex), (void*)0);

					glEnableVertexAttribArray(program->getAttributeId("msTexcoord"));
					glVertexAttribPointer(program->getAttributeId("msTexcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(sizeof(glm::vec4)));

                    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_SHORT, (void*)0);

					glDisableVertexAttribArray(program->getAttributeId("msPosition"));
					glDisableVertexAttribArray(program->getAttributeId("msTexcoord"));

                    m_IndexBuffer->unbind();
                    m_VertexArray->unbind();
                    m_VertexBuffer->unbind();

                    if(getDepth() < m_MaxDepth)
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


    unsigned int QuadTreeNode::verticesCount()
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


    unsigned int QuadTreeNode::trianglesCount()
    {
        if(hasChildren())
        {
            return m_ChildNodes[0]->trianglesCount() + m_ChildNodes[1]->trianglesCount() + m_ChildNodes[2]->trianglesCount() + m_ChildNodes[3]->trianglesCount();
        }
        else
        {
            return m_Indices.size() / 3;
        }
    }


    unsigned int QuadTreeNode::nodesCount()
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


    unsigned int QuadTreeNode::renderedNodesCount()
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


    QuadTreeNode::QuadTreeNode(QuadTreeNode * parent, unsigned int sector)
    {
        m_ParentNode = parent;

        m_NodeSector = sector;

        m_NodeFace = parent->m_NodeFace;

        for(auto i = 0; i < 4; ++i)
        {
            m_ChildNodes[i] = NULL;
        }

        auto splitX = (parent->m_NodeBounding[NODE_BOUNDING_RIGHT] + parent->m_NodeBounding[NODE_BOUNDING_LEFT]) * 0.5f;
        auto splitY = (parent->m_NodeBounding[NODE_BOUNDING_BOTTOM] + parent->m_NodeBounding[NODE_BOUNDING_TOP]) * 0.5f;

        float d = powf(0.5f, getDepth());

        switch(sector)
        {
            case NODE_SECTOR_TOP_LEFT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = parent->m_NodeBounding[NODE_BOUNDING_LEFT];
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = splitY;
                m_NodeBounding[NODE_BOUNDING_RIGHT] = splitX;
                m_NodeBounding[NODE_BOUNDING_TOP] = parent->m_NodeBounding[NODE_BOUNDING_TOP];
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y + d;
                break;

            case NODE_SECTOR_TOP_RIGHT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = splitX;
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = splitY;
                m_NodeBounding[NODE_BOUNDING_RIGHT] = parent->m_NodeBounding[NODE_BOUNDING_RIGHT];
                m_NodeBounding[NODE_BOUNDING_TOP] = parent->m_NodeBounding[NODE_BOUNDING_TOP];
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x + d;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y + d;
                break;

            case NODE_SECTOR_BOTTOM_LEFT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = parent->m_NodeBounding[NODE_BOUNDING_LEFT];
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = parent->m_NodeBounding[NODE_BOUNDING_BOTTOM];
                m_NodeBounding[NODE_BOUNDING_RIGHT] = splitX;
                m_NodeBounding[NODE_BOUNDING_TOP] = splitY;
                m_TexcoordOffset.x = m_ParentNode->m_TexcoordOffset.x;
                m_TexcoordOffset.y = m_ParentNode->m_TexcoordOffset.y;
                break;

            case NODE_SECTOR_BOTTOM_RIGHT:
                m_NodeBounding[NODE_BOUNDING_LEFT] = splitX;
                m_NodeBounding[NODE_BOUNDING_BOTTOM] = parent->m_NodeBounding[NODE_BOUNDING_BOTTOM];
                m_NodeBounding[NODE_BOUNDING_RIGHT] = parent->m_NodeBounding[NODE_BOUNDING_RIGHT];
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

        m_MaxScreenSpaceError = std::powf(2.0f, m_MaxDepth) * (m_MaxDepth - 1);

        m_VertexArray = new VertexArray();
        m_VertexBuffer = new VertexBuffer();
        m_IndexBuffer = new IndexBuffer();

        init();
    }


    void QuadTreeNode::merge()
    {
        for(unsigned int i = 0; i < 4; ++i)
        {
            delete m_ChildNodes[i];

            m_ChildNodes[i] = NULL;
        }
    }


    void QuadTreeNode::split()
    {
        m_ChildNodes[NODE_SECTOR_TOP_LEFT] = new QuadTreeNode(this, NODE_SECTOR_TOP_LEFT);
        m_ChildNodes[NODE_SECTOR_TOP_RIGHT] = new QuadTreeNode(this, NODE_SECTOR_TOP_RIGHT);
        m_ChildNodes[NODE_SECTOR_BOTTOM_LEFT] = new QuadTreeNode(this, NODE_SECTOR_BOTTOM_LEFT);
        m_ChildNodes[NODE_SECTOR_BOTTOM_RIGHT] = new QuadTreeNode(this, NODE_SECTOR_BOTTOM_RIGHT);
    }


    void QuadTreeNode::createIndexBuffer()
    {
        m_Indices.clear();

        for(auto b = 0; b < m_GridSize; ++b)
        {
            for(auto a = 0; a < m_GridSize; ++a)
            {
                m_Indices.push_back(a + b * (m_GridSize + 1));
                m_Indices.push_back(a + (b + 1) * (m_GridSize + 1));
                m_Indices.push_back((a + 1) + (b + 1) * (m_GridSize + 1));

                m_Indices.push_back(a + b * (m_GridSize + 1));
                m_Indices.push_back((a + 1) + (b + 1) * (m_GridSize + 1));
                m_Indices.push_back((a + 1) + b * (m_GridSize + 1));
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
                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);

                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(x + (y + 1) * (m_GridSize + 1));

                        _base++;
                    }

                    if(x != 0)
                    {
                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);

                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(_base - 1);

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(_base - 1);

                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base + 2);
                        m_Indices.push_back(_base);

                        _base++;
                    }
                }
                else if(y == m_GridSize)
                {
                    if(x == 0)
                    {
                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base - 2);
                        m_Indices.push_back(_base);

                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(_base + 1);

                        _base++;
                    }

                    if(x != 0)
                    {
                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back((x - 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);

                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(_base + 1);

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);

                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back((x + 1) + (y - 1) * (m_GridSize + 1));

                        _base++;
                    }
                }
                else
                {
                    if(x == 0)
                    {
                        m_Indices.push_back(x + y * (m_GridSize + 1));

                        if(y == 1)
                        {
                            m_Indices.push_back(_base - m_GridSize - 1);
                        }
                        else
                        {
                            m_Indices.push_back(_base - 2);
                        }

                        m_Indices.push_back(_base);

                        m_Indices.push_back(x + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back(x + (y + 1) * (m_GridSize + 1));

                        _base++;
                    }

                    if(x == m_GridSize - 1)
                    {
                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));
                        m_Indices.push_back(_base);
                        m_Indices.push_back((x + 1) + (y - 1) * (m_GridSize + 1));

                        m_Indices.push_back((x + 1) + y * (m_GridSize + 1));

                        if(y == m_GridSize - 1)
                        {
                            m_Indices.push_back(_base + m_GridSize + 1);
                        }
                        else
                        {
                            m_Indices.push_back(_base + 2);
                        }

                        m_Indices.push_back(_base);

                        _base++;
                    }
                }
            }
        }

        m_VertexArray->bind();
        m_IndexBuffer->setData(m_Indices.size() * sizeof(unsigned short), &m_Indices[0]);
        m_VertexArray->unbind();
    }


    void QuadTreeNode::createVertexBuffer()
    {
        m_Vertices.clear();

        auto _factorX = (m_NodeBounding[NODE_BOUNDING_RIGHT] - m_NodeBounding[NODE_BOUNDING_LEFT]) / m_GridSize;
        auto _factorY = (m_NodeBounding[NODE_BOUNDING_TOP] - m_NodeBounding[NODE_BOUNDING_BOTTOM]) / m_GridSize;

        auto _bboxRadius = glm::vec2(10000.0f, 0.0f);

        glm::vec2 point, latitude, longitude;
        glm::vec3 coords, _sphereCoords;

        point = glm::vec2(m_NodeBounding[NODE_BOUNDING_LEFT], m_NodeBounding[NODE_BOUNDING_TOP]);
        coords = mapCoordsToFace(m_NodeFace, point);
        _sphereCoords = getSphereCoords(coords);

        longitude.x = atan2(_sphereCoords.y, _sphereCoords.x);

        if(longitude.x < 0.0f)
        {
            longitude.x += 2.0f * 3.14f;
        }

        latitude.x = acos(_sphereCoords.z);

        point = glm::vec2(m_NodeBounding[NODE_BOUNDING_RIGHT], m_NodeBounding[NODE_BOUNDING_BOTTOM]);
        coords = mapCoordsToFace(m_NodeFace, point);
        _sphereCoords = getSphereCoords(coords);

        longitude.y = atan2(_sphereCoords.y, _sphereCoords.x);

        if(longitude.y < 0.0f)
        {
            longitude.y += 2.0f * 3.14f;
        }

        latitude.y = acos(_sphereCoords.z);

        // build surface vertex data
        auto index = 0;
        auto heightValue = 0.0f;
        auto oneDivNodeResolution = 1.0f / m_GridSize;

        glm::vec3 cubeCoords;

        for(float y = m_NodeBounding[NODE_BOUNDING_BOTTOM], b = 0.0f; y <= m_NodeBounding[NODE_BOUNDING_TOP]; y += _factorY, b += oneDivNodeResolution)
        {
            for(float x = m_NodeBounding[NODE_BOUNDING_LEFT], a = 0.0f; x <= m_NodeBounding[NODE_BOUNDING_RIGHT]; x += _factorX, a += oneDivNodeResolution)
            {
                TerrainVertex _tv;
                cubeCoords = mapCoordsToFace(m_NodeFace, glm::vec2(x, y));
                _tv.position = glm::vec4(getSphereCoords(cubeCoords) * m_Radius);
                _tv.texcoord = glm::vec2(a, b);
                m_Vertices.push_back(_tv);

                // compute AABB
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

        float d = powf(0.5f, getDepth());

        for(auto i = 0; i < m_Vertices.size(); ++i)
        {
            if(getDepth() > 0)
            {
                m_Vertices[i].texcoord = m_TexcoordOffset + m_Vertices[i].texcoord * d;
            }
        }

        m_VertexArray->bind();
        m_VertexBuffer->setData(m_Vertices.size() * sizeof(TerrainVertex), &m_Vertices[0]);
        m_VertexArray->unbind();
    }


    void QuadTreeNode::getBBox(glm::vec3 points[8])
    {
        points[0] = glm::vec3(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        points[1] = glm::vec3(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        points[2] = glm::vec3(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_TOP]);
        points[3] = glm::vec3(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_FRONT], m_GeoBounding[NODE_BOUNDING_TOP]);
        points[4] = glm::vec3(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        points[5] = glm::vec3(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_BOTTOM]);
        points[6] = glm::vec3(m_GeoBounding[NODE_BOUNDING_RIGHT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_TOP]);
        points[7] = glm::vec3(m_GeoBounding[NODE_BOUNDING_LEFT], m_GeoBounding[NODE_BOUNDING_BACK], m_GeoBounding[NODE_BOUNDING_TOP]);
    }


    float QuadTreeNode::getDistanceToBB(glm::vec3 & viewPoint, float geoBounding[6])
    {
        glm::vec3 points[8];
        float _distance = 10000.0f;

        getBBox(points);

        // check distance for each corner and return the lowest
        float tempD;

        for(int i = 0; i < 8; i++)
        {
            tempD = glm::distance(points[i], viewPoint);	// distance from camera to corner

            if(tempD < _distance)
            {
                _distance = tempD;
            }
        }

        return _distance;
    }


    unsigned int QuadTreeNode::getDepth()
    {
        if(hasParent())
        {
            return m_ParentNode->getDepth() + 1;
        }

        return 0;
    }


    glm::vec3 QuadTreeNode::getSphereCoords(glm::vec3 & position)
    {
        return glm::normalize(position);
    }


    bool QuadTreeNode::hasChildren()
    {
        return (m_ChildNodes[0] != NULL) && (m_ChildNodes[1] != NULL) && (m_ChildNodes[2] != NULL) && (m_ChildNodes[3] != NULL);
    }


    bool QuadTreeNode::hasParent()
    {
        return m_ParentNode != NULL;
    }


    glm::vec3 QuadTreeNode::mapCoordsToFace(unsigned int face, glm::vec2 & coords)
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
}
