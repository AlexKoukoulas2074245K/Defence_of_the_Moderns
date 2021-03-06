/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             31/12/2015
   File name:        systemmonitor.h
   
   File description: A class representing
   a monitor for the current system
   ------------------------------------------ */

#pragma once

#include "dotmdef.h"

class SystemMonitor
{
public:
    
    static uint64
    getTimeMS();

    static uint64
    getTimeS();

public:

    SystemMonitor();
    
    ~SystemMonitor();

    SystemMonitor(const SystemMonitor& rhs) = delete;

    SystemMonitor&
    operator = (const SystemMonitor& rhs) = delete;

    void
    update();

    void
    loglap(const uint64 startTime,
           uint64 (*timefunc)()) logical_const;

    uint32
    getFPS() logical_const;

    uint32
    getMemUsage() logical_const;

    uint32
    getCpuUsagePerc() logical_const;

private:

    uint32 m_fpsCounter;
    uint32 m_memUsage;
    uint32 m_cpuUsage;
    uint32 m_numProcessors;
    uint32 m_frames;    
    uint64 m_startTime;

};