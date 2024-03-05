#include "ResourcesManager.h"


#include "Sound.h"
#include "SoundManager.h"
#include "../Systems/RenderSystem.h"

namespace Resources
{
	ResourcesManager::ResourcesManager() : m_threadPool(ThreadPool::PoolSize::Hardware)
	{
	}

	ResourcesManager::~ResourcesManager()
	{
		ClearCache();
	}

	ThreadPool& ResourcesManager::GetThreadPool()
	{
		return m_threadPool;
	}

	void ResourcesManager::LoadModel(std::string_view objPath, std::string_view texturePath, std::shared_ptr<RenderSystem> mgr)
	{
		if (mgr == nullptr)
		{
			throw std::runtime_error("Rendering manager nullptr");
			return;
		}

		mgr->LoadTexturedModel(objPath, texturePath);
		//m_threadPool.EnQueue(&RenderSystem::LoadTexturedModel, mgr, objPath, texturePath);
	}

	void ResourcesManager::ClearCache()
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		//for (auto element : m_resources)
		//{
		//	delete element.second;
		//}
		m_resources.clear();
	}

	size_t ResourcesManager::GetResourceImpl(std::string_view const& path, std::shared_ptr<RenderSystem> mgr, bool isMultithread)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		auto [iterator, hasToInsert] = m_resources.emplace(std::make_pair(path, -1));
		if (hasToInsert)
		{
			iterator->second = LoadResource(path, mgr, isMultithread);
			//iterator->second = static_cast<size_t>(m_threadPool.EnQueue(&ResourcesManager::LoadResource, this, path, mgr, isMultithread).get());
		}
		return iterator->second;
	}

	size_t ResourcesManager::LoadResource(std::string_view const& path, std::shared_ptr<RenderSystem> mgr, bool isMultithread)
	{
		std::string_view extenssion = path.substr(path.length() - 3, 3);
		if (extenssion == "obj")
		{
			return mgr->LoadModel(path);
		}
		else if (extenssion == "jpg" || extenssion == "png" || extenssion == "peg")
		{
			return mgr->LoadTexture(path);
		}
		else if(extenssion == "waw" ||extenssion == "ogg" || extenssion == "mp3")
		{
			//SoundManager sm;// = SoundManager::GetInstance();
			//
			//return sm.CreateSound(path, SOUND_MODE, static_cast<Sound2D>(m_resources.at(m_resources.size() - 1)).m_sound);
		}
		return -1;
	}
}