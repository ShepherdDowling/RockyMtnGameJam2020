// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/Watch.h"

FWatch::FWatch()
{
    Clock.TotalDuration = ChronoClock::now();
    Clock.Stopwatch = ChronoClock::now();
    Clock.Timer = ChronoClock::now();
}

double FWatch::GetProgramRunningDuration() const
{
    return std::chrono::duration_cast<ChronoDuration>(ChronoClock::now() - Clock.TotalDuration).count();
}

void FWatch::SetStopwatchToZero()
{
    Clock.Stopwatch = ChronoClock::now();
}

double FWatch::GetTimeElapsed() const
{
    return std::chrono::duration_cast<ChronoDuration>(ChronoClock::now() - Clock.Stopwatch).count();
}

void FWatch::SetTimer(double Duration)
{
    Timer = Duration;
    Clock.Timer = ChronoClock::now();
    bTimerRunning = true;
}

double FWatch::GetTimeRemaining()
{
    double TimeLeft = Timer - std::chrono::duration_cast<ChronoDuration>(ChronoClock::now() - Clock.Timer).count();
    if (TimeLeft < 0) 
    {
        TimeLeft = 0;
        bTimerRunning = false;
    }
    return TimeLeft;
}

bool FWatch::TimerIsRunning()
{
    if(!bTimerRunning)
        return false;

    return static_cast<bool>(GetTimeRemaining());
}

