#pragma once
#include <cstdint>
#include <functional>

#include "libgfx_Export.h"
struct SurfaceDescriptor;
typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWcursor GLFWcursor;

typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);
class libgfx_API Window
{
public:
	Window() = delete;
	Window(const Window& other) = delete;
	Window& operator=(const Window& other) = delete;
	Window(int width, int height, const char* windowName);
	~Window();
	
	bool IsClosed();
	void CloseWindow();
	void SetWindowShouldClose();
	void InitializeWindowCallback();
	const char** GetRequiredExtensions(uint32_t* extensionCount);
	void* GetUserPointer();
	void GetFramebufferSize(int* width, int* height);
	void SetCursorMode(bool isEnable);
	void ActiveCrossHair(bool isEnable);
	void SetCreateCustomCursor(const char* path);
	void ResetAparenceCursor();
	void SetWindowIcon(const char* path);
	void WaitEvents();
	void PollEvents();
	bool GetKeyEvent(int key, int event);

	template<class pClass>
	void SetFramebufferSizeCallback(pClass* t, void(pClass::* method)(int, int));
	template<class pClass>
	void SetKeyCallback(pClass* t, void(pClass::* method)(int, int, int, int));
	template<class pClass>
	void SetMousePosCallback(pClass* t, void(pClass::* method)(double, double));
	template<class pClass>
	void SetMouseButtonCallback(pClass* t, void(pClass::* method)(int, int, int));
	template<class pClass>
	void SetScrollCallback(pClass* t, void(pClass::* method)(double, double));
	template<class pClass>
	void SetDropCallback(pClass* t, void(pClass::* method)(const char**));

	int CreateSurface(SurfaceDescriptor* sDescriptor);
	//It returns the number of seconds since the library was initialized.
	double GetTime();
	//This sets the time to the specified time, in seconds, and it continues to count from there.
	void SetTime(double seconds);
	//uint64_t fréquence = glfwGetTimerFrequency();
	//uint64_t valeur = glfwGetTimerValue ();

	void InitCallbackKey()const;
	void InitCallbackMousePos()const;
	void InitCallbackMouseButton()const;
	void InitCallbackScroll()const;
	void InitCallbackDrop()const;
	void InitCallbackFramebufferCallback()const;
	//Todo
#pragma warning(push)
#pragma warning(disable: 4251)
	std::function<void(int, int, int, int)> m_callbackKeyFunction;
	std::function<void(double, double)>m_callbackMousePosFunction;
	std::function<void(int, int, int)>m_callbackMouseButtonFunction;
	std::function<void(double, double)>m_callbackScrollFunction;
	std::function<void(const char**)>m_callbackDrop;
	std::function<void(int, int)> m_callbackFrameBufferSize;
#pragma warning(pop)


	GLFWwindow* GetWindow() { return m_window; }
private:
	int m_width;
	int m_height;
	const char* m_title;
	GLFWwindow* m_window = nullptr;
	GLFWcursor* m_cursor = nullptr;

};

template<class pClass>
inline void Window::SetFramebufferSizeCallback(pClass* t, void(pClass::* method)(int, int))
{
	using namespace std::placeholders;
	m_callbackFrameBufferSize = std::bind(method, t, _1, _2);
	InitCallbackFramebufferCallback();
}

template<class pClass>
inline void Window::SetKeyCallback(pClass* t, void(pClass::* method)(int, int, int, int))
{
	using namespace std::placeholders;
	m_callbackKeyFunction = std::bind( method,t,_1,_2,_3,_4);
	InitCallbackKey();
}

template<class pClass>
inline void Window::SetMousePosCallback(pClass* t, void(pClass::* method)(double, double))
{
	using namespace std::placeholders;
	m_callbackMousePosFunction = std::bind(method, t, _1, _2);
	InitCallbackMousePos();
}

template<class pClass>
inline void Window::SetMouseButtonCallback(pClass* t, void(pClass::* method)(int, int, int))
{
	using namespace std::placeholders;
	m_callbackMouseButtonFunction = std::bind(method, t, _1, _2,_3);
	InitCallbackMouseButton();
}

template<class pClass>
inline void Window::SetScrollCallback(pClass* t, void(pClass::* method)(double, double))
{
	using namespace std::placeholders;
	m_callbackScrollFunction = std::bind(method, t, _1, _2);
	InitCallbackScroll();
}

template<class pClass>
inline void Window::SetDropCallback(pClass* t, void(pClass::* method)(const char**))
{
	using namespace std::placeholders;
	m_callbackDrop = std::bind(method, t, _1);
	InitCallbackDrop();
}
