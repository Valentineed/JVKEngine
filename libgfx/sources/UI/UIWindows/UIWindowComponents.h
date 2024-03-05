#pragma once

#include "../../../../EngineEditor/sources/Scene/Actor.h"

#include "UI/UIWindow.h"
#include <functional>

#include "Resources/ActorComponentsData.h"

enum class TextureNameComponentsWindow
{
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS,
	BLACK_BACKGROUND,
	CURRENT_MESH_TEXTURE
};



class UIWindowComponents : public UIWindow
{
public:

	libgfx_API UIWindowComponents();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;

	int libgfx_API GetActorIDToDisplay();

	void libgfx_API SetActorIDToDisplay(int ID);

	void libgfx_API FindAndSetActorToDisplay(Actor* actor, unsigned int ActorIDToFind);

	libgfx_API Delegate<void(unsigned int, const std::string&)>& GetScriptCreationDelegate();
	libgfx_API Delegate<void(unsigned int, Vector3, Vector3, Vector3)>& GetCameraCreationDelegate();
	libgfx_API Delegate<void(unsigned int, size_t)>& GetMeshCreationDelegate();
	libgfx_API Delegate<void(unsigned int)>& GetSphereShapePhysicsCreationDelegate();
	libgfx_API Delegate<void(unsigned int)>& GetCapsuleShapePhysicsCreationDelegate();
	libgfx_API Delegate<void(unsigned int)>& GetBoxShapePhysicsCreationDelegate();
	
	libgfx_API Delegate<void(unsigned int)>& GetDirectionalLightCreationDelegate();
	libgfx_API Delegate<void(unsigned int, Vector3)>& GetSpotLightCreationDelegate();
	libgfx_API Delegate<void(unsigned int)>& GetPointLightCreationDelegate();
	
	libgfx_API Delegate<void(unsigned int)>& GetSoundCreationDelegate();

	
	libgfx_API Delegate<void(Actor*)>& GetRemoveActorDelegate();

	libgfx_API Actor* GetActorSelected();

	void DisplayActorTransform();

	void DisplayBasicActorSettings();

	void DisplayTransformComponent();
	void DisplayMeshComponent();
	void DisplayPhysixComponent();
	void DisplayCameraComponent();
	void DisplayLightComponent();
	void DisplayScriptsComponent();
	void DisplaySoundComponent();

	void libgfx_API ChangeCurrentMeshTexturePreview(std::string path);

	void libgfx_API RemoveActorDatasFromUI();

	

	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------

	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	ActorComponentsData& GetActorSelectedData() { return m_actorSelectedData; }



	libgfx_API ActorTransformCompData& GetTransformComponentData();
	libgfx_API ActorStaticMeshCompData& GetStaticMeshComponentData();
	libgfx_API ActorPhysicCompData& GetPhysicComponentData();
	libgfx_API ActorCameraCompData& GetCameraComponentData();
	libgfx_API ActorLightCompData& GetLightComponentData();
	libgfx_API ActorScriptCompData& GetScriptComponentData();
	libgfx_API ActorSoundCompData& GetSoundComponentData();

	bool m_bIsTransformComponentEligible = false;
	bool m_bIsScriptComponentEligible = false;
	bool m_bIsMeshComponentEligible = false;
	bool m_bIsPhysixComponentEligible = false;
	bool m_bIsCameraComponentEligible = false;
	bool m_bIsLightComponentEligible = false;
	bool m_bIsSoundComponentEligible = false;
	


	bool m_bReloadActorComponentsData = false;
	std::string m_ComponentNameToAddSelected;
	std::string m_componentNameToRemove;

	bool m_bIsActorRemoved = false;

	bool m_bIsTransformUpdated = false;
	bool m_bIsPhysicsUpdated = false;
	bool m_bIsRigidTypeUpdated = false;

	bool m_bIsBasicLightSettingsUpdated = false;
	bool m_bIsDirectionalLightUpdated = false;
	bool m_bIsSpotLightLightUpdated = false;
	bool m_bIsPointLightLightUpdated = false;

	bool m_bIsCameraUpdated = false;
	
	bool m_bIsSoundUpdated = false;

	
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	int m_ActorIDToDisplay = -1;

	Actor* m_actorToDisplay;

	Delegate<void(unsigned int, const std::string&)> m_createScriptComponentDelegate;
	Delegate<void(unsigned int, Vector3, Vector3, Vector3)> m_createCameraComponentDelegate;
	Delegate<void(unsigned int, size_t)> m_createMeshComponentDelegate;
	Delegate<void(unsigned int)> m_createSphereShapePhysicsComponentDelegate;
	Delegate<void(unsigned int)> m_createCapsuleShapePhysicsComponentDelegate;
	Delegate<void(unsigned int)> m_createBoxShapePhysicsComponentDelegate;
	
	Delegate<void(unsigned int)> m_createDirectionalLightComponentDelegate;
	Delegate<void(unsigned int, Vector3)> m_createSpotLightComponentDelegate;
	Delegate<void(unsigned int)> m_createPointLightComponentDelegate;
	
	Delegate<void(unsigned int)> m_createSoundComponentDelegate;

	
	Delegate<void(Actor*)> m_removeActorDelegate;
	
	ActorComponentsData m_actorSelectedData;

	
	
};