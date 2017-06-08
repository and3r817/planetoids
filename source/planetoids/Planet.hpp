#pragma once

#ifndef _PLANET_HPP
#define _PLANET_HPP

#include "BaseObject.hpp"
#include "Camera.hpp"
#include "Error.hpp"
#include "IndexBuffer.hpp"
#include "Mesh.hpp"
#include "Noise.hpp"
#include "PlanetNode.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "FrameBuffer.hpp"
#include "Texture2D.hpp"
#include "Timer.hpp"


namespace planetoids
{
    class Planet : public BaseObject
    {
        public:
            Planet();
            virtual ~Planet();

            void render(std::shared_ptr<Camera> & camera);
            void genIndexBuffer();
            void genCloudsTexture();

            std::shared_ptr<Shader> getPlanetShader();

            bool hasAtmosphere();
            std::shared_ptr<Shader> getAtmosphereShader();

            void addTexture(std::shared_ptr<Texture2D> & texture);

            unsigned int verticesCount();
            unsigned int trianglesCount();
            unsigned int nodesCount();
            unsigned int renderedNodesCount();

        private:
            // Planet surface
            std::vector<PlanetNode *> m_ChildNodes;
            std::shared_ptr<IndexBuffer> m_IndexBuffer;

            std::shared_ptr<Shader> m_PlanetCurrProgram;
            std::shared_ptr<Shader> m_PlanetFromSpaceProgram;
            std::shared_ptr<Shader> m_PlanetFromAtmProgram;

            std::vector<std::shared_ptr<Texture2D>> m_Textures;

            // Planet surface
            std::shared_ptr<FrameBuffer> m_TextureBuffer;
            std::shared_ptr<Shader> m_TextureShader;
            std::shared_ptr<Texture2D> m_Texture;

            // Atmosphere
            bool m_HasAtmosphere;

            std::vector<std::shared_ptr<Mesh>> m_AtmosphereMesh;

            std::shared_ptr<Shader> m_AtmCurrProgram;
            std::shared_ptr<Shader> m_SkyFromSpaceProgram;
            std::shared_ptr<Shader> m_SkyFromAtmProgram;

            glm::vec3 m_InvWavelength;
            float m_AtmosphereRadius;
            float m_RayleighConst;
            float m_MieConst;
            float m_Scale;
            float m_ScaleDepth;
            unsigned int m_Samples;
            float m_AsymmetryFactor;

            // Clouds
            bool m_HasClouds;
            std::vector<std::shared_ptr<Mesh>> m_CloudsMesh;
            std::shared_ptr<Shader> m_CloudsProgram;
            float m_CloudsLevelRadius;

            // Water
            bool m_HasWater;
            std::vector<std::shared_ptr<Mesh>> m_WaterMesh;
            std::shared_ptr<Texture2D> m_WaterTexture;
            std::shared_ptr<Shader> m_WaterProgram;
            float m_WaterLevelRadius;
            float m_WaterTime;
    };
}

#endif
