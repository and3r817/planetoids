#pragma once

#ifndef _MESH_HPP
#define _MESH_HPP

#include "Camera.hpp"
#include "Error.hpp"
#include "IndexBuffer.hpp"
#include "Platform.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

namespace planetoids
{
    class Mesh : public Resource
    {
        public:
			Mesh(std::vector<Vertex> & vertices);
            Mesh(std::vector<Vertex> & vertices, std::vector<unsigned short> & indices);
            ~Mesh();

            void render(std::shared_ptr<Shader> & program);

            void AddVertices(std::vector<Vertex> & val);
            void addIndices(std::vector<unsigned short> & val);

            void setVertices(std::vector<Vertex> & val);
            void setIndices(std::vector<unsigned short> & val);

            std::vector<Vertex> vertices();
            std::vector<unsigned short> indices();

        private:
            std::vector<Vertex> m_Vertices;
            std::vector<unsigned short> m_Indices;

            std::shared_ptr<VertexBuffer> m_VertexBuffer;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;

			bool m_HasTexcoords;
			bool m_HasNormals;

        public:
			static std::shared_ptr<Mesh> loadFromOBJ(std::string filename);

			static std::shared_ptr<Mesh> createFullscreenQuad();
			static std::shared_ptr<Mesh> createFullscreenQuad(std::vector<glm::vec2> & texcoords);
            static std::shared_ptr<Mesh> createSpherePart(float radius, unsigned int segments, unsigned int face);
            static std::shared_ptr<Mesh> createSphere(float radius, unsigned int rings, unsigned int sectors);
			static std::shared_ptr<Mesh> createQuadSprite(float size);
    };
}

#endif