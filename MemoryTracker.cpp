// File: src/MemoryTracker.cpp
#include "MemoryTracker.h"

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
#endif
#endif

size_t MemoryTracker::getCurrentMemoryUsage() {
    size_t memoryUsage = 0;
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        memoryUsage = pmc.WorkingSetSize;
    }
#elif defined(__APPLE__) && defined(__MACH__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        memoryUsage = info.resident_size;
    }
#elif defined(__linux__) || defined(__linux) || defined(linux)
    long rss = 0L;
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%*s%ld", &rss) == 1) {
            memoryUsage = rss * (size_t)sysconf(_SC_PAGESIZE);
        }
        fclose(fp);
    }
#endif
    return memoryUsage;
}