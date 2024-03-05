#include "ActorScriptCompData.h"

std::vector<std::string>& ActorScriptCompData::GetScriptsPath()
{
	return m_scriptsPaths;
}

void ActorScriptCompData::SetAllPaths(std::vector<std::string>& paths)
{
	m_scriptsPaths = paths;
}
