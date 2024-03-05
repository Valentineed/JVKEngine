#include "FSM.h"

#include <iostream>

#include "States/Engine/StateEngine.h"
#include "States/Play/StatePlay.h"
#include "Window/Window.h"
#include "../Scene/SceneManager.h"
#include "../Serialization/Serializer.h"

FSM::FSM(STATE state, Window* window, InputManager* inputManager, size_t sceneID, RenderingMgr* render) :
	m_inputManager(inputManager), m_sceneID(sceneID), m_render(render), m_window(window)
{
	m_state[(int)STATE::ENGINE] = new StateEngine();
	m_state[(int)STATE::PLAY] = new StatePlay();

	if (state == STATE::ENGINE)
		m_currentState = m_state[0];
	else
		m_currentState = m_state[1];

	m_currentState->Begin(m_window, m_inputManager, m_sceneID, m_render);
}

FSM::~FSM()
{
	for (auto& state : m_state)
	{
		delete state;
	}
}

STATE FSM::GetState() const
{
	return m_currentState->GetState();
}

State& FSM::MutState(STATE state)
{
	return *m_state[(int)state];
}

void FSM::ChangeState(STATE state, Window* window)
{
	SceneManager& sm = SceneManager::GetInstance();
	size_t toDeleteID = static_cast<size_t>(-1);

	if (m_currentState == m_state[(int)STATE::PLAY] && state == STATE::ENGINE)
	{
		toDeleteID = m_currentState->GetSceneID();
	}
	m_currentState->End();
	m_currentState = m_state[(int)state];
	if (state == STATE::PLAY)
	{
		sm.DuplicateCurrScene();
		m_sceneID = sm.GetCurrentID();
	}
	else if (state == STATE::ENGINE)
	{
		///XML::Serializer::GetInstance().LoadScene("no_name.xml");
		sm.ChangeScene(m_currentState->GetSceneID(), true);
		m_sceneID = sm.GetCurrentID();
	}
	m_currentState->Begin(window, m_inputManager, m_sceneID, m_render);
	if (toDeleteID >= 0 && state == STATE::ENGINE)
	{
		sm.DeleteDuplicateScene(toDeleteID);
	}

}

void FSM::Update(Window* window, const float deltaTime)
{
 	STATE next = m_currentState->Update(window, deltaTime);
	if (next != m_currentState->GetState())
	{
		ChangeState(next, window);
	}
}

void FSM::Init()
{
	m_currentState->Begin(m_window, m_inputManager, m_sceneID, m_render);
}