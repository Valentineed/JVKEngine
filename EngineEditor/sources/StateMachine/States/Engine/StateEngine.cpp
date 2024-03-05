#include "StateEngine.h"

#include "../../../App.h"
#include "../../../ECS/World.h"
#include "../../../Components/Camera.h"
#include "../../../InputSystem/DefineInput.h"
#include "../../../InputSystem/InputManager.h"
#include "../../../Scene/Scene.h"
#include "Window/Window.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Serialization/Serializer.h"

StateEngine::StateEngine()
{
	World* m_world = &World::GetInstance();
	//Setup EditorSystem
	m_camControlSystem = m_world->RegisterSystem<CameraControlSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<Camera>());
		m_world->SetSystemSignature<CameraControlSystem>(signature);//todo deplace in constructor ?
	}
}

StateEngine::~StateEngine()
{
	//TODO Check if their load has been skipped (when Loading scene with XML)
}

void StateEngine::Begin(Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render)
{
	SceneManager& sm = SceneManager::GetInstance();

	m_inputManager = inputManager;
	m_window = window;
	m_sceneID = sceneID;

	m_camControlSystem->Init(inputManager, window, sm.GetSceneAt(m_sceneID));

	m_inputManager->SaveKey("CloseApp", KEY_ESCAPE);

	//-------------------------------------------------------------------------------------

	m_inputManager->BindKey("CloseApp", KeyEvent::PRESS, this, &StateEngine::CloseApp); 
	
	sm.GetSceneAt(m_sceneID)->Begin();
}

void StateEngine::End()
{
	SceneManager::GetInstance().GetSceneAt(m_sceneID)->End();

	m_inputManager->Clear();
}

STATE StateEngine::Update(Window* window, const float deltaTime)
{
	m_camControlSystem->Update(deltaTime);
	//m_scene->Update(deltaTime);
	return m_currentState;
}

void StateEngine::CloseApp()
{
	m_window->SetWindowShouldClose();
}

void StateEngine::SaveScene()
{
	XML::Serializer::GetInstance().ExportScene(SceneManager::GetInstance().GetScene());
}

void StateEngine::LoadScene()
{
	SceneManager& sm = SceneManager::GetInstance();

	Scene scene = XML::Serializer::GetInstance().LoadScene("resources/no_name.xml");
	sm.AddScene(scene);
	sm.ChangeScene(sm.GetScenesSize() - 1);

	//m_fsm->ChangeSceneID(SceneManager::GetInstance().GetCurrentID());
}