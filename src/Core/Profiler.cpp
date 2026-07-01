#include "Profiler.h"


namespace gcle
{
	std::unordered_map<std::string, Task>* Profiler::m_tasks = GCLE_NEW std::unordered_map<std::string, Task>();

	void Profiler::Clear()
	{
		m_tasks->clear();
		delete m_tasks;
	}

	void Profiler::NewTask(std::string id, std::string message)
	{
		Task t{ message, std::chrono::high_resolution_clock::now() };
		m_tasks->emplace(id, t);
	}

	uint32 Profiler::EndTask(std::string id)
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_tasks->at(id).Start).count();
		GCLE_INFO << "Your task : " << m_tasks->at(id).Name.c_str() << " took " << duration << " nanoseconds | " << duration / 1000000000.0f << " seconds" << ENDL;
		m_tasks->erase(id);
		return static_cast<uint32>(duration);
	}
}
