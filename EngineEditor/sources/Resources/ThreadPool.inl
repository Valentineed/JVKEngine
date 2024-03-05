
template<typename Callable, typename ...Args>
inline auto ThreadPool::EnQueue(Callable&& task, Args&& ...args)
{
	auto callable = std::bind(std::forward<Callable>(task), std::forward<Args>(args)...);
	auto workerPackage = std::make_shared<std::packaged_task<decltype(callable()) ()>>(callable);
	{
		std::unique_lock<std::mutex> local{ m_eventMutex };
		m_tasks.emplace([=]
			{
				(*workerPackage)();
			});//add in queue the new task
	}
	m_event.notify_one();
	return workerPackage->get_future();
}

//Care to write std::ref when passing arg needed to be referenced
template<class T, typename... Args>
inline auto ThreadPool::Enqueue(T&& task, Args&&... args)
{
	auto myTask = std::bind(std::forward<T>(task), std::forward<Args>(args)...);

	auto wrapper = std::make_shared<std::packaged_task<decltype(myTask()) ()>>(std::move(myTask));
	{
		std::unique_lock<std::mutex> lock{ m_eventMutex };
		m_tasks.emplace(std::move([=]
			{
				(*wrapper)();
			}));

	}

	m_event.notify_one();

	return wrapper->get_future();
}