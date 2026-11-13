
#include "PerformanceMonitor.h"

#include <Psapi.h>
#include <algorithm>

#pragma comment(lib, "Psapi.lib")

void PerformanceMonitor::Initialize(
    int screenWidth,
    int screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;

    UpdateMemoryUsage();
}

void PerformanceMonitor::Update(
    int fps,
    int cpu,
    float deltaTime)
{
    m_FPS = fps;
    m_CPUUsage = cpu;

    if (deltaTime < 0.0f)
        return;

    m_SampleTime += deltaTime;
    m_FrameTimeSum += deltaTime * 1000.0f;
    m_FrameSamples++;

    // 0.5초마다 프레임 시간 평균 갱신
    if (m_SampleTime >= 0.5f)
    {
        m_FrameTime =
            m_FrameTimeSum / m_FrameSamples;

        UpdateMemoryUsage();

        m_SampleTime = 0.0f;
        m_FrameTimeSum = 0.0f;
        m_FrameSamples = 0;
    }
}

void PerformanceMonitor::UpdateMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS memoryInfo = {};

    if (GetProcessMemoryInfo(
        GetCurrentProcess(),
        &memoryInfo,
        sizeof(memoryInfo)))
    {
        m_MemoryUsageMB =
            static_cast<float>(
                memoryInfo.WorkingSetSize
                ) / (1024.0f * 1024.0f);
    }
}

int PerformanceMonitor::GetFPS() const
{
    return m_FPS;
}

int PerformanceMonitor::GetCPUUsage() const
{
    return m_CPUUsage;
}

float PerformanceMonitor::GetFrameTime() const
{
    return m_FrameTime;
}

float PerformanceMonitor::GetMemoryUsageMB() const
{
    return m_MemoryUsageMB;
}

int PerformanceMonitor::GetScreenWidth() const
{
    return m_ScreenWidth;
}

int PerformanceMonitor::GetScreenHeight() const
{
    return m_ScreenHeight;
}

void PerformanceMonitor::SetObjectCount(int count)
{
    m_ObjectCount = count;
}

void PerformanceMonitor::SetPolygonCount(int count)
{
    m_PolygonCount = count;
}

int PerformanceMonitor::GetObjectCount() const
{
    return m_ObjectCount;
}

int PerformanceMonitor::GetPolygonCount() const
{
    return m_PolygonCount;
}
