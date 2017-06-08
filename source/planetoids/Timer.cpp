#include "Timer.hpp"

namespace planetoids
{
    Timer::Timer()
    {
		m_fBegin = 0.0;
		m_fEnd = 0.033;

		m_FramesCount = 0;

		m_DeltaTime = 0.0f;
		m_FramesPerSecond = 0.0f;
    }


    Timer::~Timer()
    {
    }


    void Timer::frameBegin()
    {
        m_fBegin = glfwGetTime();
    }


    void Timer::frameEnd()
    {
        m_fEnd = glfwGetTime();

        m_DeltaTime = (float)(m_fEnd - m_fBegin);
        m_FramesPerSecond = 1.0f / m_DeltaTime;
    }


    float Timer::deltaTime()
    {
        return m_DeltaTime;
    }


    float Timer::framesPerSecond()
    {
        return m_FramesPerSecond;
    }
}
