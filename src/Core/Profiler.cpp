#include "Profiler.h"


namespace gcle
{
	std::unordered_map<std::string, Task>* Profiler::m_tasks		= GCLE_NEW std::unordered_map<std::string, Task>();
	std::unordered_map<std::string, float32>* Profiler::m_tasksTime	= GCLE_NEW std::unordered_map<std::string, float32>();

	void Profiler::Clear()
	{
		m_tasks->clear();
		delete m_tasks;

		m_tasksTime->clear();
		delete m_tasksTime;
	}

	void Profiler::NewTask(std::string id, std::string message)
	{
		Task t{ message, std::chrono::high_resolution_clock::now() };
		m_tasks->emplace(id, t);

		if (m_tasksTime->find(id) != m_tasksTime->end())
			m_tasksTime->emplace(id, 0.0f);
	}

	void Profiler::EndTask(std::string id)
	{
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_tasks->at(id).Start).count(); 
		m_tasks->erase(id);

		float32 time = duration / 1000000.0f;
		(*m_tasksTime)[id] = time; 
	}
	float32 Profiler::GetTask(std::string id)
	{
		if (m_tasksTime->find(id) != m_tasksTime->end())
		{
			return (*m_tasksTime)[id];
		}

		return float32();
	}
}
