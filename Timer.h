// File: include/Timer.h
#pragma once

#include <chrono>
#include <string>
#include "PerformanceMetrics.h"

class GraphAnalysisTool; // Forward declaration

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::string operation;
    GraphAnalysisTool& tool;
    size_t initialMemory;

public:
    Timer(std::string op, GraphAnalysisTool& t);
    ~Timer();
};