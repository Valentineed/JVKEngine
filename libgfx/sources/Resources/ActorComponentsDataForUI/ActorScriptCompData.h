#pragma once
#include <libgfx_Export.h>
#include <string>
#include <vector>

class ActorScriptCompData
{
public:
	ActorScriptCompData() = default;
	
	libgfx_API std::vector<std::string>& GetScriptsPath();

	void libgfx_API SetAllPaths(std::vector<std::string>& paths);
	
private:
	std::vector<std::string> m_scriptsPaths;
};

