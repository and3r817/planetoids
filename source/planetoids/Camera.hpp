#pragma once

#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "BaseObject.hpp"
#include "Frustum.hpp"
#include "Platform.hpp"

namespace planetoids
{
    class Camera : public BaseObject
    {
        public:
            Camera();
            virtual ~Camera();

            void perspective(float fov, float aspect, float zNear, float zFar);
            void ortho(float left, float right, float bottom, float top, float zNear, float zFar);

            glm::mat4 projMatrix();
            glm::mat4 viewMatrix();

            void update();

            std::shared_ptr<Frustum> frustum();

        private:
            float m_AspectRatio;
            float m_FarClipPlane;
            float m_Fov;
            float m_NearClipPlane;

            glm::mat4 m_ProjectionMatrix;
            glm::mat4 m_ViewMatrix;

            std::shared_ptr<Frustum> m_Frustum;
    };
}

#endif
