/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             31/12/2015
   File name:        systemmonitor.cpp
   
   File description: The implementation of 
   the system monitor class declared in
   systemmonitor.h
   ------------------------------------------ */

#include "systemmonitor.h"
#include <Windows.h>
#include <Psapi.h>
#include "util/logging.h"

#pragma comment (lib, "winmm.lib")

/* -------------
   Internal Vars
   ------------- */
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static HANDLE thisProcess;

/* --------------
   Static Methods
   -------------- */
uint64
SystemMonitor::getTimeMS()
{
    return timeGetTime();
}

uint64
SystemMonitor::getTimeS()
{
    return timeGetTime() / 1000;
}

/* --------------
   Public Methods
   -------------- */
SystemMonitor::SystemMonitor():
    
    m_fpsCounter(0U),
    m_memUsage(0U),
    m_cpuUsage(0U),
    m_numProcessors(0U),
    m_frames(0U),
    m_startTime(timeGetTime())
{
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    m_numProcessors = sysInfo.dwNumberOfProcessors;
    
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));
    
    thisProcess = GetCurrentProcess();
    GetProcessTimes(thisProcess, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

SystemMonitor::~SystemMonitor()
{

}

void
SystemMonitor::update()
{
    // FPS calculations
    m_frames++;
    uint64 currTime = timeGetTime();
    if (currTime >= (m_startTime + 1000))
    {
        m_fpsCounter = m_frames;
        m_startTime  = currTime;
        m_frames     = 0U;

        // Physical memory calculations
        MEMORYSTATUSEX memoryInfo = {};
        memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memoryInfo);

        DWORDLONG totalPhysicalMemory = memoryInfo.ullTotalPhys;

        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        SIZE_T memUsedByProcess = pmc.WorkingSetSize;

        real64 ratio = ((real64) memUsedByProcess / (real64) totalPhysicalMemory);
        m_memUsage = (uint32) (memUsedByProcess / 1e6);

        // CPU calculations
        FILETIME ftime, fsys, fuser;
        ULARGE_INTEGER now, sys, user;
        real64 percent;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));


        GetProcessTimes(thisProcess, &ftime, &ftime, &fsys, &fuser);
        memcpy(&sys, &fsys, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));
        percent = (real64) ((sys.QuadPart - lastSysCPU.QuadPart) +
            (user.QuadPart - lastUserCPU.QuadPart));
        percent /= (now.QuadPart - lastCPU.QuadPart);
        percent /= m_numProcessors;
        lastCPU = now;
        lastUserCPU = user;
        lastSysCPU = sys;

        m_cpuUsage = (uint32) (percent * 100);
    }
}

uint32
SystemMonitor::getFPS() logical_const
{
    return m_fpsCounter;
}

uint32
SystemMonitor::getMemUsage() logical_const
{
    return m_memUsage;
}

uint32
SystemMonitor::getCpuUsagePerc() logical_const
{
    return m_cpuUsage;
}
