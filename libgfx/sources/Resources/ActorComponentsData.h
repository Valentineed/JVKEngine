#pragma once
#include <libgfx_Export.h>

#include "ActorComponentsDataForUI/ActorTransformCompData.h"
#include "ActorComponentsDataForUI/ActorStaticMeshCompData.h"
#include "ActorComponentsDataForUI/ActorPhysicCompData.h"
#include "ActorComponentsDataForUI/ActorCameraCompData.h"
#include "ActorComponentsDataForUI/ActorLightCompData.h"
#include "ActorComponentsDataForUI/ActorScriptCompData.h"
#include "ActorComponentsDataForUI/ActorSoundCompData.h"

class ActorComponentsData
{
public:
    ActorComponentsData() = default;

	//Transform Component
	libgfx_API ActorTransformCompData& GetActorTransformCompData();
	
	libgfx_API ActorStaticMeshCompData& GetActorStaticMeshCompData();
	
	libgfx_API ActorPhysicCompData& GetActorPhysicCompData();
	
	libgfx_API ActorCameraCompData& GetActorCameraCompData();
	
	libgfx_API ActorLightCompData& GetActorLightCompData();
	
	libgfx_API ActorScriptCompData& GetActorScriptCompData();
	
	libgfx_API ActorSoundCompData& GetActorSoundCompData();

	
private:

	//Transform component
	ActorTransformCompData m_actorTransformCompData;
	ActorStaticMeshCompData m_actorStaticMeshCompData;
	ActorPhysicCompData m_actorPhysicCompData;
	ActorCameraCompData m_actorCameraCompData;
	ActorLightCompData m_actorLightCompData;
	ActorScriptCompData m_actorScriptData;
	ActorSoundCompData m_actorSoundData;


};
 