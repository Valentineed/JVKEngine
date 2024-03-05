#include "App.h"

#include "PhysixManager.h"
#include "RenderingManager.h"

#include "InputSystem/DefineInput.h"
#include "Components/Camera.h"
#include "Components/Collider.h"
#include "Components/Transform.h"
#include "Components/Lights/LightComponent.h"
#include "Components/ScriptComponent.h"
#include "StateMachine/FSM.h"
#include "StateMachine/State.h"
#include "Scene/SceneManager.h"
#include "Serialization/Serializer.h"
#include "Systems/CameraSystem.h"

App::App(const int windowWidth, const int windowHeight, const char* windowName) :
	m_window(windowWidth, windowHeight, windowName)
{
	m_world = &World::GetInstance();
	m_lastTime = std::chrono::high_resolution_clock::now();
	m_inputManager = &InputManager::GetInstance();
	Init();
}

App::~App()
{
	delete m_renderingMgr;
	delete m_fsm;
}

void App::Init()
{
	m_window.SetWindowIcon("resources/images/downhillduck.jpg");
	m_inputManager->Init(&m_window);

	m_renderingMgr = new RenderingMgr;
	//m_physixMgr = new PhysixManager;

	//ECS StartUp
	InitECS();

	m_inputManager->SaveKey("CloseApp", KEY_ESCAPE);
	m_inputManager->BindKey("CloseApp", KeyEvent::PRESS, this, &App::CloseApp);

	m_inputManager->SaveKey("Scene1", KEY_F2);
	m_inputManager->BindKey("Scene1", KeyEvent::PRESS, this, &App::Scene1);
	m_inputManager->SaveKey("Scene2", KEY_F1);
	m_inputManager->BindKey("Scene2", KeyEvent::PRESS, this, &App::Scene2);

#ifndef _SHIPPING
	m_window.SetCursorMode(false);
#endif

	m_renderSystem->CreateUIWindows();

	SceneManager& sm = SceneManager::GetInstance();
#ifndef _SHIPPING
	//TODO check alex use pointer or add init in fsm and use fsm in member of class
	m_fsm = new FSM(STATE::ENGINE, &m_window, m_inputManager, sm.GetCurrentID(), m_renderingMgr);
#else
	m_fsm = new FSM(STATE::PLAY, &m_window, m_inputManager, sm.GetCurrentID(), m_renderingMgr);
#endif

	m_scriptingSystem->Start();
}

int App::Update()
{
	SceneManager& sm = SceneManager::GetInstance();

	try
	{
		while (!m_window.IsClosed())
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			m_deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
			
			
			if (m_fsm->GetState() == STATE::PLAY)
			{
				//m_scriptingSystem->Update(sm.GetScene()->m_root);
				//PhysicsSystem
				m_physixSystem->Update(m_deltaTime);
				
				m_transformSystem->Update();

				//m_transformSystem->Update();
			}
			

			//  GameSystem/EditorSystem
			m_fsm->Update(&m_window, m_deltaTime);
			
			m_window.PollEvents();
			m_inputManager->ProcessInput();
			m_transformSystem->Update();

			
			m_soundSystem->Update();
			//Render
			m_renderSystem->Update(m_deltaTime, &m_window, sm.GetCurrentID(), m_fsm->GetState(), m_fsm);

			//LastLine of Update
			m_lastTime = currentTime;
		}
	}
	catch (const std::exception & exept)
	{
		std::cerr << exept.what() << std::endl;
		return EXIT_FAILURE;
	}
	m_soundSystem->End();
	m_renderSystem->End();
	return EXIT_SUCCESS;
}

void App::CloseApp()
{
	m_window.SetWindowShouldClose();
}

void App::Scene1()
{
	SceneManager& sm = SceneManager::GetInstance();

	if (sm.GetKnownScenes().empty() == false)
	{
		auto levelPath = sm.GetKnownScenes().find("no_name");

		if (levelPath != sm.GetKnownScenes().end())
		{
			sm.LoadKnownScene(levelPath->second);

			m_transformSystem->Update();
		}
		
	}

	
}

void App::Scene2()
{
	SceneManager& sm = SceneManager::GetInstance();

	if (sm.GetKnownScenes().empty() == false)
	{
		auto levelPath = sm.GetKnownScenes().find("save");

		if (levelPath != sm.GetKnownScenes().end())
		{
			sm.LoadKnownScene(levelPath->second);

			m_transformSystem->Update();
		}

	}
}

Window* App::MutWindow()
{
	return &m_window;
}

void App::ReadScriptFile(std::string_view fileName)
{
	std::filesystem::path p(fileName);
	std::system(p.string().c_str());
	std::cout << "Resources have been pasted near .exe\n";
}

void App::LoadSound(std::string_view path)
{
	m_resourceMgr->GetResource(path, m_renderSystem/*, true*/);
}

void App::InitECS()
{
	SceneManager& sm = SceneManager::GetInstance();
	m_world->Init();
	m_world->RegisterComponent<Camera>();
	m_world->RegisterComponent<Transform>();
	m_world->RegisterComponent<StaticMesh>();
	m_world->RegisterComponent<Collider>();
	m_world->RegisterComponent<ScriptComponent>();
	m_world->RegisterComponent<LightComponent>();
	m_world->RegisterComponent<SoundComponent>();

	m_soundSystem = m_world->RegisterSystem<SoundSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<SoundComponent>());
		m_world->SetSystemSignature<SoundSystem>(signature);
	}
	m_soundSystem->Init();

	m_transformSystem = m_world->RegisterSystem<TransformSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<Transform>());
		m_world->SetSystemSignature<TransformSystem>(signature);
	}

	m_physixSystem = m_world->RegisterSystem<PhysixSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<Collider>());
		m_world->SetSystemSignature<PhysixSystem>(signature);
	}
	m_physixSystem->Init();

	m_renderSystem = m_world->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<Camera>());
		m_world->SetSystemSignature<RenderSystem>(signature);
	}

	m_cameraSystem = m_world->RegisterSystem<CameraSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<Camera>());
		m_world->SetSystemSignature<CameraSystem>(signature);
	}

	m_scriptingSystem = m_world->RegisterSystem<ScriptingSystem>();
	{
		Signature signature;
		signature.set(m_world->GetComponentType<ScriptComponent>());
		m_world->SetSystemSignature<ScriptingSystem>(signature);
	}

	sm.NewScene();
#ifdef _SHIPPING
	m_renderSystem->Init(&m_window, sm.GetCurrentID(), m_renderingMgr, false);
#else
	m_renderSystem->Init(&m_window, sm.GetCurrentID(), m_renderingMgr, true);
#endif

	Scene mainScene = XML::Serializer::GetInstance().LoadScene("Assets/Levels/Level1.xml");
	sm.ChangeSceneAt(sm.GetCurrentID(), mainScene);
	m_transformSystem->Update();//Refresh all transform after LoadScene

#ifdef _SHIPPING
	//ReadScriptFile(BATCH_FILE);
#endif
}