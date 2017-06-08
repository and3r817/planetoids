#include "Skybox.hpp"

namespace planetoids
{
    const Vertex cubeVertices[24] =
    {
        // front
        {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        // back
        {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
        {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
        {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)},
        // top
        {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        // bottom
        {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
        // left
        {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        // right
        {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)}
    };


    const unsigned short cubeIndices[36] =
    {
        0, 3, 1,  1, 3, 2,
        4, 7, 5,  5, 7, 6,
        8, 11, 9,  9, 11, 10,
        12, 15, 13, 13, 15, 14,
        16, 19, 17, 17, 19, 18,
        20, 23, 21, 21, 23, 22
    };


    Skybox::Skybox(std::shared_ptr<Cubemap> & cubemap) : BaseObject()
    {
        m_Cubemap = cubemap;
        m_Program = std::make_shared<Shader>("data\\shaders\\Spacebox");

        for(auto i = 0; i < 24; ++i)
        {
            m_Vertices.push_back(cubeVertices[i]);
        }

        for(auto i = 0; i < 36; ++i)
        {
            m_Indices.push_back(cubeIndices[i]);
        }

        m_VertexBuffer = std::make_shared<VertexBuffer>();
        m_IndexBuffer = std::make_shared<IndexBuffer>();

        m_VertexBuffer->setData(m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);
        m_IndexBuffer->setData(m_Indices.size(), m_Indices.size() * sizeof(unsigned short), &m_Indices[0]);
    }

    Skybox::~Skybox()
    {
        m_Indices.clear();
        m_Vertices.clear();
    }


    void Skybox::render(std::shared_ptr<Camera> & camera)
    {
        m_Program->bind();

        auto M = glm::scale(glm::mat4(1.0f), glm::vec3(30000.0f));
        auto MVP = camera->projMatrix() * camera->viewMatrix() * M;

        m_Program->setMatrix("MVP", MVP);

        if(m_Cubemap)
        {
            m_Cubemap->bind();
            m_Program->setInteger("u_scCubemap", m_Cubemap->getId());
        }

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);

        m_VertexBuffer->bind();
        m_IndexBuffer->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        m_VertexBuffer->unbind();
        m_IndexBuffer->unbind();

        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);

        if(m_Cubemap)
        {
            m_Cubemap->unbind();
        }

        m_Program->unbind();
    }
}
