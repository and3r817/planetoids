#pragma once

#ifndef _RENDEROBJECT_HPP
#define _RENDEROBJECT_HPP

#include "BaseObject.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
    class RenderObject : public BaseObject
    {
        public:
            RenderObject();
            virtual ~RenderObject();

            virtual void render(std::shared_ptr<Camera> & camera) = 0;
			virtual void update() = 0;

            std::shared_ptr<Shader> getShader();

            void setGeometry(std::shared_ptr<Mesh> & geometry);

            void setShader(std::shared_ptr<Shader> & program);

            void setMainTexture(std::shared_ptr<Texture2D> & mainTexture);

            void setNormalTexture(std::shared_ptr<Texture2D> & normalTexture);

        protected:
            std::shared_ptr<Mesh> m_Mesh;

            std::shared_ptr<Shader> m_Program;

            std::shared_ptr<Texture2D> m_MainTexture;
            std::shared_ptr<Texture2D> m_NormalTexture;

            bool m_CastShadow;
    };
}

#endif
