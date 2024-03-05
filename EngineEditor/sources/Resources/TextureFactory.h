#pragma once

#include "Resources/Resource.h"
#include <unordered_map>

struct TextureFactory
{
	std::unordered_map<std::string, Resources::Resource*> m_resources;
};
