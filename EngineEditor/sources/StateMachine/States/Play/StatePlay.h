#pragma once
#include "../../State.h"


enum class MovCamera;
struct Camera;


class StatePlay : public State
{
public:
	void Begin(Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render) override;
	void End() override;
	STATE Update(Window* window, const float deltaTime) override;
	
	STATE GetState() const override { return STATE::PLAY; }
	size_t GetSceneID() const override { return m_sceneID; }

	void MouseMoveCamera(double xOffset, double yOffset);

	void ChangeState();
private:
	STATE m_currentState = STATE::PLAY;

	Scene* m_scene = nullptr;
	size_t m_sceneID;
	Window* m_window = nullptr;

	Camera* m_currentPlayerCamera = nullptr;

	float m_mousePosX;
	float m_mousePosY;
	
	float m_lastX;
	float m_lastY;

	bool m_bActiveMouse = false;
};
