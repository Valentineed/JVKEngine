#pragma once
#include "../Components/Camera.h"
#include "../Components/Collider.h"
#include "../Components/SoundComponent.h"
#include "../Components/StaticMesh.h"
#include "../Components/Transform.h"
#include "../Components/Lights/DirectionalLight.h"
#include "../Components/Lights/PointLight.h"
#include "../Components/Lights/SpotLight.h"


class Scene;
class UIWindowWorldHierarchy;
class UIWindowComponents;

class ComponentsDataManager
{
public:
	ComponentsDataManager() = default;

	void SetDatasOfSelectedActor(Scene* scene, UIWindowWorldHierarchy& hierarchyWindow, UIWindowComponents& componentsWindow);

	void SetBasicSelectedActorData(UIWindowComponents& componentsWindow);

	void SetSelectedActorTransformComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorStaticMeshComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorPhysicComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorCameraComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorLightComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorScriptsComp(UIWindowComponents& componentsWindow);
	void SetSelectedActorSoundComp(UIWindowComponents& componentsWindow);

	void UpdateTransformData(UIWindowComponents& componentsWindow);
	void UpdateCameraData(UIWindowComponents& componentsWindow);
	void UpdateStaticMeshData(UIWindowComponents& componentsWindow);
	void UpdatePhysicData(UIWindowComponents& componentsWindow);
	void UpdateDirectionalLightData(UIWindowComponents& componentsWindow);
	void UpdateSpotLightData(UIWindowComponents& componentsWindow);
	void UpdatePointLightData(UIWindowComponents& componentsWindow);
	void UpdateSoundData(UIWindowComponents& componentsWindow);
	
	void RemoveComponentWhenAvailable(UIWindowComponents& componentsWindow);

private:
	Transform* m_SelectedActorTransformComponent = nullptr;
	StaticMesh* m_SelectedActorMeshComponent = nullptr;
	Collider* m_SelectedActorPhysicsComponent = nullptr;
	Camera* m_selectedActorCameraComponent = nullptr;
	DirectionalLight* m_selectedActorDirectionalLightComponent = nullptr;
	SpotLight* m_selectedActorSpotLightComponent = nullptr;
	PointLight* m_selectedActorPointLightComponent = nullptr;
	SoundComponent* m_selectedActorSoundComponent = nullptr;


	void DisableDisplayLightComponent(UIWindowComponents& componentsWindow);
	void DisplayDirectionalLight(UIWindowComponents& componentsWindow, size_t indexLight);
	void DisplaySpotLight(UIWindowComponents& componentsWindow, size_t indexLight);
	void DisplayPointLight(UIWindowComponents& componentsWindow, size_t indexLight);
};

