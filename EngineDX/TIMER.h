#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>


class Timer
{
public:
    Timer();
    ~Timer();

    bool Init();
    void Frame();
    float GetTime();

private:
    float m_fTime;
    INT64 m_prevTime;
    float m_invFrequency;
};




#endif // !TIMER_H