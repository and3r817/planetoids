#include "Mesh.hpp"

namespace planetoids
{
    Mesh::Mesh(std::vector<Vertex> & vertices) : Resource()
    {
        m_Vertices = vertices;

        m_VertexBuffer = std::make_shared<VertexBuffer>();

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);
    }


    Mesh::Mesh(std::vector<Vertex> & vertices, std::vector<unsigned short> & indices) : Resource()
    {
        m_Vertices = vertices;
        m_Indices = indices;

        m_VertexBuffer = std::make_shared<VertexBuffer>();
        m_IndexBuffer = std::make_shared<IndexBuffer>();

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);
        m_IndexBuffer->setData(m_Indices.size(), m_Indices.size() * sizeof(unsigned short), &m_Indices[0]);
    }


    Mesh::~Mesh()
    {
        if(m_Vertices.size() > 0)
        {
            m_Vertices.clear();
        }

        if(m_Indices.size() > 0)
        {
            m_Indices.clear();
        }
    }


    void Mesh::render(std::shared_ptr<Shader> & program)
    {
        unsigned int pos_id = program->getAttributeId("attr_v3Position");
        unsigned int tex_id = program->getAttributeId("attr_v2Texcoord");
        unsigned int nor_id = program->getAttributeId("attr_v3Normal");

        if(m_Indices.size() > 0)
        {
            m_VertexBuffer->bind();
            m_IndexBuffer->bind();

            if(pos_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(pos_id));
                OPENGL_CALL(glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
            }

            if(tex_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(tex_id));
                OPENGL_CALL(glVertexAttribPointer(tex_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3))));
            }

            if(nor_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(nor_id));
                OPENGL_CALL(glVertexAttribPointer(nor_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2))));
            }

            OPENGL_CALL(glDrawElements(GL_TRIANGLES, m_IndexBuffer->elementsCount(), GL_UNSIGNED_SHORT, (void*)0));

            if(pos_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(pos_id));
            }

            if(tex_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(tex_id));
            }

            if(nor_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(nor_id));
            }

            m_VertexBuffer->unbind();
            m_IndexBuffer->unbind();
        }
        else
        {
            m_VertexBuffer->bind();

            if(pos_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(pos_id));
                OPENGL_CALL(glVertexAttribPointer(pos_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
            }

            if(tex_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(tex_id));
                OPENGL_CALL(glVertexAttribPointer(tex_id, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3))));
            }

            if(nor_id != -1)
            {
                OPENGL_CALL(glEnableVertexAttribArray(nor_id));
                OPENGL_CALL(glVertexAttribPointer(nor_id, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2))));
            }

            OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, m_VertexBuffer->elementsCount()));

            if(pos_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(pos_id));
            }

            if(tex_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(tex_id));
            }

            if(nor_id != -1)
            {
                OPENGL_CALL(glDisableVertexAttribArray(nor_id));
            }

            m_VertexBuffer->unbind();
        }
    }


    void Mesh::AddVertices(std::vector<Vertex> & val)
    {
        m_Vertices.reserve(m_Vertices.size() + val.size());
        m_Vertices.insert(m_Vertices.end(), val.begin(), val.end());

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);
    }


    void Mesh::addIndices(std::vector<unsigned short> & val)
    {
        m_Indices.reserve(m_Indices.size() + val.size());
        m_Indices.insert(m_Indices.end(), val.begin(), val.end());

        m_IndexBuffer->setData(m_Indices.size(), m_Indices.size() * sizeof(unsigned short), &m_Indices[0]);
    }


    void Mesh::setVertices(std::vector<Vertex> & val)
    {
        m_Vertices = val;

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);
    }


    void Mesh::setIndices(std::vector<unsigned short> & val)
    {
        m_Indices = val;

        m_IndexBuffer->setData(m_Indices.size(), m_Indices.size() * sizeof(unsigned short), &m_Indices[0]);
    }


    std::vector<Vertex> Mesh::vertices()
    {
        return m_Vertices;
    }


    std::vector<unsigned short> Mesh::indices()
    {
        return m_Indices;
    }


    std::shared_ptr<Mesh> Mesh::loadFromOBJ(std::string filename)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;


        FILE * file = fopen(filename.c_str(), "r");

        if(file)
        {
            while(1)
            {
                char lineHeader[128];
                int res = fscanf(file, "%s", lineHeader);

                if(res == EOF)
                {
                    break;
                }

                // else : parse lineHeader

                if(strcmp(lineHeader, "v") == 0)
                {
                    glm::vec3 vertex;
                    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                    temp_vertices.push_back(vertex);
                }
                else if(strcmp(lineHeader, "vt") == 0)
                {
                    glm::vec2 uv;
                    fscanf(file, "%f %f\n", &uv.x, &uv.y);
                    temp_uvs.push_back(uv);
                }
                else if(strcmp(lineHeader, "vn") == 0)
                {
                    glm::vec3 normal;
                    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                    temp_normals.push_back(normal);
                }
                else if(strcmp(lineHeader, "f") == 0)
                {
                    std::string vertex1, vertex2, vertex3;
                    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

                    if(matches != 9)
                    {
                        printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                        return false;
                    }

                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices    .push_back(uvIndex[0]);
                    uvIndices    .push_back(uvIndex[1]);
                    uvIndices    .push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                else
                {
                    // Probably a comment, eat up the rest of the line
                    char stupidBuffer[1000];
                    fgets(stupidBuffer, 1000, file);
                }

            }

            // For each vertex of each triangle
            for(unsigned int i = 0; i < vertexIndices.size(); i++)
            {

                // Get the indices of its attributes
                unsigned int vertexIndex = vertexIndices[i];
                unsigned int uvIndex = uvIndices[i];
                unsigned int normalIndex = normalIndices[i];

                // Get the attributes thanks to the index
                glm::vec3 vertex = temp_vertices[ vertexIndex - 1 ];
                glm::vec2 uv = temp_uvs[ uvIndex - 1 ];
                glm::vec3 normal = temp_normals[ normalIndex - 1 ];

                Vertex v;
                v.position = vertex;
                v.texcoord = uv;
                v.normal = normal;

                vertices.push_back(v);

            }
        }

        return std::make_shared<Mesh>(vertices);
    }


    std::shared_ptr<Mesh> Mesh::createFullscreenQuad()
    {
        std::vector<Vertex> vertices;

        Vertex v;

        v.position = glm::vec3(-1.0f, -1.0f, 0.0f);
        v.texcoord = glm::vec2(0.0f, 0.0f);
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, -1.0f, 0.0f);
        v.texcoord = glm::vec2(1.0f, 0.0f);
        vertices.push_back(v);

        v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
        v.texcoord = glm::vec2(0.0f, 1.0f);
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, -1.0f, 0.0f);
        v.texcoord = glm::vec2(1.0f, 0.0f);
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, 1.0f, 0.0f);
        v.texcoord = glm::vec2(1.0f, 1.0f);
        vertices.push_back(v);

        v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
        v.texcoord = glm::vec2(0.0f, 1.0f);
        vertices.push_back(v);

        return std::make_shared<Mesh>(vertices);
    }


    std::shared_ptr<Mesh> Mesh::createFullscreenQuad(std::vector<glm::vec2> & texcoords)
    {
        std::vector<Vertex> vertices;

        Vertex v;

        v.position = glm::vec3(-1.0f, -1.0f, 0.0f);
        v.texcoord = texcoords[0];
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, -1.0f, 0.0f);
        v.texcoord = texcoords[1];
        vertices.push_back(v);

        v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
        v.texcoord = texcoords[2];
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, -1.0f, 0.0f);
        v.texcoord = texcoords[3];
        vertices.push_back(v);

        v.position = glm::vec3(1.0f, 1.0f, 0.0f);
        v.texcoord = texcoords[4];
        vertices.push_back(v);

        v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
        v.texcoord = texcoords[5];
        vertices.push_back(v);

        return std::make_shared<Mesh>(vertices);
    }


    std::shared_ptr<Mesh> Mesh::createSpherePart(float radius, unsigned int segments, unsigned int face)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;

        auto twoDivSegments = 2.0f / segments;
        auto oneDivSegments = 1.0f / segments;

        for(auto y = -1.0f, b = 0.0f; y <= 1.0f; y += twoDivSegments, b += segments)
        {
            for(auto x = -1.0f, a = 0.0f; x <= 1.0f; x += twoDivSegments, a += segments)
            {
                Vertex v;

                switch(face)
                {
                    case 0:
                        v.normal = glm::normalize(glm::vec3(x, y, -1.0f));
                        v.position = v.normal * radius;
                        break;

                    case 1:
                        v.normal = glm::normalize(glm::vec3(y, x, 1.0f));
                        v.position = v.normal * radius;
                        break;

                    case 2:
                        v.normal = glm::normalize(glm::vec3(-1.0f, x, y));
                        v.position = v.normal * radius;
                        break;

                    case 3:
                        v.normal = glm::normalize(glm::vec3(1.0f, y, x));
                        v.position = v.normal * radius;
                        break;

                    case 4:
                        v.normal = glm::normalize(glm::vec3(y, -1.0f, x));
                        v.position = v.normal * radius;
                        break;

                    case 5:
                        v.normal = glm::normalize(glm::vec3(x, 1.0f, y));
                        v.position = v.normal * radius;
                        break;
                }

                v.texcoord = glm::vec2(a, b);
                vertices.push_back(v);
            }
        }

        for(auto i = 0; i < segments; ++i)
        {
            for(auto j = 0; j < segments; ++j)
            {
                indices.push_back(j + i * (segments + 1));
                indices.push_back(j + (i + 1) * (segments + 1));
                indices.push_back((j + 1) + (i + 1) * (segments + 1));

                indices.push_back(j + i * (segments + 1));
                indices.push_back((j + 1) + (i + 1) * (segments + 1));
                indices.push_back((j + 1) + i * (segments + 1));
            }
        }

        return std::make_shared<Mesh>(vertices, indices);
    }


    std::shared_ptr<Mesh> Mesh::createSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;

        auto R = 1.0f / (float)(rings - 1);
        auto S = 1.0f / (float)(sectors - 1);

        for(auto r = 0; r < rings; ++r)
        {
            for(auto s = 0; s < sectors; ++s)
            {
                Vertex v;

                auto y = glm::sin(-M_PI_2 + M_PI * r * R);
                auto x = glm::cos(2.0f * M_PI * s * S) * sin(M_PI * r * R);
                auto z = glm::sin(2.0f * M_PI * s * S) * sin(M_PI * r * R);

                v.position = glm::vec3(x, y, z) * radius;
                v.texcoord = glm::vec2(r * R, s * S);
                v.normal = glm::vec3(x, y, z);

                vertices.push_back(v);
            }
        }

        for(auto r = 0; r < rings; ++r)
        {
            for(auto s = 0; s < sectors; ++s)
            {
                indices.push_back(s + r * sectors);
                indices.push_back(s + (r + 1) * sectors);
                indices.push_back((s + 1) + (r + 1) * sectors);

                indices.push_back(s + r * sectors);
                indices.push_back((s + 1) + (r + 1) * sectors);
                indices.push_back((s + 1) + r * sectors);
            }
        }

        return std::make_shared<Mesh>(vertices, indices);
    }


    std::shared_ptr<Mesh> Mesh::createQuadSprite(float size)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;

        Vertex v;

        v.position = glm::vec3(-size, -size, 0.0f);
        v.texcoord = glm::vec2(0.0f, 0.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

        v.position = glm::vec3(size, -size, 0.0f);
        v.texcoord = glm::vec2(1.0f, 0.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

        v.position = glm::vec3(-size, size, 0.0f);
        v.texcoord = glm::vec2(0.0f, 1.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

        v.position = glm::vec3(size, -size, 0.0f);
        v.texcoord = glm::vec2(1.0f, 0.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

        v.position = glm::vec3(size, size, 0.0f);
        v.texcoord = glm::vec2(1.0f, 1.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

        v.position = glm::vec3(-size, size, 0.0f);
        v.texcoord = glm::vec2(0.0f, 1.0f);
        v.normal = glm::vec3(0.0f, 0.0f, -1.0);
        vertices.push_back(v);

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(1);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);

        return std::make_shared<Mesh>(vertices, indices);
    }
}