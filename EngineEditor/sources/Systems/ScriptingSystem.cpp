#include "ScriptingSystem.h"

#include <filesystem>



#include "Defines.h"
#include "Descriptors.h"
#include "../Components/ScriptComponent.h"
#include "../ECS/World.h"

void ScriptingSystem::Start()
{
	//sol::state lua_state;

	//lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);

	//std::string package_path = lua_state["package"]["path"];
	//lua_state["package"]["path"] = (package_path + ";" + std::filesystem::current_path().string() + "/resources/scripts/middleclass.lua").c_str();

	//std::string path = std::filesystem::current_path().string();
	//path += "/resources/scripts/Core.lua";
	//lua_state.script_file(path);
}

void ScriptingSystem::Update(Actor* actor)
{
	/*auto childrens = actor->m_children;

	if (childrens.empty() == false)
	{
		for (Actor* dactor : childrens)
		{
			if (World::GetInstance().GetComponent<ScriptComponent>(dactor->m_idEntity) != nullptr)
			{
				World::GetInstance().GetComponent<ScriptComponent>(dactor->m_idEntity)->Update();
			}

			Update(dactor);
		}
	}*/
}

void ScriptingSystem::Register(Entity entity, const std::string& string)
{
	m_scripts.emplace_back(entity, string);
}



std::vector<std::string> ScriptingSystem::GetAllScriptsPath(size_t entityID)
{
	std::vector<std::string> tempVec;

	auto it = FindSpecificScript(entityID, 0);

	while (it != World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts.end())
	{
		auto index = std::distance(World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts.begin(), it);

		if (it != World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts.end())
		{
			std::filesystem::path pathName = it->GetPath();
			tempVec.push_back(pathName.filename().string());
		}
		it = FindSpecificScript(entityID, index + 1);
	}

	return tempVec;
}

std::vector<ScriptComponent>::iterator ScriptingSystem::FindSpecificScript(size_t entityID, int indexfound)
{
	auto& Scripts = World::GetInstance().GetSystem<ScriptingSystem>()->m_scripts;
	return std::find_if(Scripts.begin() + indexfound, Scripts.end(), [entityID](ScriptComponent& obj) {return obj.GetEntityID() == entityID; });
}

