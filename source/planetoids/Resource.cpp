#include "Resource.hpp"

namespace planetoids
{
	Resource::Resource()
    {
        m_Id = 0;

        m_Name = "";
        m_Loaded = false;
		m_IsActive = false;
    }


    Resource::~Resource()
    {
    }


    unsigned int Resource::getId() const
    {
        return m_Id;
    }


    std::string Resource::getName() const
    {
        return m_Name;
    }


    bool Resource::getLoaded() const
    {
        return m_Loaded;
    }


    void Resource::setName(std::string val)
    {
        m_Name = val;
    }
}