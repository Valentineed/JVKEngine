#include "ActorCameraCompData.h"

libgfx_API LibMath::Vector3& ActorCameraCompData::GetCameraOffset()
{
	return m_cameraOffset;
}

void libgfx_API ActorCameraCompData::SetCameraOffset(LibMath::Vector3& cameraOffset)
{
	m_cameraOffset = cameraOffset;
}
