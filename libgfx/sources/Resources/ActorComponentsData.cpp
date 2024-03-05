#include "ActorComponentsData.h"

libgfx_API ActorTransformCompData& ActorComponentsData::GetActorTransformCompData()
{
    return m_actorTransformCompData;
}

libgfx_API ActorStaticMeshCompData& ActorComponentsData::GetActorStaticMeshCompData()
{
	return m_actorStaticMeshCompData;
}

libgfx_API ActorPhysicCompData& ActorComponentsData::GetActorPhysicCompData()
{
	return m_actorPhysicCompData;
}

libgfx_API ActorCameraCompData& ActorComponentsData::GetActorCameraCompData()
{
	return m_actorCameraCompData;
}

libgfx_API ActorLightCompData& ActorComponentsData::GetActorLightCompData()
{
	return m_actorLightCompData;
}

libgfx_API ActorScriptCompData& ActorComponentsData::GetActorScriptCompData()
{
	return m_actorScriptData;
}

libgfx_API ActorSoundCompData& ActorComponentsData::GetActorSoundCompData()
{
	return m_actorSoundData;
}
