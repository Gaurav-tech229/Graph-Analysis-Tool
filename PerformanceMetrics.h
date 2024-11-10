// File: include/PerformanceMetrics.h
#pragma once

#include <chrono>
#include <cstddef>

struct PerformanceMetrics {
    std::chrono::duration<double> executionTime;
    size_t nodesProcessed;
    size_t memoryUsed;  // in bytes

    PerformanceMetrics();
};