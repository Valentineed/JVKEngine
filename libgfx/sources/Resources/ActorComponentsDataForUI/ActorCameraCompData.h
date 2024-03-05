#pragma once
#include "Vector/Vector3.h"

#include <libgfx_Export.h>

class ActorCameraCompData
{
public:
	ActorCameraCompData() = default;

	//Getters
	libgfx_API LibMath::Vector3& GetCameraOffset();

	//Setters
	void libgfx_API SetCameraOffset(LibMath::Vector3& cameraOffset);
	
private:
	LibMath::Vector3 m_cameraOffset = { 0.f, 0.f, 1.f };
};

