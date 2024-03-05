#pragma once
#include "SoundManager.h"
#include "InputSystem/InputManager.h"
#include "ECS/World.h"
#include "Window/Window.h"
#include "Systems/RenderSystem.h"
#include "Systems/TransformSystem.h"
#include "Resources/ResourcesManager.h"
#include "Systems/CameraSystem.h"
#include "Systems/PhysixSystem.h"
#include "Systems/ScriptingSystem.h"
#include "Systems/SoundSystem.h"

class PhysixManager;
class FSM;
class RenderingMgr;

class App
{
public:
	// Constructor and destructor
	App() = delete;
	App(const App& other) = delete;
	App& operator=(const App& other) = delete;
	App(const int windowWidth, const int windowHeight, const char* windowName);
	~App();
	void Init();
	int Update();

	void CloseApp();
	void Scene1();
	void Scene2();
	Window* MutWindow();
	//Todo move this functions in class CameraEngine

	void ReadScriptFile(std::string_view fileName);
	void ActiveMouse();
	void LoadTexture(std::string_view path);
	void BindTextureToModel(std::string_view texturePath, size_t modelID);
	void LoadSound(std::string_view path);
	bool m_bactiveMouse = false;

	double m_lastX = 0.0f;
	double m_lasty = 0.0f;
	bool m_bFirstTime = true;
	float m_deltaTime = 0.0f;
private:
	void InitECS();
	World* m_world;
	FSM* m_fsm;
	Window m_window;
	size_t m_sceneID;
	InputManager* m_inputManager;
	std::shared_ptr<RenderSystem> m_renderSystem;//TODO unique_ptr
	std::shared_ptr<CameraSystem> m_cameraSystem;//TODO unique_ptr
	std::shared_ptr<TransformSystem> m_transformSystem;
	std::shared_ptr<PhysixSystem> m_physixSystem;
	std::shared_ptr<ScriptingSystem> m_scriptingSystem;
	std::shared_ptr<SoundSystem> m_soundSystem;
	RenderingMgr* m_renderingMgr;
	PhysixManager* m_physixMgr;
	std::unique_ptr<Resources::ResourcesManager> m_resourceMgr;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
	bool m_bFirstUpdate = true;
};
