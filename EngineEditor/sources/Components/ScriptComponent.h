#pragma once

#include "../Scripting/ScriptObj.h"
#include "sol/sol.hpp"

enum class ScriptType
{
	NONE,
	CHARACTER,
	UI,
	
};

class ScriptComponent
{
public:
	ScriptComponent() = default;
	ScriptComponent(size_t entityID, const std::string& path);
	void InitScriptComponent();
	const std::string& GetPath();
	void ReloadLuaScript(const std::string& path);

	void Start();
	void Update();

	size_t GetEntityID();

	ScriptType FindParentType();
private:
	sol::state m_luaState;
	
	sol::table m_luaObj;
	std::string m_scriptPath;
	std::string m_scriptName;
	size_t m_entityID;

	
	ScriptObj* m_scriptObjectType;

	
};
