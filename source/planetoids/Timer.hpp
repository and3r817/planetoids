#pragma once

#ifndef _TIME_HPP
#define _TIME_HPP

#include "Platform.hpp"
#include "Singleton.hpp"

namespace planetoids
{
    class Timer : public Singleton<Timer>
    {
        public:
            Timer();
            virtual ~Timer();

            void frameBegin();
            void frameEnd();

            float deltaTime();
            float framesPerSecond();

        private:
            double m_fBegin;
            double m_fEnd;

			int m_FramesCount;

			float m_DeltaTime;
			float m_FramesPerSecond;
    };
}

#endif
