
#pragma once

#include <Windows.h>

class PerformanceMonitor
{
public:
    PerformanceMonitor() = default;

    void Initialize(int screenWidth, int screenHeight);

    void Update(
        int fps,
        int cpu,
        float deltaTime
    );

    int GetFPS() const;
    int GetCPUUsage() const;

    float GetFrameTime() const;
    float GetMemoryUsageMB() const;

    int GetScreenWidth() const;
    int GetScreenHeight() const;

    void SetObjectCount(int count);
    void SetPolygonCount(int count);

    int GetObjectCount() const;
    int GetPolygonCount() const;

private:
    void UpdateMemoryUsage();

private:
    int m_FPS = 0;
    int m_CPUUsage = 0;

    float m_FrameTime = 0.0f;
    float m_MemoryUsageMB = 0.0f;

    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;

    int m_ObjectCount = 0;
    int m_PolygonCount = 0;

    float m_SampleTime = 0.0f;
    float m_FrameTimeSum = 0.0f;
    int m_FrameSamples = 0;
};
