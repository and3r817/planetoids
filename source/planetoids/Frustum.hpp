#pragma once

#ifndef _FRUSTUM_HPP
#define _FRUSTUM_HPP

#include "BoundingBox.hpp"
#include "Platform.hpp"

namespace planetoids
{
	typedef enum
	{
		FP_NEAR,
		FP_FAR,
		FP_LEFT,
		FP_RIGHT,
		FP_TOP,
		FP_BOTTOM
	} FrustumPlane;

    class Frustum
    {
        public:
            Frustum(glm::mat4 & pMat, glm::mat4 & mvMat);
            virtual ~Frustum();

			void extract(glm::mat4 & pMat, glm::mat4 & mvMat);

			bool intersects(BoundingBox boundingBox);
			//bool Intersects(BoundingSphere sphere);

	private:
		glm::vec4 m_Planes[6];
    };
}

#endif
