// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include<chrono>
#include<iostream>

//#define ChronoClock    std::chrono::steady_clock
//#define ChronoDuration std::chrono::duration<double, std::ratio<1>
/**
 * 
 */
class FWatch
{
    using ChronoClock = std::chrono::steady_clock;
    using ChronoDuration = std::chrono::duration<double, std::ratio<1>>;
    using ClockType = std::chrono::time_point<ChronoClock>;

    double Timer = 0;
    struct FClocks
    {
        ClockType TotalDuration;
        ClockType Stopwatch;
        ClockType Timer;
    };
    FClocks Clock;
    bool bTimerRunning = false;

public:
    FWatch();
    double GetProgramRunningDuration() const;
    void SetStopwatchToZero();
    double GetTimeElapsed() const;

    void SetTimer(double Duration);
    double GetTimeRemaining();
    bool TimerIsRunning();
};
