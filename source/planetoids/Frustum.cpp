#include "Frustum.hpp"

namespace planetoids
{
    Frustum::Frustum(glm::mat4 & pMat, glm::mat4 & mvMat)
    {
        extract(pMat, mvMat);
    }


    Frustum::~Frustum()
    {

    }


    void Frustum::extract(glm::mat4 & pMat, glm::mat4 & mvMat)
    {
		/*
        auto _clipMat = glm::mat4(0.0f);//pMat * mvMat;

        _clipMat[0][0] = mvMat[0][0] * pMat[0][0] + mvMat[0][1] * pMat[1][0] + mvMat[0][2] * pMat[2][0] + mvMat[0][3] * pMat[3][0];
        _clipMat[1][0] = mvMat[0][0] * pMat[0][1] + mvMat[0][1] * pMat[1][1] + mvMat[0][2] * pMat[2][1] + mvMat[0][3] * pMat[3][1];
        _clipMat[2][0] = mvMat[0][0] * pMat[0][2] + mvMat[0][1] * pMat[1][2] + mvMat[0][2] * pMat[2][2] + mvMat[0][3] * pMat[3][2];
        _clipMat[3][0] = mvMat[0][0] * pMat[0][3] + mvMat[0][1] * pMat[1][3] + mvMat[0][2] * pMat[2][3] + mvMat[0][3] * pMat[3][3];
        _clipMat[0][1] = mvMat[1][0] * pMat[0][0] + mvMat[1][1] * pMat[1][0] + mvMat[1][2] * pMat[2][0] + mvMat[1][3] * pMat[3][0];
        _clipMat[1][1] = mvMat[1][0] * pMat[0][1] + mvMat[1][1] * pMat[1][1] + mvMat[1][2] * pMat[2][1] + mvMat[1][3] * pMat[3][1];
        _clipMat[2][1] = mvMat[1][0] * pMat[0][2] + mvMat[1][1] * pMat[1][2] + mvMat[1][2] * pMat[2][2] + mvMat[1][3] * pMat[3][2];
        _clipMat[3][1] = mvMat[1][0] * pMat[0][3] + mvMat[1][1] * pMat[1][3] + mvMat[1][2] * pMat[2][3] + mvMat[1][3] * pMat[3][3];
        _clipMat[0][2] = mvMat[2][0] * pMat[0][0] + mvMat[2][1] * pMat[1][0] + mvMat[2][2] * pMat[2][0] + mvMat[2][3] * pMat[3][0];
        _clipMat[1][2] = mvMat[2][0] * pMat[0][1] + mvMat[2][1] * pMat[1][1] + mvMat[2][2] * pMat[2][1] + mvMat[2][3] * pMat[3][1];
        _clipMat[2][2] = mvMat[2][0] * pMat[0][2] + mvMat[2][1] * pMat[1][2] + mvMat[2][2] * pMat[2][2] + mvMat[2][3] * pMat[3][2];
        _clipMat[3][2] = mvMat[2][0] * pMat[0][3] + mvMat[2][1] * pMat[1][3] + mvMat[2][2] * pMat[2][3] + mvMat[2][3] * pMat[3][3];
        _clipMat[0][3] = mvMat[3][0] * pMat[0][0] + mvMat[3][1] * pMat[1][0] + mvMat[3][2] * pMat[2][0] + mvMat[3][3] * pMat[3][0];
        _clipMat[1][3] = mvMat[3][0] * pMat[0][1] + mvMat[3][1] * pMat[1][1] + mvMat[3][2] * pMat[2][1] + mvMat[3][3] * pMat[3][1];
        _clipMat[2][3] = mvMat[3][0] * pMat[0][2] + mvMat[3][1] * pMat[1][2] + mvMat[3][2] * pMat[2][2] + mvMat[3][3] * pMat[3][2];
        _clipMat[3][3] = mvMat[3][0] * pMat[0][3] + mvMat[3][1] * pMat[1][3] + mvMat[3][2] * pMat[2][3] + mvMat[3][3] * pMat[3][3];
		*/
				auto _clipMat = pMat * mvMat;

        // FP_LEFT plane
        m_Planes[FP_LEFT].x = _clipMat[3][0] + _clipMat[0][0];
        m_Planes[FP_LEFT].y = _clipMat[3][1] + _clipMat[0][1];
        m_Planes[FP_LEFT].z = _clipMat[3][2] + _clipMat[0][2];
        m_Planes[FP_LEFT].w = _clipMat[3][3] + _clipMat[0][3];
		
        // FP_RIGHT plane
        m_Planes[FP_RIGHT].x = _clipMat[3][0] - _clipMat[0][0];
        m_Planes[FP_RIGHT].y = _clipMat[3][1] - _clipMat[0][1];
        m_Planes[FP_RIGHT].z = _clipMat[3][2] - _clipMat[0][2];
        m_Planes[FP_RIGHT].w = _clipMat[3][3] - _clipMat[0][3];
		
        // FP_BOTTOM plane
        m_Planes[FP_BOTTOM].x = _clipMat[3][0] + _clipMat[1][0];
        m_Planes[FP_BOTTOM].y = _clipMat[3][1] + _clipMat[1][1];
        m_Planes[FP_BOTTOM].z = _clipMat[3][2] + _clipMat[1][2];
        m_Planes[FP_BOTTOM].w = _clipMat[3][3] + _clipMat[1][3];
		
        // FP_TOP plane
        m_Planes[FP_TOP].x = _clipMat[3][0] - _clipMat[1][0];
        m_Planes[FP_TOP].y = _clipMat[3][1] - _clipMat[1][1];
        m_Planes[FP_TOP].z = _clipMat[3][2] - _clipMat[1][2];
        m_Planes[FP_TOP].w = _clipMat[3][3] - _clipMat[1][3];
		
        // FP_NEAR plane
        m_Planes[FP_NEAR].x = _clipMat[3][0] + _clipMat[2][0];
        m_Planes[FP_NEAR].y = _clipMat[3][1] + _clipMat[2][1];
        m_Planes[FP_NEAR].z = _clipMat[3][2] + _clipMat[2][2];
        m_Planes[FP_NEAR].w = _clipMat[3][3] + _clipMat[2][3];
		
        // FP_FAR plane
        m_Planes[FP_FAR].x = _clipMat[3][0] - _clipMat[2][0];
        m_Planes[FP_FAR].y = _clipMat[3][1] - _clipMat[2][1];
        m_Planes[FP_FAR].z = _clipMat[3][2] - _clipMat[2][2];
        m_Planes[FP_FAR].w = _clipMat[3][3] - _clipMat[2][3];
		
        for(int i = 0; i < 6; i++)
        {
            m_Planes[i] = glm::normalize(m_Planes[i]);
        }
    }


    bool Frustum::intersects(BoundingBox boundingBox)
    {
        for(auto i = 0; i < 6; ++i)
        {
            float distance = m_Planes[i].x * boundingBox.center.x + m_Planes[i].y * boundingBox.center.y + m_Planes[i].z * boundingBox.center.z + m_Planes[i].w;

            if(distance < -boundingBox.radius.x)
            {
                //return false;
            }
        }

        return true;
    }
}