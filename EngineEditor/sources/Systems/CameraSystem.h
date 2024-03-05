#pragma once
#include <stack>
#include <vector>

#include "../Components/Camera.h"
#include "../ECS/System.h"

class CameraSystem : public System
{
public:
	size_t Register(Vector3 position, Vector3 front, Vector3 worldUp);
	size_t Register(Camera& camera);

	std::vector<Camera>& MutCollider() { return m_cameras; }

	void RemoveCamera(size_t index, int idEntity);

	Camera* GetCamera(size_t index);

private:
	std::vector<Camera> m_cameras;
	std::stack<size_t> m_freeIndex;
};

