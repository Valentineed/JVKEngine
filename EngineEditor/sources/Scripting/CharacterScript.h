#pragma once
#include "ScriptObj.h"
#include "../Scene/Actor.h"

enum class KeyEvent;

class CharacterScript : public ScriptObj
{
public:
	CharacterScript() = default;
	CharacterScript(sol::state& pLuaState, const std::string& luaClassName, const std::string& luaScriptName, size_t entityID);

	void BindInputKey(std::string& action, KeyEvent pressType, int key);

	void LinkFunctions(sol::state& pLuaState);
	
	void LinkVariables(sol::state& pLuaState);

	void InitCallBackFunctions();


protected:
	std::unordered_map<std::string, void (Actor::*)()> m_mapFunctions;
};

