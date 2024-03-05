#pragma once

#include "../ECS/System.h"
#include "../Scene/Actor.h"
class ScriptingSystem : public System
{
public:
	void Start();
	void Update(Actor* actor);

	void Register(Entity entity, const std::string& string);

	std::vector<std::string> GetAllScriptsPath(size_t entityID);

	std::vector<ScriptComponent>::iterator FindSpecificScript(size_t entityID, int indexfound);
	std::vector<ScriptComponent> m_scripts;
	
private:
	
};

