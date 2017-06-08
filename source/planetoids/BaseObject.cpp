#include "BaseObject.hpp"

namespace planetoids
{
    BaseObject::BaseObject()
    {
        m_Position = glm::vec3(0.0f);
        m_Rotation = glm::quat();
        m_Scale = glm::vec3(1.0f);

        m_TransformMatrix = glm::mat4(1.0f);
    }


    BaseObject::~BaseObject()
    {
    }


    void BaseObject::translate(glm::vec3 & position)
    {
        m_Position += position;
    }


    void BaseObject::rotate(glm::quat & rotation)
    {
        m_Rotation = rotation * m_Rotation;
    }


	void BaseObject::rotate(glm::vec3 & rotation)
	{
		m_Rotation = glm::quat(rotation) * m_Rotation;
	}


	void BaseObject::lookAt(glm::vec3 & position, glm::vec3 & target, glm::vec3 & up)
	{
		setRotation(glm::quat_cast(glm::lookAt(position, target, up)));
	}


    void BaseObject::lookAt(glm::vec3 & target, glm::vec3 & up)
    {
        setRotation(glm::quat_cast(glm::lookAt(m_Position, target, up)));
    }


    void BaseObject::setPosition(glm::vec3 & position)
    {
        m_Position = position;
        //setPosition((position - m_Position) / m_Scale * glm::inverse(m_Rotation));
    }

    void BaseObject::setRotation(glm::quat & rotation)
    {
        m_Rotation = rotation;
        //setRotation(glm::inverse(m_Rotation) * rotation);
    }


	void BaseObject::setRotation(glm::vec3 & rotation)
	{
		m_Rotation = glm::quat(rotation);
		//setRotation(glm::inverse(m_Rotation) * rotation);
	}


    glm::vec3 BaseObject::position()
    {
        return m_Position;
    }


    glm::quat BaseObject::rotation()
    {
        return m_Rotation;
    }


    glm::vec3 BaseObject::scale()
    {
        return m_Scale;
    }


    glm::mat4 BaseObject::matrix()
    {
        return glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation);
    }


    glm::vec3 BaseObject::right()
    {
        return m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f);//m_TransformMatrix[0].xyz;
    }


    glm::vec3 BaseObject::up()
    {
        return m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f);//m_TransformMatrix[1].xyz;
    }


    glm::vec3 BaseObject::foward()
    {
        return m_Rotation * glm::vec3(0.0f, 0.0f, 1.0f);//m_TransformMatrix[2].xyz;
    }
}