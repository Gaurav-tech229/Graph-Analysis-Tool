// File: include/MemoryTracker.h
#pragma once

#include <cstddef>

class MemoryTracker {
public:
    static size_t getCurrentMemoryUsage();
};