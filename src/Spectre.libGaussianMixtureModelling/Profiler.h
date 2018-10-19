#pragma once

#ifndef PROFILER
#define PROFILER

#define WARM_UP_NUM 0
#define RUNS_NUM 1
#define WARM_UP(x) for(unsigned i = 0; i < WARM_UP_NUM; i++) \
                        (x);

#include <intrin.h>
#include <iostream>

typedef unsigned long long uint64;

struct Profiler
{
    Profiler(const char* profilerName)
    {
        name = profilerName;
        total = 0;
        count = 0;
        measurement = 0;
    }

    ~Profiler()
    {
        unsigned long long frequency = 2600000; // Hz * 1000
        std::cout << "Profiler " << name << " result: "
            << total / (double)count / frequency / RUNS_NUM
            << " [ms]" << std::endl;
    }

    const char* name;
    uint64 total;
    uint64 count;
    uint64 measurement;
};

#define INIT(name) static Profiler profiler(name);
#define BEGIN() profiler.measurement = __rdtsc();
#define END() profiler.total += __rdtsc() - profiler.measurement; profiler.count++;
#else
#define INIT(name)
#define BEGIN()
#define END()
#endif