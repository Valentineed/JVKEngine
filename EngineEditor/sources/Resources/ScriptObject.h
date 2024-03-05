#include "sol/sol.hpp"

class ScriptObject
{
public:
    explicit ScriptObject(sol::state* pLuaState, const std::string& luaClassName, const std::string& luaScriptName)
        :
        m_pLuaState(pLuaState),
        m_initialized(false)
    {
        if (m_pLuaState)
        {
            static std::size_t scriptNum = 0;
            m_scriptVarName = luaScriptName + "_" + std::to_string(scriptNum++);
            bool isValidCreation = true;
            std::string luaScript = m_scriptVarName + " = " + luaClassName + ":new()";
            m_pLuaState->script(luaScript,
                [&isValidCreation](lua_State* state, sol::protected_function_result res) { isValidCreation = false; return res; });
            if (isValidCreation)
                m_luaObjectData = (*m_pLuaState)[m_scriptVarName];
            m_initialized = isValidCreation && m_luaObjectData.valid();
        }
    }
    void CallFunction(const std::string& fnName)
    {
        if (m_initialized)
            m_luaObjectData[fnName]();
    }
    ~ScriptObject()
    {
        if (m_initialized && m_pLuaState)
            m_pLuaState->do_string(m_scriptVarName + " = nil");
    }
private:
    std::string m_scriptVarName; // The name of the lua object in the lua state
    sol::table m_luaObjectData;  // A variable holding the data of the created lua object
    sol::state* m_pLuaState;     // The lua state the lua object is created
    bool m_initialized;          // whether the scriptObject is initialized
};