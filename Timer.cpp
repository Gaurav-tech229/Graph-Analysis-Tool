// File: src/Timer.cpp
#include "Timer.h"
#include "GraphAnalysisTool.h"
#include "MemoryTracker.h"

Timer::Timer(std::string op, GraphAnalysisTool& t)
    : operation(op), tool(t) {
    start = std::chrono::high_resolution_clock::now();
    initialMemory = MemoryTracker::getCurrentMemoryUsage();
}

Timer::~Timer() {
    auto end = std::chrono::high_resolution_clock::now();
    tool.metrics[operation].executionTime = end - start;
    tool.metrics[operation].memoryUsed =
        MemoryTracker::getCurrentMemoryUsage() - initialMemory;
}