#include "CharacterScript.h"

#include <iostream>

#include "../InputSystem/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../TestGameplay/Trampoline.h"

#include "../../libgfx/sources/Resources/HashString.h"

CharacterScript::CharacterScript(sol::state& pLuaState, const std::string& luaClassName, const std::string& luaScriptName, size_t entityID)
{

	InitCallBackFunctions();

	
	LinkFunctions(pLuaState);

	LinkVariables(pLuaState);

	m_luaObjectData = (pLuaState)[luaScriptName];

	if (m_luaObjectData.valid())
	{
		m_initialized = true;
		m_entityIDAssociated = entityID;
		m_scriptVarName = luaScriptName;
	}


}

void CharacterScript::BindInputKey(std::string& action, KeyEvent pressType, int key)
{
	std::cout << "Bind Input Key called from C++" << action << std::endl;

	auto* scene = SceneManager::GetInstance().GetSceneAt(SceneManager::GetInstance().GetCurrentID());

	auto inputManager = &InputManager::GetInstance();
	inputManager->SaveKey(action, key);

	//TODO: Remove static cast when we will only have actors
	//auto* actor = static_cast<Player*>(scene->GetActor(scene->m_root, m_entityIDAssociated));
	auto* actor = scene->GetActor(scene->m_root, m_entityIDAssociated);

	inputManager->BindKey(action, pressType, actor, m_mapFunctions.find(action)->second);
}

void CharacterScript::LinkFunctions(sol::state& pLuaState)
{
	pLuaState.set_function("BindInputKey", &CharacterScript::BindInputKey, this);
}

void CharacterScript::LinkVariables(sol::state& pLuaState)
{
	pLuaState["Keys"] = pLuaState.create_table_with(
		"KEY_A", 65, "KEY_B", 66, "KEY_C", 67, "KEY_D", 68, "KEY_E", 69, "KEY_F", 70, "KEY_G", 71, "KEY_H", 72, "KEY_I", 73, "KEY_J", 74, "KEY_K", 75, "KEY_L", 76, "KEY_M", 77,
		"KEY_N", 78, "KEY_O", 79, "KEY_P", 80, "KEY_Q", 81, "KEY_R", 82, "KEY_S", 83, "KEY_T", 84, "KEY_U", 85, "KEY_V", 86, "KEY_W", 87, "KEY_X", 88, "KEY_Y", 89, "KEY_Z", 90,
		"KEY_SPACE", 32
	);

	pLuaState["KeyPressType"] = pLuaState.create_table_with(
		"RELEASE", 0, "PRESS", 1, "REPEAT", 3
	);
}

void CharacterScript::InitCallBackFunctions()
{
	m_mapFunctions.insert({ "MoveForward", &Actor::CallMoveForward });
	m_mapFunctions.insert({ "MoveBackward", &Actor::CallMoveBackward });
	m_mapFunctions.insert({ "MoveLeft", &Actor::CallMoveLeft });
	m_mapFunctions.insert({ "MoveRight", &Actor::CallMoveRight });
	m_mapFunctions.insert({ "Jump", &Actor::Jump });
}