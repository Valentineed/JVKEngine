#include "ScriptComponent.h"

#include <filesystem>
#include <fstream>

#include <iostream>

#include "../InputSystem/DefineInput.h"
#include "../InputSystem/InputManager.h"
#include "../Scripting/ScriptObj.h"
#include "../Scene/SceneManager.h"
#include "../Scripting/CharacterScript.h"

ScriptComponent::ScriptComponent(size_t entityID, const std::string& path) : m_scriptPath(path), m_entityID(entityID)
{
	std::filesystem::path tempPath = m_scriptPath;

	m_scriptName = tempPath.filename().string();
	m_scriptName = m_scriptName.substr(0, m_scriptName.size() - 4);
	
	m_luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);

	InitScriptComponent();
}

void ScriptComponent::InitScriptComponent()
{

	//"include" middleclass to the lua to be able to use the POO
	std::string package_path = m_luaState["package"]["path"];
	m_luaState["package"]["path"] = (package_path + ";" + std::filesystem::current_path().string() + "/resources/scripts/middleclass.lua").c_str();
	package_path = m_luaState["package"]["path"];

	std::string path = std::filesystem::current_path().string();
	
	switch (FindParentType())
	{
		case ScriptType::CHARACTER:
			std::cout << "Character type script";
		
			path += "/resources/scripts/Character.lua";
			m_luaState.do_file(path);

			m_luaState.script_file(m_scriptPath);
			//m_luaObj = (m_luaState)[m_scriptName];

			//TODO: delete when remove
			m_scriptObjectType = new CharacterScript(m_luaState, "Character", m_scriptName, m_entityID);

			
			break;
		case ScriptType::UI:
			std::cout << "UI type script";
			break;
		case ScriptType::NONE:
			std::cout << "Error adding script, the parent type was not found";
			break;
	}
}

const std::string& ScriptComponent::GetPath()
{
	return m_scriptPath;
}

void ScriptComponent::ReloadLuaScript(const std::string& path)
{
	m_luaState.script_file(path);
}

void ScriptComponent::Start()
{
	m_scriptObjectType->CallFunction("Start");
}

void ScriptComponent::Update()
{
	m_scriptObjectType->CallFunction("Update");
	auto* scene = SceneManager::GetInstance().GetSceneAt(SceneManager::GetInstance().GetCurrentID());
	
}

size_t ScriptComponent::GetEntityID()
{
	return m_entityID;
}



ScriptType ScriptComponent::FindParentType()
{
	std::string line;
	std::ifstream file(m_scriptPath.c_str());

	while (std::getline(file, line)) {
		if (line.find("Character:new()"))
		{
			return ScriptType::CHARACTER;
		}
		if (line.find("UI:new()"))
		{
			return ScriptType::UI;
		}
	}
	return ScriptType::NONE;
}
