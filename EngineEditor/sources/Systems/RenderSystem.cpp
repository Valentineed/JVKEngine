#include "RenderSystem.h"

#include "CameraSystem.h"
#include "PhysixSystem.h"
#include "RenderingManager.h"
#include "SoundManager.h"
#include "TransformSystem.h"
#include "../Components/Camera.h"
#include "../Components/Lights/LightComponent.h"
#include "../Components/Lights/DirectionalLight.h"
#include "../Components/Lights/SpotLight.h"
#include "../Components/Lights/PointLight.h"
#include "../ECS/World.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Serialization/Serializer.h"
#include "../StateMachine/State.h"
#include "../StateMachine/FSM.h"

RenderSystem::RenderSystem()
{
	m_world = &World::GetInstance();
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Init(Window* window, size_t sceneID, RenderingMgr* render, bool aliasing)
{
	SceneManager& sm = SceneManager::GetInstance();
	m_transformSystem = m_world->GetSystem<TransformSystem>();

	m_staticMeshSystem = m_world->RegisterSystem<StaticMeshSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<StaticMesh>());
		m_world->SetSystemSignature<StaticMeshSystem>(signature);
	}
	m_staticMeshSystem->Init(m_transformSystem);

	m_lightSystem = m_world->RegisterSystem<LightSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<LightComponent>());
		m_world->SetSystemSignature<LightSystem>(signature);
	}
	m_lightSystem->Init();

	assert(render != nullptr && "Fail create Rendering Manager");
	assert(window != nullptr && "window  is null in RenderSystemInit");
	m_renderingMgr = render;

	m_renderingMgr->InitGraphicAPI(window, aliasing);

	m_mainCamera.m_name = "[Main Root]";

	if (sm.GetSceneAt(sceneID)->m_root == nullptr)
	{
		sm.GetSceneAt(sceneID)->m_root = &m_mainCamera;
	}

	Camera mainCam({ 2.f, 0.f, 2.f });
	const size_t indexMainCamera = m_world->GetSystem<CameraSystem>()->Register(mainCam);
	m_mainCamera.AddComponent<Camera>(indexMainCamera);

	UIWindowUIInGamePreview.CreateSampler();
	UIWindowViewportScene.CreateSampler();

	
	UIWindowWorldHierarchy.InitScene(sm.GetSceneAt(sceneID));

	UIWindowComponents.GetScriptCreationDelegate().Connect(m_world, &World::CreateScriptingComponent);
	UIWindowComponents.GetCameraCreationDelegate().Connect(m_world, &World::CreateCameraComponent);
	UIWindowComponents.GetMeshCreationDelegate().Connect(m_world, &World::CreateMeshComponent);
	UIWindowComponents.GetSphereShapePhysicsCreationDelegate().Connect(m_world, &World::CreateSphereShapePhysicsComponent);
	UIWindowComponents.GetCapsuleShapePhysicsCreationDelegate().Connect(m_world, &World::CreateCapsuleShapePhysicsComponent);
	UIWindowComponents.GetBoxShapePhysicsCreationDelegate().Connect(m_world, &World::CreateBoxShapePhysicsComponent);

	UIWindowComponents.GetDirectionalLightCreationDelegate().Connect(m_world, &World::CreateDirectionalLightComponent);
	UIWindowComponents.GetSpotLightCreationDelegate().Connect(m_world, &World::CreateSpotlightComponent);
	UIWindowComponents.GetPointLightCreationDelegate().Connect(m_world, &World::CreatePointlightComponent);
	
	UIWindowComponents.GetSoundCreationDelegate().Connect(m_world, &World::CreateSoundComponent);

	UIWindowComponents.GetRemoveActorDelegate().Connect(m_world, &World::RemoveActor);
	
	UIWindowWorldHierarchy.GetCreateActorToRootDelegate().Connect(m_world, &World::AddActorToRoot);
	UIWindowWorldHierarchy.GetCreateActorToActorDelegate().Connect(m_world, &World::AddActorToActor);
	UIWindowMainToolBar.GetSaveSceneDelegate().Connect(m_world, &World::SaveScene);

	m_soundManager = SoundManager::GetInstance();

	
	#ifdef _SHIPPING
		SceneManager::GetInstance().AddScenePath("Level1", "Assets/Levels/Level1.xml");
		SceneManager::GetInstance().AddScenePath("Level2", "Assets/Levels/Level2.xml");
		SceneManager::GetInstance().AddScenePath("Level3", "Assets/Levels/Level3.xml");
	#endif


}

void RenderSystem::Update(float deltaTime, Window* window, size_t sceneID, STATE state, FSM* fsm)
{
	if (m_renderingMgr == nullptr)
	{
		throw std::runtime_error("Rendering Manager not Init");
	}
	if (window == nullptr)
	{
		throw std::runtime_error("window is null in RenderSystemUpdate");
	}
	if (SceneManager::GetInstance().GetSceneAt(sceneID) == nullptr)
	{
		throw std::runtime_error("scene is null in RenderSystemUpdate");
	}

	//Send data in RenderingManager
	m_staticMeshSystem->Update(m_renderingMgr);

	UpdateDataFromUI(sceneID, fsm, window);
	//Update Cam in Libgfx
	UpdateMainCam(state, deltaTime);
	m_lightSystem->Update();

	m_renderingMgr->DrawFrame(window);
}

void RenderSystem::UpdateMainCam(STATE currState, float deltaTime)
{
	if (currState == STATE::ENGINE)
	{
		/*Create a Begin for RenderSystem To set a camera*/
		auto* cam = m_mainCamera.GetCameraComponent();
		if (cam != nullptr)
		{
			const Vector3 campPos = cam->GetCameraPos();
			m_renderingMgr->currCam->m_pos = campPos;
			m_renderingMgr->currCam->m_view = cam->GetViewMatrix();
			m_renderingMgr->currCam->m_proj = cam->GetPerspectiveMatrix();
			/*Set a Listener*/
			const Vector3 velocity = (campPos - m_camOldPos) / deltaTime;
			m_soundManager->SetListener(campPos, velocity, cam->m_cameraFront, cam->m_cameraUp);
			m_camOldPos = campPos;
		}
	}
	else if (currState == STATE::PLAY)
	{
		////HERE ID THAT WE POSSESS
		const int actorIdToPossess = m_renderingMgr->m_UIWindowManager.GetActorIDToPossess();
		const auto indexCam = m_world->GetComponentID<Camera>(actorIdToPossess);
		if (indexCam != -1)
		{
			if (SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, actorIdToPossess) != nullptr)
			{
				SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, actorIdToPossess)->UpdateCameraPos();
			}
			m_currCam = m_world->GetSystem<CameraSystem>()->GetCamera(indexCam);
			const Vector3 campPos = m_currCam->GetCameraPos();
			m_renderingMgr->currCam->m_pos = campPos;
			m_renderingMgr->currCam->m_view = m_currCam->GetViewMatrix();
			m_renderingMgr->currCam->m_proj = m_currCam->GetPerspectiveMatrix();

			/*Set a Listener*/
			const Vector3 velocity = (campPos - m_camOldPos) / deltaTime;
			m_soundManager->SetListener(campPos, velocity, -m_currCam->m_cameraFront, m_currCam->m_cameraUp);
			m_camOldPos = campPos;
		}
	}
}

void RenderSystem::End()
{
	assert(m_renderingMgr != nullptr && "Rendering Manager not Init");
	UIWindowFIleBrowser.ClearImageViews();
	UIWindowUIInGamePreview.ClearWidgetsImage();
	UIWindowViewportScene.ClearWidgetsImage();
	m_renderingMgr->m_UIWindowManager.CLearWindows();
	m_renderingMgr->Cleanup();
}

void RenderSystem::LoadTexturedModel(std::string_view obj, std::string_view text)
{
	(void)m_renderingMgr->LoadModel(obj, text);
	//uint32_t id = m_renderingMgr->AddModel();
}

size_t RenderSystem::LoadModel(std::string_view obj)
{
	m_renderingMgr->EmplaceModel(obj);
	return -1;	//TODO CHANGE EMPLACE MODEL RETURN TYPE TO PAIR<Model, size_t>
}

size_t RenderSystem::LoadTexture(std::string_view path)
{
	return m_renderingMgr->EmplaceTexture(path);
}

void RenderSystem::CreateUIWindows()
{
#ifndef _SHIPPING
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowWorldHierarchy);
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowComponents);

	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowFIleBrowser);
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowUIInGamePreview);
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowBuild);
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowMainToolBar);
#endif
	m_renderingMgr->m_UIWindowManager.AddWindow(&UIWindowViewportScene);
}

void RenderSystem::UpdateDataFromUI(size_t sceneID, FSM* fsm, Window* window)
{
	if (fsm->GetState() != STATE::PLAY)
	{
		m_componentsDataManager.SetDatasOfSelectedActor(SceneManager::GetInstance().GetSceneAt(sceneID), UIWindowWorldHierarchy, UIWindowComponents);
		UIWindowWorldHierarchy.InitScene(SceneManager::GetInstance().GetScene());

		UpdateDataFromBuildWindow();
		
		UpdateDataFromFileBrowserWindow();
		
		UpdateDataFromComponentsWindow();

		
	}

	UpdateDataFromUIPreviewWindow();
	
	if (fsm->GetState() == STATE::PLAY)
	{
		UIWindowWorldHierarchy.InitScene(SceneManager::GetInstance().GetScene());
	}

	UpdateReparentingActors(SceneManager::GetInstance().GetSceneAt(sceneID));

	UpdateChangingStateFromUI(fsm, window);
}

void RenderSystem::UpdateDataFromBuildWindow()
{
	if (UIWindowBuild.IsSavesUpdated() == true)
	{
		SceneManager::GetInstance().SetKnownScenes(UIWindowBuild.GetXMLSaves());
		UIWindowBuild.ResetSavesUpdated();
	}
}

void RenderSystem::UpdateDataFromComponentsWindow()
{
	if (UIWindowComponents.GetActorIDToDisplay() != -1)
	{
		m_componentsDataManager.UpdateTransformData(UIWindowComponents);
		m_componentsDataManager.UpdateCameraData(UIWindowComponents);
		m_componentsDataManager.UpdatePhysicData(UIWindowComponents);
		m_componentsDataManager.UpdateStaticMeshData(UIWindowComponents);
		m_componentsDataManager.UpdateDirectionalLightData(UIWindowComponents);
		m_componentsDataManager.UpdateSpotLightData(UIWindowComponents);
		m_componentsDataManager.UpdatePointLightData(UIWindowComponents);
		m_componentsDataManager.UpdateSoundData(UIWindowComponents);
		m_componentsDataManager.RemoveComponentWhenAvailable(UIWindowComponents);
	}

	//TODO: replace by delegate
	if (UIWindowComponents.m_bIsActorRemoved == true)
	{
		//UIWindowComponents.RemoveActorDatasFromUI();
		UIWindowWorldHierarchy.SetActorSelectedID(-1);
		UIWindowComponents.RemoveActorDatasFromUI();
		UIWindowComponents.GetActorSelected()->Remove();

		UIWindowComponents.m_bIsActorRemoved = false;
	}
}

void RenderSystem::UpdateDataFromUIPreviewWindow()
{
	if (UIWindowUIInGamePreview.NeedReloadViewportWidgets() == true)
	{
		UIWindowViewportScene.SetArrayWidgetsInGame(UIWindowUIInGamePreview.GetWidgets());

		UIWindowViewportScene.SetAnchorPositions(UIWindowUIInGamePreview.GetWidgets());
		
		UIWindowUIInGamePreview.SetNeedReloadViewportWidgets(false);
		UIWindowUIInGamePreview.SetNeedReloadViewportWidgetsDatas(true);
	}

	if (UIWindowUIInGamePreview.NeedReloadViewportWidgetsDatas() == true)
	{
		UIWindowViewportScene.ShowUIInGame(UIWindowUIInGamePreview.GetShowUIInVPWindow());
		UIWindowViewportScene.UpdateArrayWidgetsInGameDatas(UIWindowUIInGamePreview.GetWidgets());
		UIWindowUIInGamePreview.SetNeedReloadViewportWidgetsDatas(false);
	}

	if (UIWindowUIInGamePreview.NeedReloadAnchors() == true)
	{
		UIWindowViewportScene.SetAnchorPositions(UIWindowUIInGamePreview.GetWidgets());
		UIWindowUIInGamePreview.SetNeedReloadAnchors(false);
	}
}

void RenderSystem::UpdateDataFromFileBrowserWindow()
{
	if (UIWindowFIleBrowser.HasNewLevelsToAdd() == true)
	{
		for (std::pair<std::string, std::string> level : UIWindowFIleBrowser.GetLevelsToAdd())
		{
			SceneManager::GetInstance().AddScenePath(level.first, level.second);
		}
		UIWindowFIleBrowser.GetLevelsToAdd().clear();
	}

	if (UIWindowFIleBrowser.GetLevelNameToLoad().empty() == false)
	{
		if (SceneManager::GetInstance().GetKnownScenes().empty() == false)
		{
			
			if (UIWindowFIleBrowser.GetLevelNameToLoad() == "Level1" || UIWindowFIleBrowser.GetLevelNameToLoad() == "Level2" || UIWindowFIleBrowser.GetLevelNameToLoad() == "Level3")
			{
				GetViewportWindow().m_bShowTimer = true;
			}
			else
			{
				GetViewportWindow().m_bShowTimer = false;
			}
			SceneManager::GetInstance().LoadKnownScene(SceneManager::GetInstance().GetKnownScenes().find(UIWindowFIleBrowser.GetLevelNameToLoad())->second);

			World::GetInstance().GetSystem<TransformSystem>()->Update();

			UIWindowComponents.SetActorIDToDisplay(-1);
			UIWindowWorldHierarchy.SetActorSelectedID(-1);

			XML::Serializer::GetInstance().m_IfOnDefaultMap = false;
			
		}
		UIWindowFIleBrowser.GetLevelNameToLoad().clear();
	}
}

void RenderSystem::UpdateChangingStateFromUI(FSM* fsm, Window* window)
{
	if (UIWindowMainToolBar.IsChangingState() == true)
	{
		if (UIWindowMainToolBar.GetEngineState() == EngineState::PLAY)
		{
			if (UIWindowViewportScene.IsUIShowing() == false)
			{
				UIWindowViewportScene.ShowUIInGame(true);
			}
			fsm->ChangeState(STATE::PLAY, window);
		}
		else if (UIWindowMainToolBar.GetEngineState() == EngineState::PAUSE)
		{
			std::cout << "Pause";
		}
		else
		{
			fsm->ChangeState(STATE::ENGINE, window);
			if (UIWindowUIInGamePreview.GetShowUIInVPWindow() == false)
			{
				UIWindowViewportScene.ShowUIInGame(false);
			}
		}
		UIWindowMainToolBar.SetChangingState(false);
	}
}

void RenderSystem::UpdateReparentingActors(Scene* scene)
{
	if (UIWindowWorldHierarchy.IsReparentingActors() == true)
	{
		UIWindowWorldHierarchy.SetActorSelectedID(-1);
		UIWindowComponents.SetActorIDToDisplay(-1);
		
		auto* futureParentActor = scene->GetActor(scene->m_root, UIWindowWorldHierarchy.GetFutureParentActorID());
		auto* futureChildActor = scene->GetActor(scene->m_root, UIWindowWorldHierarchy.GetFutureChildActorID());

		futureParentActor->ReparentActor(futureChildActor);

		UIWindowWorldHierarchy.SetReparentingActors(false);
	}
}

UIWindowUIInGamePreview& RenderSystem::GetUIWindow()
{
	return UIWindowUIInGamePreview;
}

UIWindowViewportScene& RenderSystem::GetViewportWindow()
{
	return UIWindowViewportScene;
}
