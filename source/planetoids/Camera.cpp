#include "Camera.hpp"

namespace planetoids
{
    Camera::Camera() : BaseObject()
    {
		m_Frustum = std::make_shared<Frustum>(m_ProjectionMatrix, m_ViewMatrix);

        update();
    }


    Camera::~Camera()
    {
    }


    void Camera::perspective(float fov, float aspect, float zNear, float zFar)
    {
        m_Fov = fov;
        m_AspectRatio = aspect;
        m_NearClipPlane = zNear;
        m_FarClipPlane = zFar;

        m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearClipPlane, m_FarClipPlane);
    }


    void Camera::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }


    std::shared_ptr<Frustum> Camera::frustum()
    {
		m_Frustum->extract(m_ProjectionMatrix, viewMatrix());

        return m_Frustum;
    }


    glm::mat4 Camera::projMatrix()
    {
        return m_ProjectionMatrix;
    }


    glm::mat4 Camera::viewMatrix()
    {
		return glm::mat4_cast(glm::inverse(m_Rotation)) * glm::translate(glm::mat4(1.0f), -m_Position);
    }


	void Camera::update()
	{
		m_Frustum->extract(projMatrix(), viewMatrix());
	}
}