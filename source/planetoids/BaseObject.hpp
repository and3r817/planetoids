#pragma once

#ifndef _BASEOBJECT_HPP
#define _BASEOBJECT_HPP

#include "Platform.hpp"

namespace planetoids
{
    class BaseObject
    {
        public:
            BaseObject();
            virtual ~BaseObject();

            void translate(glm::vec3 & position);
            void rotate(glm::quat & rotation);
			void rotate(glm::vec3 & rotation);
			void lookAt(glm::vec3 & position, glm::vec3 & target, glm::vec3 & up);
            void lookAt(glm::vec3 & target, glm::vec3 & up);

            void setPosition(glm::vec3 & position);
            void setRotation(glm::quat & rotation);
			void setRotation(glm::vec3 & rotation);

            glm::vec3 position();
            glm::quat rotation();
            glm::vec3 scale();
            glm::mat4 matrix();

            glm::vec3 right();
            glm::vec3 up();
            glm::vec3 foward();

        protected:
            glm::vec3 m_Position;
            glm::quat m_Rotation;
            glm::vec3 m_Scale;

            glm::mat4 m_TransformMatrix;
    };
}

#endif
