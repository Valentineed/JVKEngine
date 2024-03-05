#pragma once
#include <cstdint>



#include "PhysixCollider.h"
#include "PhysixManager.h"
#include "Shapes/Shape.h"
#include "Vector/Vector3.h"
#include "PxShape.h"

struct Transform;

namespace LibMath {
	struct Quaternion;
}


class TriangleMesh;
struct TriangleMeshDescriptor;
class PhysixSystem;
enum class RigidType;
class Shape;

struct Collider
{
	Collider() = default;
	Collider(float nx, float ny, float nz, float dist, Transform& transform, size_t entityID);
	Collider(Shape& shape, RigidType type, Transform & transform, size_t entityID);
	Collider(TriangleMesh& triangleMesh, RigidType type, Transform & transform, size_t entityID);
	Collider(Shape& shape, RigidType type, const LibMath::Vector3 pos, const LibMath::Quaternion& rot, size_t entityID);
	Collider(TriangleMesh& triangleMesh, RigidType type, const LibMath::Vector3 pos, const LibMath::Quaternion& rot, size_t entityID);
	~Collider() = default;

	void SetKinematic(bool isKinematic);

	void ChangeType(RigidType type);
	LibMath::Vector3 GetScaleShape();
	RigidType GetRigidType();
	
	[[nodiscard]] LibMath::Vector3 GetLinearVelocity() const;
	[[nodiscard]] LibMath::Vector3 GetAngularVelocity() const;
	[[nodiscard]] float GetMass() const;

	void SetScale(LibMath::Vector3 scale);
	void SetLinearVelocity(const LibMath::Vector3& linVel) const;
	void SetAngularVelocity(const LibMath::Vector3& angVel)const;
	void ResetVelocity(bool x, bool y ,bool z) const;
	void DisableGravity(bool disable);
	void SetMass(float mass) const;
	void AddTorque(const LibMath::Vector3& force, const ForceMode& mode) const;
	void SetAngularDamping(float angDamp) const;
	void SetLinearDamping(float linDamp) const;
	void ClearTorque(const ForceMode& mode) const;
	void ClearForce(const ForceMode& mode) const;
	void ClearAllForce() const;
	void AddForce(const LibMath::Vector3& force, const ForceMode& mode) const;
	void SetMassSpaceInertiaTensor(const LibMath::Vector3& mass) const;
	void AddForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
	void AddForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
	void AddLocalForceAtPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;
	void AddLocalForceAtLocalPos(const LibMath::Vector3& force, const LibMath::Vector3& pos, const ForceMode& mode, bool wakeup = true) const;

	void LockRotation(bool lockX = false, bool lockY = false, bool lockZ = false);
	void LockLocation(bool lockX = false, bool lockY = false, bool lockZ = false);
	
	void PutToSleep();
	
	//Collision Settings
	void ShapeInShapePairIntersectionTests(bool active);
	void ShapeInSceneQueryTests(bool active);
	void TriggerCollision(bool bisTrigger);

	//Collision Callback
	template<typename pClass>
	void BindOnBeginContact(pClass* pclass, void(pClass::* method)(size_t));

	template<typename pClass>
	void BindOnEndContact(pClass* pclass, void(pClass::* method)(size_t));
	void UpdateTransform(const LibMath::Vector3& pos, const LibMath::Quaternion& quaternion);
	LibMath::Vector3 GetPosition();
	LibMath::Quaternion GetRotator();
	void Release();

	size_t idEntity = s_invalidIndex;
	size_t idSystem = s_invalidIndex;
	RigidType collisionType;

	bool m_bIsKinematic = false;
	bool m_bIsGravityDisabled = false;

	bool m_bIsTrigger = false;
	bool m_bIsQuery = false;
	bool m_bHaveIntersection = true;
	
	bool m_lockAngularX = false;
	bool m_lockAngularY = false;
	bool m_lockAngularZ = false;

	bool m_lockLinearX = false;
	bool m_lockLinearY = false;
	bool m_lockLinearZ = false;

	LibMath::Vector3 m_scale = {1.,1.f,1.f};
	bool m_bFree = false;
protected:
	static  const  size_t s_invalidIndex = SIZE_MAX;
private:
	PhysixSystem* m_system = nullptr;
	Physix::PhysixCollider* m_collider = nullptr;
	Shape* m_shape;
};

template<typename pClass>
inline void Collider::BindOnBeginContact(pClass* pclass, void(pClass::* method)(size_t))
{
	m_collider->BindOnBeginContact(pclass, method);
}

template<typename pClass>
inline void Collider::BindOnEndContact(pClass* pclass, void(pClass::* method)(size_t))
{
	m_collider->BindOnEndContact(pclass, method);
}
