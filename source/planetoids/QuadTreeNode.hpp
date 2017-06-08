#pragma once

#ifndef _QUADTREENODE_HPP
#define _QUADTREENODE_HPP

#include "Camera.hpp"
#include "IndexBuffer.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

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

    const float m_Radius = 1200.0f;
    const static unsigned int m_GridSize = 8;
    const static unsigned int m_MaxDepth = 6;

    class QuadTreeNode
    {
        public:
            QuadTreeNode(unsigned int face);
            virtual ~QuadTreeNode();

            void init();

            void render(Camera * camera, Shader * program);

            unsigned int verticesCount();

            unsigned int trianglesCount();

            unsigned int nodesCount();

            unsigned int renderedNodesCount();

        private:
            QuadTreeNode * m_ParentNode;
            QuadTreeNode * m_ChildNodes[4];

            float m_NodeBounding[4];
            float m_GeoBounding[6];

            std::vector<TerrainVertex> m_Vertices;
            std::vector<unsigned short> m_Indices;

            VertexArray * m_VertexArray;
            VertexBuffer * m_VertexBuffer;
            IndexBuffer * m_IndexBuffer;

            float m_MaxScreenSpaceError;

            unsigned int m_NodeFace;
            unsigned int m_NodeSector;

            bool renderNode;

            glm::vec2 m_TexcoordOffset;
            float m_HeightValue;

            QuadTreeNode(QuadTreeNode * parent, unsigned int sector);

            void merge();

            void split();

            void createIndexBuffer();

            void createVertexBuffer();

            void getBBox(glm::vec3 points[8]);

            unsigned int getDepth();

            float getDistanceToBB(glm::vec3 & viewPoint, float geoBounding[6]);

            glm::vec3 getSphereCoords(glm::vec3 & position);

            bool hasChildren();

            bool hasParent();

            glm::vec3 mapCoordsToFace(unsigned int face, glm::vec2 & coords);
    };
}

#endif

