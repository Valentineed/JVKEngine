#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <sstream> 

#include "ToolsEngine_Export.h"

class ToolsEngine_API ToolsEngine
{
public:
    static void TestMessageToolsEngine();

    //Use the OPENFILENAMEA function from windows to get the selected file path
    static const std::string OpenFileName();

    static void CreateScriptCharacter(std::string& scriptPath, std::string& scriptName);

    static void ReadBatchFile(std::string_view fileName);

    static const std::string ChoseFolderPath();

    static void RewriteBatShippingFile(std::string& dirPath);

    static void CreateNewLevel(std::string& levelName);
private:
};


template <typename>
class Delegate;

template <typename rType, typename... Args>
class Delegate<rType(Args &&...)>
{
public:
    template<typename pClass>
    void Connect(pClass* t, rType(pClass::* method)(Args&&...))
    {
        m_function = [=](Args&& ... as) { (t->*method)(std::forward<Args>(as)...); };
    }

    rType operator() (Args&&... args)
    {
        return m_function(std::forward<Args>(args)...);
    }

protected:
    std::function<rType(Args &&...)> m_function;
};