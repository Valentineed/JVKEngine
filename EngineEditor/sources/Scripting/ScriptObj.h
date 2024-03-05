#pragma once
#include <sol/sol.hpp>


class ScriptObj
{
public:
    ScriptObj() = default;
    //ScriptObj(sol::state& pLuaState, const std::string& luaClassName, const std::string& luaScriptName);
    void CallFunction(const std::string& fnName);
    ~ScriptObj();
protected:
    std::string m_scriptVarName; // The name of the lua object in the lua state
    sol::table m_luaObjectData;  // A variable holding the data of the created lua object
    bool m_initialized = false;          // whether the scriptObject is initialized
    size_t m_entityIDAssociated; // ID of actor associated to the script
};
