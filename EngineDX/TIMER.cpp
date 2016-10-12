#include "TIMER.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Init()
{
    INT64 frequency; // Tics per second
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    if (frequency == 0)
        return false;

    m_invFrequency = 1.0f / frequency;

    QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTime);
    m_fTime = 0;

    return true;
}

void Timer::Frame()
{
    INT64 curTime = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

    m_fTime += (curTime - m_prevTime) * m_invFrequency;

    m_prevTime = curTime;
}

float Timer::GetTime()
{
    return m_fTime;
}