#include "SavedData.h"

#include <filesystem>

#include "LowRenderer/Model/Model.h"
#include "TextureLoader.h"
#include "Image.h"

//SavedData* SavedData::instance;

void SavedData::RemoveData(std::string path)
{
	m_savedResources.erase(path);
}

std::pair<SavedResource*, bool>  SavedData::GetResourceImpl(std::string_view path)
{
	std::filesystem::path fileName = path;

	std::string name = fileName.filename().string();
	auto [iterator, hasToInsert] = m_savedResources.emplace(std::make_pair(name, nullptr));
	if (hasToInsert)
	{
		iterator->second = LoadResource(path);
	}
	return std::make_pair(iterator->second.get(), hasToInsert);
}

std::unique_ptr<SavedResource> SavedData::LoadResource(std::string_view path)
{
	//todo substr on last dot
	std::string_view extenssion = path.substr(path.length() - 3, 3);
	if (extenssion == "obj" || extenssion == "fbx")
	{
		return std::make_unique<Model>(path.data());
	}
	else if (extenssion == "jpg" || extenssion == "png" || extenssion == "peg")
	{
		return std::make_unique<Image>(DataTexture::CreateTexture(path, DataTexture::RGBA, false));
	}
	return nullptr;
}