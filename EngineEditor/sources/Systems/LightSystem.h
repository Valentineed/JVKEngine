#pragma once
#include <cassert>
#include <memory>
#include <stack>
#include <vector>

#include "../ECS/System.h"

class SpotLight;
class DirectionalLight;
class PointLight;
class LightManager;
struct LightComponent;

class LightSystem : public System
{
public:
	void Init();
	void Update();
	[[nodiscard]] size_t Register(PointLight& light);
	[[nodiscard]] size_t Register(DirectionalLight& light);
	[[nodiscard]] size_t Register(SpotLight& light);
	void RemoveLight(size_t index, int idEntity);

	template<typename T>
	[[nodiscard]] T* GetLight(size_t index);

	std::vector<size_t> m_commonModifyIndex;
	std::vector<size_t> m_attenuationModifIndex;
	std::vector<size_t> m_spotSettingsModifIndex;
private:
	LightManager* m_lightManager = nullptr;
	std::vector<std::unique_ptr<LightComponent>> m_lights;
	std::stack<size_t> m_freeIndex;
	size_t m_maxLight = 0;
	//keep all the indexes in which the light has been modified //TODO use dirty system in libgfx
	std::vector<size_t> modifyIndex;

	static  const  size_t s_invalidIndex = SIZE_MAX;
};

template<typename T>
inline T* LightSystem::GetLight(size_t index)
{
	assert(index < m_lights.size());
	
	return static_cast<T*>(m_lights[index].get());
}
