#include "Collider.h"

#include "PhysixCollider.h"
#include "PhysixManager.h"
#include "../ECS/World.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Components/Transform.h"
#include "PTransform.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Actor.h"
#include "Actors/RigidType.h"
#include "Forces/ForceMode.h"

Collider::Collider(float nx, float ny, float nz, float dist, Transform& transform, size_t entityID)
{
	m_system = World::GetInstance().GetSystem<PhysixSystem>();
	idEntity = entityID;
	const auto pos = transform.GetWorldLocation();
	const auto rot = transform.GetWorldRotation();
	m_collider = new Physix::PhysixCollider(nx, ny,nz, dist, { pos.x, pos.y, pos.z,rot.X,rot.Y,rot.Z,rot.W }, idEntity);
}

Collider::Collider(Shape& shape, RigidType type, Transform& transform, size_t entityID): collisionType(type)
{
	m_shape = new Shape(shape);
	m_system = World::GetInstance().GetSystem<PhysixSystem>();
	idEntity = entityID;
 	auto pos = transform.GetWorldLocation();
	auto rot = transform.GetWorldRotation();
	m_collider = new Physix::PhysixCollider(m_shape, type, { pos.x, pos.y, pos.z,rot.X,rot.Y,rot.Z,rot.W }, idEntity);
}

Collider::Collider(TriangleMesh& triangleMesh, RigidType type, Transform& transform, size_t entityID) : collisionType(type)
{
	m_system = World::GetInstance().GetSystem<PhysixSystem>();
	idEntity = entityID;
	auto pos = transform.GetWorldLocation();
	auto rot = transform.GetWorldRotation();
	m_collider = new Physix::PhysixCollider(triangleMesh, type, { pos.x, pos.y, pos.z,rot.X,rot.Y,rot.Z,rot.W }, idEntity);
}

Collider::Collider(Shape& shape, RigidType type, const LibMath::Vector3 pos,
	const LibMath::Quaternion& rot, size_t entityID) : collisionType(type)
{
	m_shape = new Shape(shape);
	m_system = World::GetInstance().GetSystem<PhysixSystem>();
	idEntity = entityID;
	m_collider = new Physix::PhysixCollider(m_shape, type, { pos.x, pos.y, pos.z,rot.X,rot.Y,rot.Z,rot.W }, idEntity);
}

Collider::Collider(TriangleMesh& triangleMesh, RigidType type, const LibMath::Vector3 pos, const LibMath::Quaternion& rot, size_t entityID) : collisionType(type)
{
        m_system = World::GetInstance().GetSystem<PhysixSystem>();
	idEntity = entityID;
	m_collider = new Physix::PhysixCollider(triangleMesh, type, { pos.x, pos.y, pos.z,rot.X,rot.Y,rot.Z,rot.W }, idEntity);
}

void Collider::SetKinematic(bool isKinematic)
{
	m_collider->SetKinematic(isKinematic);
	m_bIsKinematic = isKinematic;
}

void Collider::ChangeType(RigidType type)
{
	m_collider->ChangeType(type);
	collisionType = type;
}

RigidType Collider::GetRigidType()
{
	return collisionType;
}

LibMath::Vector3 Collider::GetScaleShape()
{
	return m_collider->GetScaleShape();
}

LibMath::Vector3 Collider::GetLinearVelocity() const
{
	return m_collider->GetLinearVelocity();
}

LibMath::Vector3 Collider::GetAngularVelocity() const
{
	return m_collider->GetAngularVelocity();
}

float Collider::GetMass() const
{
	return  m_collider->GetMass();
}

void Collider::SetScale(LibMath::Vector3 scale)
{
	m_scale = scale;
	m_collider->SetScale(m_scale);
}


void Collider::SetLinearVelocity(const LibMath::Vector3& linVel) const
{
	m_collider->SetLinearVelocity(linVel);
}

void Collider::SetAngularVelocity(const LibMath::Vector3& angVel) const
{
	m_collider->SetAngularVelocity(angVel);
}

void Collider::ResetVelocity(bool x, bool y, bool z) const
{
	if (x && y && z)
	{
		SetLinearVelocity({ 0.f, 0.f, 0.f });
		SetAngularVelocity({ 0.f, 0.f, 0.f });
	}
	else if (x && y)
	{
		SetLinearVelocity({ 0.f, 0.f, GetLinearVelocity().z });
		SetAngularVelocity({ 0.f, 0.f,  GetAngularVelocity().z });
	}
	else if (x && z)
	{
		SetLinearVelocity({ 0.f, GetLinearVelocity().y, 0.f });
		SetAngularVelocity({ 0.f, GetAngularVelocity().y, 0.f });
	}
	else if (y && z)
	{
		SetLinearVelocity({ GetLinearVelocity().x, 0.f, 0.f });
		SetAngularVelocity({ GetAngularVelocity().x, 0.f, 0.f });
	}
	else if (x)
	{
		SetLinearVelocity({ 0.f, GetLinearVelocity().y, GetLinearVelocity().z });
		SetAngularVelocity({ 0.f, GetAngularVelocity().y,  GetAngularVelocity().z });
	}
	else if (y)
	{
		SetLinearVelocity({ GetLinearVelocity().x,0.f,  GetLinearVelocity().z });
		SetAngularVelocity({ GetAngularVelocity().x,0.f,   GetAngularVelocity().z });
	}
	else if (z)
	{
		SetLinearVelocity({ GetLinearVelocity().x, GetLinearVelocity().y, 0.0f });
		SetAngularVelocity({ GetAngularVelocity().x, GetAngularVelocity().y,  0.0f});
	}
}

void Collider::DisableGravity(bool disable)
{
	m_collider->DisableGravity(disable);
	m_bIsGravityDisabled = disable;
}

void Collider::SetMass(float mass) const
{
	m_collider->SetMass(mass);
}

void Collider::AddTorque(const LibMath::Vector3& force, const ForceMode& mode) const
{
	m_collider->AddTorque(force, mode);
}

void Collider::SetAngularDamping(float angDamp) const
{
	m_collider->SetAngularDamping(angDamp);
}

void Collider::SetLinearDamping(float linDamp) const
{
	m_collider->SetLinearDamping(linDamp);
}

void Collider::ClearTorque(const ForceMode& mode) const
{
	m_collider->ClearTorque(mode);
}

void Collider::ClearForce(const ForceMode& mode) const
{
	m_collider->ClearForce(mode);
}

void Collider::ClearAllForce() const
{
	m_collider->ClearForce(FORCE);
	m_collider->ClearForce(IMPULSE);
	m_collider->ClearForce(ACCELERATION);
	m_collider->ClearForce(VELOCITY_CHANGE);
}

void Collider::SetMassSpaceInertiaTensor(const LibMath::Vector3& mass) const
{
	m_collider->SetMassSpaceInertiaTensor(mass);
}

void Collider::AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup) const
{
	m_collider->AddForceAtPos(force, pos, mode, wakeup);
}

void Collider::AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup) const
{
	m_collider->AddForceAtLocalPos(force, pos, mode, wakeup);
}

void Collider::AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode,
	bool wakeup) const
{
	m_collider->AddLocalForceAtPos(force, pos, mode, wakeup);
}

void Collider::AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos,
	const ForceMode& mode, bool wakeup) const
{
	m_collider->AddLocalForceAtLocalPos(force, pos, mode, wakeup);
}

void Collider::LockRotation(bool lockX, bool lockY, bool lockZ)
{
	m_lockAngularX = lockX;
	m_lockAngularY = lockY;
	m_lockAngularZ = lockZ;
	
	m_collider->LockRotation(m_lockAngularX, m_lockAngularY, m_lockAngularZ);
}

void Collider::LockLocation(bool lockX, bool lockY, bool lockZ)
{
	m_lockLinearX = lockX;
	m_lockLinearY = lockY;
	m_lockLinearZ = lockZ;
	m_collider->LockLocation(m_lockLinearX, m_lockAngularY, m_lockAngularZ);
}

void Collider::PutToSleep()
{
	if(m_collider->m_type == RigidType::STATIC)
	{
		return;
	}
	m_collider->PutToSleep();
}

void Collider::ShapeInShapePairIntersectionTests(bool active)
{
	m_bHaveIntersection = active;
	m_collider->ShapeInShapePairIntersectionTests(m_bHaveIntersection);
}

void Collider::ShapeInSceneQueryTests(bool active)
{
	m_bIsQuery = active;
	m_collider->ShapeInSceneQueryTests(m_bIsQuery);
}

void Collider::TriggerCollision(bool bisTrigger)
{
	m_bIsTrigger = bisTrigger;
	m_collider->TriggerCollision(m_bIsTrigger);
}

void Collider::UpdateTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& quaternion)
{
	m_collider->UpdateTransform(pos, quaternion);
}

LibMath::Vector3 Collider::GetPosition()
{
	return  m_collider->GetPosition();
}

LibMath::Quaternion Collider::GetRotator()
{
	return m_collider->GetRotator();
}

void Collider::Release()
{
	m_collider->Release();
}

void Collider::AddForce(const LibMath::Vector3& force, const ForceMode& mode) const
{
	m_collider->AddForce(force, mode);
}
