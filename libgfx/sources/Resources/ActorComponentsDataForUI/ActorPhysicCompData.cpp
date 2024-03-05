#include "ActorPhysicCompData.h"

libgfx_API bool& ActorPhysicCompData::IsKinematic()
{
	return m_bIsKinematic;
}

libgfx_API bool& ActorPhysicCompData::IsGravityDisabled()
{
	return m_bIsGravityDisabled;
}

libgfx_API float& ActorPhysicCompData::GetMass()
{
	return m_mass;
}

libgfx_API bool& ActorPhysicCompData::IsRotationXLocked()
{
	return m_bIsRotationXLocked;
}

libgfx_API bool& ActorPhysicCompData::IsRotationYLocked()
{
	return m_bIsRotationYLocked;
}

libgfx_API bool& ActorPhysicCompData::IsRotationZLocked()
{
	return m_bIsRotationZLocked;
}

libgfx_API bool& ActorPhysicCompData::IsTrigger()
{
	return m_bIsTrigger;
}

void libgfx_API ActorPhysicCompData::SetKinematic(bool kinematic)
{
	m_bIsKinematic = kinematic;
}

void libgfx_API ActorPhysicCompData::SetGravityDisabled(bool gravityDisabled)
{
	m_bIsGravityDisabled = gravityDisabled;
}

void libgfx_API ActorPhysicCompData::SetMass(float mass)
{
	m_mass = mass;
}

void libgfx_API ActorPhysicCompData::SetRotationXLocked(bool rotationlocked)
{
	m_bIsRotationXLocked = rotationlocked;
}

void libgfx_API ActorPhysicCompData::SetRotationYLocked(bool rotationlocked)
{
	m_bIsRotationYLocked = rotationlocked;
}

void libgfx_API ActorPhysicCompData::SetRotationZLocked(bool rotationlocked)
{
	m_bIsRotationZLocked = rotationlocked;
}

void libgfx_API ActorPhysicCompData::SetTrigger(bool trigger)
{
	m_bIsTrigger = trigger;
}

std::vector<std::string>& ActorPhysicCompData::GetRigidTypes()
{
	return m_physicsType;
}

int ActorPhysicCompData::GetCurrentRigidType()
{
	return m_chosenPhysicType;
}

void ActorPhysicCompData::SetCurrentRigidType(int rigidType)
{
	m_chosenPhysicType = rigidType;
}

libgfx_API std::string& ActorPhysicCompData::GetShape()
{
	return m_shape;
}

void libgfx_API ActorPhysicCompData::SetShape(std::string shape)
{
	m_shape = shape;
}

libgfx_API LibMath::Vector3& ActorPhysicCompData::GetScalePhysic()
{
	return m_scalePhysic;
}

void libgfx_API ActorPhysicCompData::SetScalePhysic(LibMath::Vector3 scale)
{
	m_scalePhysic = scale;
}
