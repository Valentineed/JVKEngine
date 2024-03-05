#include "PhysixSystem.h"

#include "Controller.h"
#include "PhysixManager.h"
#include "TransformSystem.h"
#include "../ECS/World.h"
#include "../Scene/SceneManager.h"
#include "Actors/RigidType.h"
#include "../Scene/Actor.h"

void PhysixSystem::Update(float deltaTime)
{
	UpdateAllTransform();
	
	PhysixManager::GetInstance()->Update(deltaTime);
	
	for (auto& collider : m_colliders)
	{
		if(collider.collisionType == RigidType::DYNAMIC)
		{
			//Get Transform in PhysX
			Vector3 pos = collider.GetPosition();
			Quaternion rot = collider.GetRotator();
			auto myActor = SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, collider.idEntity);
			if(!myActor->GetTransform()->bModify)
			{
				myActor->GetTransform()->SetLocalPosition(pos);
				myActor->GetTransform()->SetLocalRotation(rot);
			}
		}
	}
}

void PhysixSystem::End()
{
	for (auto& collider : m_colliders)
	{
		if (collider.collisionType == RigidType::DYNAMIC)
		{
			collider.PutToSleep();
		}
	}
}

size_t PhysixSystem::Register(Collider& collider)
{
	size_t index = m_colliders.size();
	m_colliders.push_back(std::move(collider));
	return index;
}

size_t PhysixSystem::RegisterDefaultShape(Shape shape, RigidType type, Transform* transform, size_t entityID)
{
	size_t index = m_colliders.size();
	m_colliders.emplace_back(shape, type, *transform, entityID);
	return  index;
}

size_t PhysixSystem::RegisterDefaultTriangleMesh(TriangleMesh& triangleMesh, RigidType type, Transform* transform, size_t entityID)
{
	size_t index = m_colliders.size();
	m_colliders.emplace_back(triangleMesh, type, *transform, entityID);
	return  index;
}

void PhysixSystem::RemoveComponent(size_t index, int idEntity)
{
	if (m_colliders.empty())
	{
		return;
	}
	
	if (index != m_colliders.size() - 1)
	{
		m_colliders.back().idSystem = m_colliders[index].idSystem;
		std::iter_swap(m_colliders.begin() + index, m_colliders.end() - 1);
	}
	m_colliders.back().Release();
	m_colliders.pop_back();
	if(idEntity != -1)
	{
		World::GetInstance().RemoveComponentID<Collider>(idEntity);
	}
}

void PhysixSystem::UpdateAllTransform()
{
	for (auto& collider : m_colliders)
	{
		auto t = SceneManager::GetInstance().GetScene()->m_root;
		auto myActor = SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, collider.idEntity);

		auto transform = myActor->GetTransform();

		collider.UpdateTransform(transform->GetWorldLocation(), transform->GetWorldRotation());
	}
}

PhysixSystem::~PhysixSystem()
{
	for (auto& collider : m_colliders)
	{
		if(!collider.m_bFree)
		{
			collider.Release();
		}
	}
}

Collider* PhysixSystem::GetCollider(size_t index)
{
	return &m_colliders[index];
}

void PhysixSystem::Init()
{
	PhysixManager::GetInstance()->Init(true, 10);
}
