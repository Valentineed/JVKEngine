#pragma once
//#include "Resources/Resource.h"
#include "ThreadPool.h"
#include <string>

class RenderSystem;

namespace Resources
{
	class ResourcesManager
	{
	public:
		~ResourcesManager();
		ResourcesManager(ResourcesManager const&) = delete;
		ResourcesManager& operator=(ResourcesManager const&) = delete;
		void ClearCache();
		
		static ResourcesManager* GetInstance()
		{
			static ResourcesManager newInstance;

			return &newInstance;
		}
		ThreadPool& GetThreadPool();
		//static ResourcesManager& GetInstance() { return m_instance; }

		//template <class T>
		size_t GetResource(std::string_view const& path, std::shared_ptr<RenderSystem> mgr, bool isMultithread = true)
		{
			return GetResourceImpl(path, mgr, isMultithread);
		}
		void LoadModel(std::string_view objPath, std::string_view texturePath, std::shared_ptr<RenderSystem> mgr);
	private:
		ResourcesManager();
		
		std::mutex m_mutex;
		ThreadPool m_threadPool;
		size_t GetResourceImpl(std::string_view const& path, std::shared_ptr<RenderSystem> mgr, bool isMultithread);
		size_t LoadResource(std::string_view const& path, std::shared_ptr<RenderSystem> mgr, bool isMultithread);

		std::unordered_map<std::string, size_t> m_resources;
	};
}