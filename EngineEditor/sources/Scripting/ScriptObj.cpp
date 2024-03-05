#include "ScriptObj.h"

//ScriptObj::ScriptObj(sol::state& pLuaState, const std::string& luaClassName, const std::string& luaScriptName) : /*m_pLuaState(pLuaState),*/ m_initialized(false), m_scriptVarName(luaScriptName)
//{
//    /*if (m_pLuaState)
//    {*/
//        m_luaObjectData = (pLuaState)[luaScriptName];
//
//    	if (m_luaObjectData.valid())
//    	{
//            m_initialized = true;
//    	}
//    //}
//}

void ScriptObj::CallFunction(const std::string& fnName)
{
    if (m_initialized)
    {
        m_luaObjectData[fnName]();
    } 
}

ScriptObj::~ScriptObj()
{
	if (m_initialized)
	{
		m_luaObjectData.clear();
	}
	
	delete this;
}
