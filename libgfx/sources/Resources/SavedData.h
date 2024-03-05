#pragma once

#include <memory>
#include <string>
#include <unordered_map>

//User calls in engineeditor-> use of resourceMgr that uses saved data in libgfx
//Libgfx calls for load -> use of saved data in libgfx

class SavedResource
{
public:
	SavedResource() = default;
	virtual ~SavedResource() = default;
};

struct SavedData
{
	SavedData() = default;
	~SavedData() = default;
	SavedData(SavedData const&) = delete;
	SavedData& operator=(SavedData const&) = delete;

	static SavedData* GetInstance()
	{
		static SavedData newInstance;

		return &newInstance;
	}

	void RemoveData(std::string path);
	std::pair<SavedResource*, bool> GetResourceImpl(std::string_view path);
	std::unique_ptr<SavedResource> LoadResource(std::string_view path);
	std::unordered_map<std::string, std::unique_ptr<SavedResource>> m_savedResources;
};
