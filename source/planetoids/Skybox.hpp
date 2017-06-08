#pragma once

#ifndef _SKYBOX_HPP
#define _SKYBOX_HPP

#include "BaseObject.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Cubemap.hpp"
#include "Camera.hpp"

namespace planetoids
{
    class Skybox : public BaseObject
    {
        public:
            Skybox(std::shared_ptr<Cubemap> & cubemap);
            virtual ~Skybox();

            void render(std::shared_ptr<Camera> & camera);

        private:
			std::shared_ptr<Cubemap> m_Cubemap;
			std::shared_ptr<Shader> m_Program;

            std::vector<Vertex> m_Vertices;
			std::vector<unsigned short> m_Indices;

			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}

#endif