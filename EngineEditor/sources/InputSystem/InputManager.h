#pragma once
#include <unordered_map>
#include "Resources/HashString.h"
#include <functional>
#include <sstream> 

#include "ToolsEngine.h"
class Window;
class EngineManager;

//template <typename>
//class  Delegate;
//
//template <typename rType, typename... Args>
//class  Delegate<rType(Args&&...)>
//{
//public:
//    template<typename pClass>
//    void Connect(pClass* t, rType(pClass::* method)(Args&&...))
//    {
//        m_function = [=](Args&& ... as) { (t->*method)(std::forward<Args>(as)...); };
//    }
//
//    rType operator() (Args&&... args)
//    {
//        return m_function(std::forward<Args>(args)...);
//    }
//
//protected:
//    std::function<rType(Args&&...)> m_function;
//};

enum class KeyEvent
{
    RELEASE = 0,
    PRESS = 1,
    REPEAT = 3,
	NO_ACTION
};

class InputManager
{
public:
    static InputManager& GetInstance();
	
    void Init(Window* window);
    void ProcessInput();
    void Clear();
    void SaveKey(std::string_view userSTR, int defineKey);

    template<typename pClass>
    void BindKey(std::string_view userSTR, KeyEvent event, pClass* pclass, void(pClass::* method)());

    void SaveMouseKey(std::string_view userSTR, int defineMouseKey);
	
    template<typename pClass>
    void BindMouseKey(std::string_view userSTR, KeyEvent event, pClass* pclass, void(pClass::* method)());

    template<class pClass>
    inline void SetMousePosCallback(pClass* t, void(pClass::* method)(double, double));
private:
    /*Singleton*/
    InputManager() = default;
    ~InputManager() = default;
    InputManager(InputManager const&) = delete;
    InputManager& operator=(InputManager const&) = delete;
	
    std::vector<int> m_keyInRepeat;
    std::vector<int> m_mouseButtonInRepeat;
    void KeyCallback(int key, int scancode, int action, int mods);
    void MouseButtonCallback(int button, int action, int mods);
    void MousePosCallback(double xpos, double ypos);
	
	/*move_forward, key_W*/
    std::unordered_map<size_t, int>m_settingsKey;
    std::unordered_map<size_t, int>m_settingsMouse;
    //std::unordered_map<int, int>m_settingsJoystick;
	
    struct InputMapping
    {
        KeyEvent m_state = KeyEvent::NO_ACTION;
        Delegate<void()> m_delegate;
    };

	/*MousePossCallback*/
    bool m_bMousePoscallback = false;
   // Delegate<void(double placeholder::, double)> m_mousePosCallback;
#pragma warning(push)
#pragma warning(disable: 4251)//Todo use my custom delegate
    std::function<void(double, double)>m_mousePosCallback;
#pragma warning(pop)
	/*key,*/
    std::unordered_map<size_t, InputMapping> m_keyInputs;
    std::unordered_map<size_t, InputMapping> m_mouseInputs;
    std::unordered_map<size_t, std::vector<InputMapping>> m_joystickInputs;
};

template<typename pClass>
inline void InputManager::BindKey(std::string_view userSTR, KeyEvent event, pClass* pclass, void(pClass::* method)())
{
    const size_t key = m_settingsKey[HashString::Hash(userSTR.data())];
    m_keyInputs[key].m_state = event;
    m_keyInputs[key].m_delegate.Connect(pclass, method);
}

template<typename pClass>
inline void InputManager::BindMouseKey(std::string_view userSTR, KeyEvent event, pClass* pclass, void(pClass::* method)())
{
    const size_t key = m_settingsMouse[HashString::Hash(userSTR.data())];
    m_mouseInputs[key].m_state = event;
    m_mouseInputs[key].m_delegate.Connect(pclass, method);
}

template<class pClass>
inline void InputManager::SetMousePosCallback(pClass* t, void(pClass::* method)(double, double))
{
    using namespace std::placeholders;
    m_mousePosCallback = std::bind(method, t, _1, _2);
    m_bMousePoscallback = true;
}
