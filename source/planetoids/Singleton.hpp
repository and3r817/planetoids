#pragma once

#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

#include "Platform.hpp"

namespace planetoids
{
    template<class T>
    class Singleton
    {
        public:
            Singleton(void)
            {
                assert(singleton == 0);
                singleton = static_cast<T *>(this);
            }

            virtual ~Singleton(void)
            {
                singleton = 0;
            }
        public:
            static T & getSingleton()
            {
                assert(singleton);
                return * singleton;
            }

            static T * ptr()
            {
                return singleton;
            }

        protected:
            static T * singleton;
    };

    template<class T>
    T * Singleton<T>::singleton = 0;
}

#endif
