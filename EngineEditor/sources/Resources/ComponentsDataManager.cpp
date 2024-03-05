#include "ComponentsDataManager.h"
#include "UI/UIWindows/UIWindowWorldHierarchy.h"
#include "UI/UIWindows/UIWindowComponents.h"
#include "../Scene/Scene.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/LightSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/ScriptingSystem.h"
#include "../Systems/SoundSystem.h"


void ComponentsDataManager::SetDatasOfSelectedActor(Scene* scene, UIWindowWorldHierarchy& hierarchyWindow, UIWindowComponents& componentsWindow)
{
	if ((hierarchyWindow.GetActorSelectedID() != -1 && componentsWindow.GetActorIDToDisplay() != hierarchyWindow.GetActorSelectedID() && hierarchyWindow.GetActorSelectedID() != 0) || componentsWindow.m_bReloadActorComponentsData == true)
	{
		if (componentsWindow.m_bReloadActorComponentsData == true)
		{
			componentsWindow.m_bReloadActorComponentsData = false;
		}

		//Set the new actor to display in the components window
		componentsWindow.SetActorIDToDisplay(hierarchyWindow.GetActorSelectedID());
		componentsWindow.FindAndSetActorToDisplay(scene->m_root, hierarchyWindow.GetActorSelectedID());

		if (componentsWindow.GetActorSelected() != nullptr)
		{
			SetBasicSelectedActorData(componentsWindow);

			SetSelectedActorTransformComp(componentsWindow);
			
			SetSelectedActorStaticMeshComp(componentsWindow);

			SetSelectedActorPhysicComp(componentsWindow);

			SetSelectedActorCameraComp(componentsWindow);

			SetSelectedActorLightComp(componentsWindow);

			SetSelectedActorScriptsComp(componentsWindow);
			
			SetSelectedActorSoundComp(componentsWindow);
			
		}
	}
}

void ComponentsDataManager::SetBasicSelectedActorData(UIWindowComponents& componentsWindow)
{;
	strcpy_s(componentsWindow.GetActorSelected()->m_actorNameInput, componentsWindow.GetActorSelected()->m_name.data());
	//componentsWindow.GetActorSelected()->m_actorNameInput = componentsWindow.GetActorSelected()->m_name;
}

void ComponentsDataManager::SetSelectedActorTransformComp(UIWindowComponents& componentsWindow)
{
	m_SelectedActorTransformComponent = componentsWindow.GetActorSelected()->GetTransform();

	if (m_SelectedActorTransformComponent != nullptr)
	{
		//Tell the components window to display the transform component in the UI
		componentsWindow.m_bIsTransformComponentEligible = true;

		//Set the transform values of the actor
		componentsWindow.GetTransformComponentData().SetTransform(m_SelectedActorTransformComponent->GetLocalLocation(),
			m_SelectedActorTransformComponent->GetLocalRotation().EulerAngles(),
			m_SelectedActorTransformComponent->GetLocalScale());
	}
	else
	{
		componentsWindow.m_bIsTransformComponentEligible = false;

		m_SelectedActorTransformComponent = nullptr;
	}
}

void ComponentsDataManager::SetSelectedActorStaticMeshComp(UIWindowComponents& componentsWindow)
{
	auto meshComponentIdFound = World::GetInstance().GetComponentID<StaticMesh>(componentsWindow.GetActorSelected()->m_idEntity);

	if (meshComponentIdFound != -1)
	{
		//Try to find the static mesh component for the current selected actor
		m_SelectedActorMeshComponent = World::GetInstance().GetSystem<StaticMeshSystem>()->GetStaticMesh(meshComponentIdFound);
		//Tell the components window to display the static mesh component in the UI
		componentsWindow.m_bIsMeshComponentEligible = true;

		//	//Set the static meshes/meshes textures of the current selected actor
		componentsWindow.GetStaticMeshComponentData().SetStaticMeshName(m_SelectedActorMeshComponent->m_staticMeshName);
		componentsWindow.GetStaticMeshComponentData().SetStaticMeshTextureName(m_SelectedActorMeshComponent->m_staticMeshTextureName);


		componentsWindow.ChangeCurrentMeshTexturePreview(m_SelectedActorMeshComponent->m_currentTexturePath);
	}
	else
	{
		componentsWindow.m_bIsMeshComponentEligible = false;

		m_SelectedActorMeshComponent = nullptr;
	}
}

void ComponentsDataManager::SetSelectedActorPhysicComp(UIWindowComponents& componentsWindow)
{
	auto colliderComponentIdFound = World::GetInstance().GetComponentID<Collider>(componentsWindow.GetActorSelected()->m_idEntity);

	//Try to find the static mesh component for the current selected actor
	if (colliderComponentIdFound != -1)
	{
		m_SelectedActorPhysicsComponent = World::GetInstance().GetSystem<PhysixSystem>()->GetCollider(colliderComponentIdFound);
		componentsWindow.m_bIsPhysixComponentEligible = true;

		const auto dim = m_SelectedActorPhysicsComponent->GetScaleShape();
		if (dim.z != (float)INVALID_INDEX)
		{
			componentsWindow.GetPhysicComponentData().SetShape("Box");
			componentsWindow.GetPhysicComponentData().SetScalePhysic(dim);
		}
		else
		{
			if (dim.y != (float)INVALID_INDEX)
			{
				componentsWindow.GetPhysicComponentData().SetShape("Capsule");
				componentsWindow.GetPhysicComponentData().SetScalePhysic({dim.x, dim.y, 0.f});
			}
			else if(dim.x != (float)INVALID_INDEX)
			{
				componentsWindow.GetPhysicComponentData().SetScalePhysic({ dim.x, 0.f, 0.f });
				componentsWindow.GetPhysicComponentData().SetShape("Sphere");
			}
			else
			{
				componentsWindow.GetPhysicComponentData().SetShape("Convex");
			}
		}

		

		
		componentsWindow.GetPhysicComponentData().SetMass(m_SelectedActorPhysicsComponent->GetMass());
		
		componentsWindow.GetPhysicComponentData().SetRotationXLocked(m_SelectedActorPhysicsComponent->m_lockAngularX);
		componentsWindow.GetPhysicComponentData().SetRotationYLocked(m_SelectedActorPhysicsComponent->m_lockAngularY);
		componentsWindow.GetPhysicComponentData().SetRotationZLocked(m_SelectedActorPhysicsComponent->m_lockAngularZ);
		
		componentsWindow.GetPhysicComponentData().SetTrigger(m_SelectedActorPhysicsComponent->m_bIsTrigger);

		if (m_SelectedActorPhysicsComponent->GetRigidType() == RigidType::STATIC)
		{
			componentsWindow.GetPhysicComponentData().SetCurrentRigidType((int)RigidType::STATIC);
		}
		else if (m_SelectedActorPhysicsComponent->GetRigidType() == RigidType::DYNAMIC)
		{
			componentsWindow.GetPhysicComponentData().SetCurrentRigidType((int)RigidType::DYNAMIC);
		}


	}
	else
	{
		componentsWindow.m_bIsPhysixComponentEligible = false;

		m_SelectedActorPhysicsComponent = nullptr;
	}
}

void ComponentsDataManager::SetSelectedActorCameraComp(UIWindowComponents& componentsWindow)
{
	auto cameraComponentFound = World::GetInstance().GetComponentID<Camera>(componentsWindow.GetActorSelected()->m_idEntity);

	if (cameraComponentFound != -1)
	{
		m_selectedActorCameraComponent = World::GetInstance().GetSystem<CameraSystem>()->GetCamera(cameraComponentFound);

		componentsWindow.GetCameraComponentData().SetCameraOffset(m_selectedActorCameraComponent->m_OffsetFromPlayer);
		componentsWindow.m_bIsCameraComponentEligible = true;
	}
	else
	{
		componentsWindow.m_bIsCameraComponentEligible = false;

		m_selectedActorCameraComponent = nullptr;
	}
}

void ComponentsDataManager::SetSelectedActorLightComp(UIWindowComponents& componentsWindow)
{
	const size_t lightComponentIDFound = World::GetInstance().GetComponentID<LightComponent>(componentsWindow.GetActorSelected()->m_idEntity);

	//Try to find the static mesh component for the current selected actor
	if (lightComponentIDFound != INVALID_INDEX)
	{
		auto* lightComponent = World::GetInstance().GetSystem<LightSystem>()->GetLight<LightComponent>(lightComponentIDFound);
		if(lightComponent)
		{
			const auto typeLight = lightComponent->type;
			switch (typeLight)
			{
			case Type::DIRECTIONAL:
				DisplayDirectionalLight(componentsWindow,lightComponentIDFound);
				break;
			case Type::SPOT:
				DisplaySpotLight(componentsWindow, lightComponentIDFound);
				break;
			case Type::POINT:
				DisplayPointLight(componentsWindow, lightComponentIDFound);
			}
			componentsWindow.m_bIsLightComponentEligible = true;
		}
		else
		{
			DisableDisplayLightComponent(componentsWindow);
		}
	}
	else
	{
		DisableDisplayLightComponent(componentsWindow);
	}
}

void ComponentsDataManager::SetSelectedActorScriptsComp(UIWindowComponents& componentsWindow)
{
	//TODO Maybe change this to get components instead of the strings, however we do just needs the string to display in the UI tbh
	auto ScriptComponentsFound = World::GetInstance().GetSystem<ScriptingSystem>()->GetAllScriptsPath(componentsWindow.GetActorSelected()->m_idEntity);

	if (ScriptComponentsFound.empty() == false)
	{
		componentsWindow.GetScriptComponentData().SetAllPaths(ScriptComponentsFound);
		componentsWindow.m_bIsScriptComponentEligible = true;
	}
	else
	{
		componentsWindow.GetScriptComponentData().GetScriptsPath().clear();
		componentsWindow.m_bIsScriptComponentEligible = false;
	}
}

void ComponentsDataManager::SetSelectedActorSoundComp(UIWindowComponents& componentsWindow)
{
	auto soundComponentIDFound = World::GetInstance().GetComponentID<SoundComponent>(componentsWindow.GetActorSelected()->m_idEntity);

	//Try to find the static mesh component for the current selected actor
	if (soundComponentIDFound != -1)
	{
		m_selectedActorSoundComponent = World::GetInstance().GetSystem<SoundSystem>()->GetSound(soundComponentIDFound);
		componentsWindow.m_bIsSoundComponentEligible = true;

		componentsWindow.GetSoundComponentData().SetPath(m_selectedActorSoundComponent->GetPath());
		
		componentsWindow.GetSoundComponentData().SetLooping(m_selectedActorSoundComponent->m_bIsLooping);
		componentsWindow.GetSoundComponentData().SetMute(m_selectedActorSoundComponent->m_bIsMute);
		componentsWindow.GetSoundComponentData().SetVolume(m_selectedActorSoundComponent->m_volume);
		componentsWindow.GetSoundComponentData().SetPlayOnStart(m_selectedActorSoundComponent->m_bPlayOnWake);
		//componentsWindow.GetSoundComponentData().SetFrequency(m_selectedActorSoundComponent->GetFrequency());
	}
	else
	{
		componentsWindow.m_bIsSoundComponentEligible = false;
		m_selectedActorSoundComponent = nullptr;
	}
}

void ComponentsDataManager::UpdateTransformData(UIWindowComponents& componentsWindow)
{
	if (m_SelectedActorTransformComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.m_bIsTransformUpdated == true)
	{
		componentsWindow.m_bIsTransformUpdated = false;

		m_SelectedActorTransformComponent->SetLocalPosition(componentsWindow.GetTransformComponentData().GetPosition());
		if (m_selectedActorSpotLightComponent != nullptr || m_selectedActorPointLightComponent != nullptr || m_selectedActorDirectionalLightComponent != nullptr)
		{
			auto positionActor = componentsWindow.GetTransformComponentData().GetPosition();
			
			componentsWindow.GetLightComponentData().SetLightPosition(Vector4(positionActor.x, positionActor.y, positionActor.z, 1.f));	
		}

		m_SelectedActorTransformComponent->SetLocalRotation(componentsWindow.GetTransformComponentData().GetRotation());
		m_SelectedActorTransformComponent->SetScale(componentsWindow.GetTransformComponentData().GetScale());	
	}
}

void ComponentsDataManager::UpdateCameraData(UIWindowComponents& componentsWindow)
{
	if (m_selectedActorCameraComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.m_bIsCameraUpdated == true)
	{
		componentsWindow.m_bIsCameraUpdated = false;
		
		m_selectedActorCameraComponent->m_OffsetFromPlayer = componentsWindow.GetCameraComponentData().GetCameraOffset();
	}
}

void ComponentsDataManager::UpdateStaticMeshData(UIWindowComponents& componentsWindow)
{
	if (m_SelectedActorMeshComponent == nullptr)
	{
		return;
	}

	//Changing mesh from the UI Mesh component
	if (componentsWindow.GetStaticMeshComponentData().IsMeshUpdated() == true)
	{
		//Loading the new model
		m_SelectedActorMeshComponent->LoadModel(componentsWindow.GetStaticMeshComponentData().GetNewMeshActorPath());
		//m_SelectedActorMeshComponent->LoadModel(UIWindowComponents.m_newMeshActorPath);

		//Loading the current Texture
		if (m_SelectedActorMeshComponent->m_currentTexturePath.empty() == false)
		{
			m_SelectedActorMeshComponent->LoadTexture(m_SelectedActorMeshComponent->m_currentTexturePath);
		}

		//Update the new mesh name
		componentsWindow.GetStaticMeshComponentData().UpdateMeshData(m_SelectedActorMeshComponent->m_staticMeshName);
	}

	if (componentsWindow.GetStaticMeshComponentData().IsMeshTextureUpdated() == true)
	{
		//Load the new Texture
		m_SelectedActorMeshComponent->LoadTexture(componentsWindow.GetStaticMeshComponentData().GetNewMeshTexturePath());

		//Change the texture preview in the UI
		componentsWindow.ChangeCurrentMeshTexturePreview(m_SelectedActorMeshComponent->m_currentTexturePath);

		//Change the texture name
		componentsWindow.GetStaticMeshComponentData().UpdateTextureMeshData(m_SelectedActorMeshComponent->m_staticMeshTextureName);
	}
}

void ComponentsDataManager::UpdatePhysicData(UIWindowComponents& componentsWindow)
{
	if (m_SelectedActorPhysicsComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.m_bIsPhysicsUpdated == true)
	{
		componentsWindow.m_bIsPhysicsUpdated = false;


		m_SelectedActorPhysicsComponent->SetKinematic(componentsWindow.GetPhysicComponentData().IsKinematic());
		m_SelectedActorPhysicsComponent->DisableGravity(componentsWindow.GetPhysicComponentData().IsGravityDisabled());
		m_SelectedActorPhysicsComponent->SetMass(componentsWindow.GetPhysicComponentData().GetMass());
		
		m_SelectedActorPhysicsComponent->LockRotation(componentsWindow.GetPhysicComponentData().IsRotationXLocked(),
			m_SelectedActorPhysicsComponent->m_lockAngularY,
			m_SelectedActorPhysicsComponent->m_lockAngularZ);
		
		m_SelectedActorPhysicsComponent->LockRotation(m_SelectedActorPhysicsComponent->m_lockAngularX,
			componentsWindow.GetPhysicComponentData().IsRotationYLocked(),
			m_SelectedActorPhysicsComponent->m_lockAngularZ);
		
		m_SelectedActorPhysicsComponent->LockRotation(m_SelectedActorPhysicsComponent->m_lockAngularX,
			m_SelectedActorPhysicsComponent->m_lockAngularY,
			componentsWindow.GetPhysicComponentData().IsRotationZLocked());

		m_SelectedActorPhysicsComponent->TriggerCollision(componentsWindow.GetPhysicComponentData().IsTrigger());

		m_SelectedActorPhysicsComponent->SetScale(componentsWindow.GetPhysicComponentData().GetScalePhysic());

	}

	if (componentsWindow.m_bIsRigidTypeUpdated == true)
	{

		componentsWindow.m_bIsRigidTypeUpdated = false;
		
		if (componentsWindow.GetPhysicComponentData().GetRigidTypes()[componentsWindow.GetPhysicComponentData().GetCurrentRigidType()] == "Static")
		{
			m_SelectedActorPhysicsComponent->ChangeType(RigidType::STATIC);
		}
		else if (componentsWindow.GetPhysicComponentData().GetRigidTypes()[componentsWindow.GetPhysicComponentData().GetCurrentRigidType()] == "Dynamic")
		{
			m_SelectedActorPhysicsComponent->ChangeType(RigidType::DYNAMIC);
		}
	}
}

void ComponentsDataManager::UpdateDirectionalLightData(UIWindowComponents& componentsWindow)
{
	if (m_selectedActorDirectionalLightComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.m_bIsDirectionalLightUpdated == true)
	{

		componentsWindow.m_bIsDirectionalLightUpdated = false;

		m_selectedActorDirectionalLightComponent->SetAmbientColor({ componentsWindow.GetLightComponentData().GetAmbiantColorLight().x, componentsWindow.GetLightComponentData().GetAmbiantColorLight().y, componentsWindow.GetLightComponentData().GetAmbiantColorLight().z });
		m_selectedActorDirectionalLightComponent->SetDiffuseColor({ componentsWindow.GetLightComponentData().GetDiffuseColorLight().x, componentsWindow.GetLightComponentData().GetDiffuseColorLight().y, componentsWindow.GetLightComponentData().GetDiffuseColorLight().z });
		m_selectedActorDirectionalLightComponent->SetSpecularColr({ componentsWindow.GetLightComponentData().GetSpecularColorLight().x, componentsWindow.GetLightComponentData().GetSpecularColorLight().y, componentsWindow.GetLightComponentData().GetSpecularColorLight().z });

		Vector4 positionLight = Vector4(componentsWindow.GetLightComponentData().GetLightPosition().x, componentsWindow.GetLightComponentData().GetLightPosition().y, componentsWindow.GetLightComponentData().GetLightPosition().z, 0.f);
		m_selectedActorDirectionalLightComponent->SetPosition(positionLight);
	}
}

void ComponentsDataManager::UpdateSpotLightData(UIWindowComponents& componentsWindow)
{
	if (m_selectedActorSpotLightComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.m_bIsSpotLightLightUpdated == true)
	{
		componentsWindow.m_bIsSpotLightLightUpdated = false;

		m_selectedActorSpotLightComponent->SetAmbientColor({ componentsWindow.GetLightComponentData().GetAmbiantColorLight().x, componentsWindow.GetLightComponentData().GetAmbiantColorLight().y, componentsWindow.GetLightComponentData().GetAmbiantColorLight().z });
		m_selectedActorSpotLightComponent->SetDiffuseColor({ componentsWindow.GetLightComponentData().GetDiffuseColorLight().x, componentsWindow.GetLightComponentData().GetDiffuseColorLight().y, componentsWindow.GetLightComponentData().GetDiffuseColorLight().z });
		m_selectedActorSpotLightComponent->SetSpecularColr({ componentsWindow.GetLightComponentData().GetSpecularColorLight().x, componentsWindow.GetLightComponentData().GetSpecularColorLight().y, componentsWindow.GetLightComponentData().GetSpecularColorLight().z });

		Vector4 positionLight = Vector4(componentsWindow.GetLightComponentData().GetLightPosition().x, componentsWindow.GetLightComponentData().GetLightPosition().y, componentsWindow.GetLightComponentData().GetLightPosition().z, 1.f);
		m_selectedActorSpotLightComponent->SetPosition(positionLight);
		
		m_selectedActorSpotLightComponent->SetDirection(componentsWindow.GetLightComponentData().GetLightDirection());
		m_selectedActorSpotLightComponent->SetCutOff(componentsWindow.GetLightComponentData().GetLightCutOff());
		m_selectedActorSpotLightComponent->SetOuterCutOff(componentsWindow.GetLightComponentData().GetLightOuterCutOff());
		m_selectedActorSpotLightComponent->SetConstantAttenuation(componentsWindow.GetLightComponentData().GetConstantAttenuation());
		m_selectedActorSpotLightComponent->SetLinearAttenuation(componentsWindow.GetLightComponentData().GetLinearAttenuation());
		m_selectedActorSpotLightComponent->SetQuadraticAttenuation(componentsWindow.GetLightComponentData().GetQuadraticAttenuation());
	}
}

void ComponentsDataManager::UpdatePointLightData(UIWindowComponents& componentsWindow)
{
	if (m_selectedActorPointLightComponent == nullptr)
	{
		return;
	}


	if (componentsWindow.m_bIsPointLightLightUpdated == true)
	{
		componentsWindow.m_bIsPointLightLightUpdated = false;



		m_selectedActorPointLightComponent->SetAmbientColor({ componentsWindow.GetLightComponentData().GetAmbiantColorLight().x, componentsWindow.GetLightComponentData().GetAmbiantColorLight().y, componentsWindow.GetLightComponentData().GetAmbiantColorLight().z });
		m_selectedActorPointLightComponent->SetDiffuseColor({ componentsWindow.GetLightComponentData().GetDiffuseColorLight().x, componentsWindow.GetLightComponentData().GetDiffuseColorLight().y, componentsWindow.GetLightComponentData().GetDiffuseColorLight().z });
		m_selectedActorPointLightComponent->SetSpecularColr({ componentsWindow.GetLightComponentData().GetSpecularColorLight().x, componentsWindow.GetLightComponentData().GetSpecularColorLight().y, componentsWindow.GetLightComponentData().GetSpecularColorLight().z });

		Vector4 positionLight = Vector4(componentsWindow.GetLightComponentData().GetLightPosition().x, componentsWindow.GetLightComponentData().GetLightPosition().y, componentsWindow.GetLightComponentData().GetLightPosition().z, 1.f);
		m_selectedActorPointLightComponent->SetPosition(positionLight);

		
		m_selectedActorPointLightComponent->SetConstantAttenuation(componentsWindow.GetLightComponentData().GetConstantAttenuation());
		m_selectedActorPointLightComponent->SetLinearAttenuation(componentsWindow.GetLightComponentData().GetLinearAttenuation());
		m_selectedActorPointLightComponent->SetQuadraticAttenuation(componentsWindow.GetLightComponentData().GetQuadraticAttenuation());
	}
}

void ComponentsDataManager::UpdateSoundData(UIWindowComponents& componentsWindow)
{
	if (m_selectedActorSoundComponent == nullptr)
	{
		return;
	}

	if (componentsWindow.GetSoundComponentData().IsSoundChanged() == true)
	{
		m_selectedActorSoundComponent->ChangeSound(componentsWindow.GetSoundComponentData().GetNewSoundPath());
		componentsWindow.GetSoundComponentData().SetPath(m_selectedActorSoundComponent->GetPath());

		componentsWindow.GetSoundComponentData().SetIsChangingSound(false);
	}

	if (componentsWindow.m_bIsSoundUpdated == true)
	{
		componentsWindow.m_bIsSoundUpdated = false;
		
		m_selectedActorSoundComponent->m_bIsLooping = componentsWindow.GetSoundComponentData().IsLooping();
		m_selectedActorSoundComponent->SetLoop(componentsWindow.GetSoundComponentData().IsLooping());

		m_selectedActorSoundComponent->m_bIsMute = componentsWindow.GetSoundComponentData().IsMute();
		m_selectedActorSoundComponent->m_volume = componentsWindow.GetSoundComponentData().GetVolume();

		m_selectedActorSoundComponent->m_bPlayOnWake = componentsWindow.GetSoundComponentData().PlayOnStart();
	}
}

void ComponentsDataManager::RemoveComponentWhenAvailable(UIWindowComponents& componentsWindow)
{
	if (componentsWindow.m_componentNameToRemove.empty() == false)
	{
		componentsWindow.m_bReloadActorComponentsData = true;
		
		if (componentsWindow.m_componentNameToRemove == "Remove Static Mesh Component")
		{
			componentsWindow.m_componentNameToRemove.clear();
			
			auto indexStaticMesh = World::GetInstance().GetComponentID<StaticMesh>(componentsWindow.GetActorIDToDisplay());

			World::GetInstance().GetSystem<StaticMeshSystem>()->RemoveStaticMesh(indexStaticMesh, componentsWindow.GetActorIDToDisplay());

			return;
		}

		if (componentsWindow.m_componentNameToRemove == "Remove Physix Component")
		{
		    componentsWindow.m_componentNameToRemove.clear();
			
			auto indexCollider = World::GetInstance().GetComponentID<Collider>(componentsWindow.GetActorIDToDisplay());

			World::GetInstance().GetSystem<PhysixSystem>()->RemoveComponent(indexCollider, componentsWindow.GetActorIDToDisplay());

			return;
		}

		if (componentsWindow.m_componentNameToRemove == "Remove Camera Component")
		{
			componentsWindow.m_componentNameToRemove.clear();

			auto indexCamera = World::GetInstance().GetComponentID<Camera>(componentsWindow.GetActorIDToDisplay());

			World::GetInstance().GetSystem<CameraSystem>()->RemoveCamera(indexCamera, componentsWindow.GetActorIDToDisplay());

			
			return;
		}

		if (componentsWindow.m_componentNameToRemove == "Remove Light Component")
		{
			componentsWindow.m_componentNameToRemove.clear();

			auto indexLight = World::GetInstance().GetComponentID<LightComponent>(componentsWindow.GetActorIDToDisplay());
			
			World::GetInstance().GetSystem<LightSystem>()->RemoveLight(indexLight, componentsWindow.GetActorIDToDisplay());

			return;
		}

		if (componentsWindow.m_componentNameToRemove == "Remove Sound Component")
		{
			componentsWindow.m_componentNameToRemove.clear();

			auto indexSound = World::GetInstance().GetComponentID<SoundComponent>(componentsWindow.GetActorIDToDisplay());

			World::GetInstance().GetSystem<SoundSystem>()->RemoveSound(indexSound, componentsWindow.GetActorIDToDisplay());

			return;
		}
		


	}
}

void ComponentsDataManager::DisableDisplayLightComponent(UIWindowComponents& componentsWindow)
{
	componentsWindow.m_bIsLightComponentEligible = false;
	m_selectedActorDirectionalLightComponent = nullptr;
	m_selectedActorSpotLightComponent = nullptr;
	m_selectedActorPointLightComponent = nullptr;
}

void ComponentsDataManager::DisplayDirectionalLight(UIWindowComponents& componentsWindow, size_t indexLight)
{
	m_selectedActorDirectionalLightComponent = World::GetInstance().GetSystem<LightSystem>()->GetLight<DirectionalLight>(indexLight);

	componentsWindow.GetLightComponentData().SetLightColors(m_selectedActorDirectionalLightComponent->GetAmbientColor(),
		m_selectedActorDirectionalLightComponent->GetDiffuseColor(),
		m_selectedActorDirectionalLightComponent->GetSpecularColor());

	componentsWindow.GetLightComponentData().SetDirectionalLightData(m_selectedActorDirectionalLightComponent->GetPosition());
}

void ComponentsDataManager::DisplaySpotLight(UIWindowComponents& componentsWindow, size_t indexLight)
{
	m_selectedActorSpotLightComponent = World::GetInstance().GetSystem<LightSystem>()->GetLight<SpotLight>(indexLight);

	componentsWindow.GetLightComponentData().SetLightColors(m_selectedActorSpotLightComponent->GetAmbientColor(),
		m_selectedActorSpotLightComponent->GetDiffuseColor(),
		m_selectedActorSpotLightComponent->GetSpecularColor());

	componentsWindow.GetLightComponentData().SetSpotLightData(m_selectedActorSpotLightComponent->GetPosition(),
		m_selectedActorSpotLightComponent->GetDirection(),
		m_selectedActorSpotLightComponent->GetCutOff(),
		m_selectedActorSpotLightComponent->GetOuterCutOff(),
		m_selectedActorSpotLightComponent->GetConstantAttenuation(),
		m_selectedActorSpotLightComponent->GetLinearAttenuation(),
		m_selectedActorSpotLightComponent->GetQuadraticAttenuation());
}

void ComponentsDataManager::DisplayPointLight(UIWindowComponents& componentsWindow, size_t indexLight)
{
	m_selectedActorPointLightComponent = World::GetInstance().GetSystem<LightSystem>()->GetLight<PointLight>(indexLight);

	componentsWindow.GetLightComponentData().SetLightColors(m_selectedActorPointLightComponent->GetAmbientColor(),
		m_selectedActorPointLightComponent->GetDiffuseColor(),
		m_selectedActorPointLightComponent->GetSpecularColor());

	componentsWindow.GetLightComponentData().SetPointLightData(m_selectedActorPointLightComponent->GetPosition(),
		m_selectedActorPointLightComponent->GetConstantAttenuation(),
		m_selectedActorPointLightComponent->GetLinearAttenuation(),
		m_selectedActorPointLightComponent->GetQuadraticAttenuation());
}
