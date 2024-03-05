#pragma once

#include <libgfx_Export.h>
#include <string>
#include <vector>

#include "Vector/Vector3.h"


class ActorPhysicCompData
{
public:
	ActorPhysicCompData() = default;

	//Getters
	libgfx_API bool& IsKinematic();
	libgfx_API bool& IsGravityDisabled();
	libgfx_API float& GetMass();

	libgfx_API bool& IsRotationXLocked();

	libgfx_API bool& IsRotationYLocked();

	libgfx_API bool& IsRotationZLocked();

	libgfx_API bool& IsTrigger();

	//Setters
	void libgfx_API SetKinematic(bool kinematic);
	void libgfx_API SetGravityDisabled(bool gravityDisabled);
	void libgfx_API SetMass(float mass);

	void libgfx_API SetRotationXLocked(bool rotationlocked);

	void libgfx_API SetRotationYLocked(bool rotationlocked);

	void libgfx_API SetRotationZLocked(bool rotationlocked);

	void libgfx_API SetTrigger(bool trigger);
	
	libgfx_API std::vector<std::string>& GetRigidTypes();
	
	libgfx_API int GetCurrentRigidType();
	void libgfx_API SetCurrentRigidType(int rigidType);

	libgfx_API std::string& GetShape();
	void libgfx_API SetShape(std::string shape);

	libgfx_API LibMath::Vector3& GetScalePhysic();
	void libgfx_API SetScalePhysic(LibMath::Vector3 scale);
private:
	bool m_bIsKinematic = false;
	bool m_bIsGravityDisabled = false;
	
	bool m_bIsRotationXLocked = false;
	bool m_bIsRotationYLocked = false;
	bool m_bIsRotationZLocked = false;
	
	bool m_bIsTrigger = false;
	
	float m_mass = 0.f;

	std::vector<std::string> m_physicsType;
	int m_chosenPhysicType = 0;

	std::string m_shape;

	LibMath::Vector3 m_scalePhysic;
};

