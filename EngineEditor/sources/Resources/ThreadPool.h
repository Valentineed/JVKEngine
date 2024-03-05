#pragma once
#include <condition_variable> 
#include <future>
#include <queue>
#include <vector>
#include <thread>

class ThreadPool
{
public:
	template <typename T>
	using PackagedTask = std::packaged_task<T>;
	using Task = PackagedTask<void()>;

	enum class PoolSize
	{
		Hardware = 0,
		HardwareMinusOne = -1
	};
	explicit ThreadPool(PoolSize poolSize);

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;

	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	~ThreadPool()
	{
		Stop();
	}

	template<typename Callable, typename... Args>
	auto EnQueue(Callable&& task, Args&&... args);

	template<class T, typename... Args>
	auto Enqueue(T&& task, Args&&... args);

	bool HasTasks() const;
private:
	void Work();
	void Start(size_t numbrThreads);
	void Stop();

	std::vector<std::thread> m_threads;
	std::queue<Task> m_tasks;

	std::condition_variable m_event;
	mutable std::mutex m_eventMutex;
	bool m_stop = false;

};

#include "ThreadPool.inl"
