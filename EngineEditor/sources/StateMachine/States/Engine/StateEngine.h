#pragma once

#include <memory>

#include "../../State.h"
#include "Systems/CameraControlSystem.h"

class Capsule;
class LvlManager;
class Mountain;
class StaticBox;
class Weapon;
class Player;

class StateEngine : public State
{
public:
	StateEngine();
	~StateEngine();
	void Begin(Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render) override;
	void End() override;
	STATE Update(Window* window, const float deltaTime) override;
	void CloseApp();
	void SaveScene();
	void LoadScene();
	void AddModel();
	void AddModel2();
	STATE GetState() const override { return STATE::ENGINE; }
	size_t GetSceneID() const override { return m_sceneID; }
private:
	STATE m_currentState = STATE::ENGINE;
	Window* m_window;
	std::shared_ptr<CameraControlSystem> m_camControlSystem;
	InputManager* m_inputManager = nullptr;
	Player* m_player;
	Weapon* m_weapon;
	Mountain* m_mountain;
	StaticBox* m_box;
	LvlManager* m_lvlManager;
	Capsule* m_capsule;
	size_t m_sceneID;
	bool m_bisFirstTime = true;
	bool m_bDefaultScene = false;
};
