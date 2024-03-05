#include "CameraSystem.h"

#include "../ECS/World.h"

size_t CameraSystem::Register(Vector3 position, Vector3 front, Vector3 worldUp)
{
	size_t index = m_cameras.size();
	m_cameras.emplace_back(position, front, worldUp);
	return  index;
}

size_t CameraSystem::Register(Camera& camera)
{
	size_t index = m_cameras.size();
	m_cameras.push_back(camera);
	return  index;
}

void CameraSystem::RemoveCamera(size_t index, int idEntity)
{
	if (index != m_cameras.size() - 1)
	{
		std::iter_swap(m_cameras.begin() + index, m_cameras.end() - 1);
	};
	m_cameras.pop_back();
	if(idEntity != -1)
	{
		World::GetInstance().RemoveComponentID<Camera>(idEntity);
	}
}

Camera* CameraSystem::GetCamera(size_t index)
{
	auto* camera = &m_cameras[index];
	if(camera->m_bFreeIndex)
	{
		return  nullptr;
	}
	return camera;
}
