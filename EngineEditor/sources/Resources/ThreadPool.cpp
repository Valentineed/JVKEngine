#include "ThreadPool.h"
#include <mutex>


ThreadPool::ThreadPool(PoolSize poolSize)
{
	int numbrThreads = 0;
	switch (poolSize)
	{
	case PoolSize::Hardware:
		numbrThreads = (int)std::thread::hardware_concurrency();
		break;
	case PoolSize::HardwareMinusOne:
		numbrThreads = (int)std::thread::hardware_concurrency() - 1;
		break;
	default:
		numbrThreads = (int)poolSize;
		break;
	}
	numbrThreads = std::max(numbrThreads, 1);
	Start(numbrThreads);
}

bool ThreadPool::HasTasks() const
{
	std::lock_guard<std::mutex> lock(m_eventMutex);
	return !m_tasks.empty();
}

void ThreadPool::Start(size_t numbrThreads)
{
	for (int i = 0; i < numbrThreads; i++)
	{
		m_threads.emplace_back([this] {
			while (true)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock{ m_eventMutex };
					m_event.wait(lock, [this] {return m_stop || !m_tasks.empty(); });

					if (m_stop && m_tasks.empty())
						break;
					task = std::move(m_tasks.front());
					m_tasks.pop();//remove the element
				}

				task();
			}
			});
	}
}

void ThreadPool::Stop()
{
	{
		std::unique_lock<std::mutex> lock{ m_eventMutex };
		m_stop = true;
	}
	m_event.notify_all();

	for (auto &thread : m_threads)
	{
		thread.join();
	}
}
