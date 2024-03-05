#pragma once
#include <stack>
#include <vector>

#include "../Components/Collider.h"
#include "../ECS/System.h"

class PhysixSystem : public System
{
public:
	void Update(float deltaTime);
	void End();
	size_t Register(Collider& collider);
	size_t RegisterDefaultShape(Shape shape, RigidType type, Transform* transform, size_t entityID);
	size_t RegisterDefaultTriangleMesh(TriangleMesh& triangleMesh, RigidType type, Transform* transform, size_t entityID);
	void RemoveComponent(size_t index, int idEntity);
	void UpdateAllTransform();
	~PhysixSystem();
	std::vector<Collider>& MutCollider() { return m_colliders; }

	Collider* GetCollider(size_t index);
	void Init();

private:
	std::vector<Collider> m_colliders;
};
