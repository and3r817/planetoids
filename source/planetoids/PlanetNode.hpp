#pragma once

#ifndef _PLANETNODE_HPP
#define _PLANETNODE_HPP

#include "Camera.hpp"
#include "Error.hpp"
#include "IndexBuffer.hpp"
#include "Heightmap.hpp"
#include "Mesh.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "Noise.hpp"
#include "Texture2D.hpp"
#include "FrameBuffer.hpp"
#include "PBuffer.hpp"

namespace planetoids
{
    typedef enum
    {
        NODE_SECTOR_TOP_LEFT,
        NODE_SECTOR_TOP_RIGHT,
        NODE_SECTOR_BOTTOM_LEFT,
        NODE_SECTOR_BOTTOM_RIGHT
    } NodeSector;

    typedef enum
    {
        NODE_BOUNDING_LEFT,
        NODE_BOUNDING_BOTTOM,
        NODE_BOUNDING_RIGHT,
        NODE_BOUNDING_TOP,
        NODE_BOUNDING_FRONT,
        NODE_BOUNDING_BACK
    } NodeBounding;

    typedef enum
    {
        NODE_FRONT,
        NODE_BACK,
        NODE_LEFT,
        NODE_RIGHT,
        NODE_BOTTOM,
        NODE_TOP
    } NodeFace;

    typedef enum
    {
        NODE_CORNER_FRONT_LEFT_BOTTOM,
        NODE_CORNER_FRONT_LEFT_TOP,
        NODE_CORNER_FRONT_RIGHT_BOTTOM,
        NODE_CORNER_FRONT_RIGHT_TOP,
        NODE_CORNER_BACK_LEFT_BOTTOM,
        NODE_CORNER_BACK_LEFT_TOP,
        NODE_CORNER_BACK_RIGHT_BOTTOM,
        NODE_CORNER_BACK_RIGHT_TOP
    } NodeCorner;

    const unsigned int m_HeightmapSize = 4096;
    const float m_HeightScale = 0.009f;
    const float m_HeightBias = 0.997f;
    const float m_PlanetRadius = 6000.0f;
    const static unsigned int m_GridSize = 16;
    const static unsigned int m_MaxDepth = 8;

    class PlanetNode
    {
        public:
            PlanetNode(unsigned int face, std::shared_ptr<Shader> & textureShader, std::shared_ptr<FrameBuffer> & textureBuffer);
            virtual ~PlanetNode();

            void init();
            void render(std::shared_ptr<Camera> & camera, std::shared_ptr<Shader> & program, std::shared_ptr<IndexBuffer> & buffer);

            unsigned int verticesCount();
            unsigned int trianglesCount();
            unsigned int nodesCount();
            unsigned int renderedNodesCount();

        private:
            PlanetNode * m_ParentNode;
            PlanetNode * m_ChildNodes[4];

            bool renderNode;

            std::vector<TerrainVertex> m_Vertices;
            std::shared_ptr<VertexBuffer> m_VertexBuffer;
            bool m_VertexBufferCreated;

            std::shared_ptr<Heightmap> m_Heights;

            // Texturing
            std::shared_ptr<FrameBuffer> m_TextureBuffer;
            std::shared_ptr<Shader> m_TextureShader;
            std::shared_ptr<Texture2D> m_Texture;

            BoundingBox m_BoundingBox;

            float m_GeoBounding[6];
            float m_NodeBounding[4];

            unsigned int m_NodeFace;
            unsigned int m_NodeSector;

            glm::vec2 m_TexcoordOffset;

            float m_MaxScreenSpaceError;

            PlanetNode(unsigned int sector, PlanetNode * parent);

            void merge();
            void split();

            void createVertexBuffer();
            void createTexture();

            BoundingBox boundingBox();
            float distanceToBoundingBox(glm::vec3 & viewPoint);

            bool hasChildren();
            bool hasParent();
            unsigned int depth();

            glm::vec3 coordsToFace(unsigned int face, glm::vec2 & coords);
            glm::vec3 sphereCoords(glm::vec3 & position);

            float heightValue(glm::vec2 & coord);
    };
}

#endif

