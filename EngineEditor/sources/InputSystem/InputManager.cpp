#include "InputManager.h"



#include "Window/Window.h"


InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return  instance;
}

void InputManager::Init(Window* window/*,State state*/)
{
	//window->SetKeyCallback(this, &State::InputManager::KeyCallback);
	window->SetKeyCallback(this, &InputManager::KeyCallback);
	window->SetMousePosCallback(this, &InputManager::MousePosCallback);
	window->SetMouseButtonCallback(this, &InputManager::MouseButtonCallback);
}

void InputManager::ProcessInput()
{
	//tout les input encore actif
	for (int key : m_keyInRepeat)
	{
		m_keyInputs[key].m_delegate();
	}
	for(int button : m_mouseButtonInRepeat)
	{
		m_mouseInputs[button].m_delegate();
	}
}

void InputManager::Clear()
{
	m_settingsKey.clear();
	m_settingsMouse.clear();
	m_keyInputs.clear();
	m_mouseInputs.clear();
	m_joystickInputs.clear();
}

void InputManager::KeyCallback(int key, int /*scancode*/, int action, int /*mods*/)
{
	if (m_keyInputs[key].m_state == KeyEvent::REPEAT && action == (int)KeyEvent::PRESS)
	{
		m_keyInRepeat.push_back(key);
		return;
	}
	else if (m_keyInputs[key].m_state == KeyEvent::REPEAT && action == (int)KeyEvent::RELEASE)
	{
		int pos = 0;
		while(true)
		{
			if(m_keyInRepeat[pos] == key)
			{
				m_keyInRepeat.erase(m_keyInRepeat.begin() + pos);
				return;
			}
			pos++;
		}
	}

	if((int)m_keyInputs[key].m_state == action)
	{
		m_keyInputs[key].m_delegate();
	}
}

void InputManager::MouseButtonCallback(int button, int action, int mods)
{
	if (m_mouseInputs[button].m_state == KeyEvent::REPEAT && action == (int)KeyEvent::PRESS)
	{
		m_mouseButtonInRepeat.push_back(button);
		return;
	}
	else if (m_mouseInputs[button].m_state == KeyEvent::REPEAT && action == (int)KeyEvent::RELEASE)
	{
		int pos = 0;
		while (true)
		{
			if (m_mouseButtonInRepeat[pos] == button)
			{
				m_mouseButtonInRepeat.erase(m_mouseButtonInRepeat.begin() + pos);
				return;
			}
			pos++;
		}
	}

	if ((int)m_mouseInputs[button].m_state == action)
	{
		m_mouseInputs[button].m_delegate();
	}
}

void InputManager::MousePosCallback(double xpos, double ypos)
{
	if(m_bMousePoscallback)
	{
		m_mousePosCallback(xpos, ypos);
	}
	//std::cout << "Mouse Positions X: " << xpos << " Y: " << ypos << std::endl;
}


void InputManager::SaveKey(std::string_view userSTR, int defineKey)
{
	m_settingsKey[HashString::Hash(userSTR.data())] = defineKey;
}

void InputManager::SaveMouseKey(std::string_view userSTR, int defineMouseKey)
{
	m_settingsMouse[HashString::Hash(userSTR.data())] = defineMouseKey;
}
