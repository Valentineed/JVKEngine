#pragma once

#include <memory>

#include "State.h"
class Window;
class FSM
{
public:
	FSM(STATE state, Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render);
	FSM() = default;
	~FSM();

	[[nodiscard]] STATE GetState()const;
	State& MutState(STATE state);
	void ChangeState(STATE state, Window* window);
	void ChangeSceneID(size_t id)
	{
		m_currentState->End();
		m_sceneID = id;
		m_currentState->Begin(m_window, m_inputManager, m_sceneID, m_render);
	}
	void Update(Window* window, const float deltaTime);

	void Init();
private:
	std::shared_ptr<State> m_stateShared[(int)STATE::NO_STATE];
	State* m_state[(int)STATE::NO_STATE];
	State* m_currentState = nullptr;
	InputManager* m_inputManager;
	size_t m_sceneID;
	RenderingMgr* m_render;
	Window* m_window;
};