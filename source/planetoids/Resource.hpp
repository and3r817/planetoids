#pragma once

#ifndef _RESOURCE_HPP
#define _RESOURCE_HPP

#include "Platform.hpp"

namespace planetoids
{
    class Resource
    {
        public:
            Resource();
            virtual ~Resource();

            unsigned int getId() const;

            std::string getName() const;

            bool getLoaded() const;

            void setName(std::string val);

        protected:
            unsigned int m_Id;
            std::string m_Name;
            bool m_Loaded;
			bool m_IsActive;
    };
}

#endif