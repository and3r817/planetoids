#include "BufferObject.hpp"

namespace planetoids
{
    BufferObject::BufferObject()
    {
		m_Id = 0;
    }


    BufferObject::~BufferObject()
    {
		m_Id = 0;
    }
	

	unsigned int BufferObject::getId()
	{
		return m_Id;
	}	
}	