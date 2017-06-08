#pragma once

#ifndef _SHADER_HPP
#define _SHADER_HPP

#include "Error.hpp"
#include "Platform.hpp"
#include "Resource.hpp"

namespace planetoids
{
    class Shader : public Resource
    {
        public:
            Shader(std::string name);
            virtual ~Shader();

            void bind();

            void unbind();

            unsigned int getAttributeId(std::string attribute);

            unsigned int getUniformId(std::string uniform);

            // TODO: add method
            bool hasUniform(std::string uniform);

            void setInteger(std::string uniform, unsigned int val);

            void setFloat(std::string uniform, float val);

            void setVector(std::string uniform, glm::vec3 & val);

            void setVector(std::string uniform, glm::vec4 & val);

            void setMatrix(std::string uniform, glm::mat4 & val);

        private:
            unsigned int load(std::string fileName, unsigned int shaderType);

            std::string readFromFile(std::string fileName);
    };
}

#endif