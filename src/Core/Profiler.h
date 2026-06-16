#pragma once


#include <chrono>
#include <string>
#include <unordered_map>

#include "Utils.h"

#define PROFILER_START(name, message) gcle::Profiler::NewTask(name, message)
#define PROFILER_END(name) gcle::Profiler::EndTask(name)
#define PROFILER_CLEAR(name) gcle::Profiler::Clear()

namespace gcle
{
    struct Task
    {
        std::string Name;
        std::chrono::high_resolution_clock::time_point Start;

        Task(std::string name, std::chrono::high_resolution_clock::time_point start) : Name(name), Start(start) {}
    };

    struct Profiler
    {
        static void Clear();

        static void NewTask(std::string id, std::string message = "");
        static void EndTask(std::string id);

    private:
        static std::unordered_map<std::string, Task>* m_tasks;
    };
}