#include "Window.h"
#include <iostream>
#include "SurfaceDescriptor.h"
#define GLFW_INCLUDE_VULKAN//glfwCreateWindowSurface()
#include <GLFW/glfw3.h>

#include "Resources/TextureLoader.h"

void framebufferSize_callback(GLFWwindow* window, int xpos, int ypos);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void drop_callback(GLFWwindow* window, int count, const char** paths);

Window::Window(int width, int height, const char* windowName)
	: m_width(width), m_height(height), m_title(windowName)
{
	/* Initialize the library */
	if (!glfwInit())
	{
		//Todo Debug
		std::cerr << "Fail to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, true);
	//-------------- glfw window creation / option generate fullscreen ---------------//
	//m_window = glfwCreateWindow(m_width, m_height, m_title, glfwGetPrimaryMonitor(), nullptr);  glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);


	m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	if (!m_window)
	{
		//Todo Debug
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-EXIT_FAILURE);
	}
	glfwSetWindowUserPointer(m_window, this);
	glfwGetWindowSize(m_window, &m_width, &m_height);
	//glfwMakeContextCurrent(m_window);
	//glfwSetGamma(m_window, 0.1f);
}

Window::~Window()
{
	glfwTerminate();
	glfwDestroyWindow(m_window);
}

bool Window::IsClosed()
{
	return glfwWindowShouldClose(m_window);
}

void Window::CloseWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::SetWindowShouldClose()
{
	glfwSetWindowShouldClose(m_window, true);
}

void Window::InitializeWindowCallback()
{
	//If you wish to be notified when the cursor enters or leaves the content area of a window 
	glfwSetCursorEnterCallback(m_window, cursor_enter_callback);
}

const char** Window::GetRequiredExtensions(uint32_t* extensionCount)
{
	const char** extensions = glfwGetRequiredInstanceExtensions(extensionCount);
	return extensions;
}

void* Window::GetUserPointer()
{
	//return glfwGetWindowUserPointer(m_window);
	return this;
}

void Window::GetFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(m_window, width, height);
}

void Window::SetCursorMode(bool isEnable)
{
	
	if(isEnable)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CENTER_CURSOR);
	}
}

void Window::ActiveCrossHair(bool isEnable)
{
	if(isEnable)
	{
		m_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(m_window, m_cursor);
	}
	else
	{
		ResetAparenceCursor();
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::SetCreateCustomCursor(const char* path)
{
	GLFWimage icons;
	TemporaryTexture iconLoad(path, &icons.width, &icons.height, 0, 4);
	GLFWcursor* cursor = glfwCreateCursor(&icons, 0, 0);
	if(cursor == nullptr)
	{
		std::cerr << "Failed to load Window Icon" << std::endl;
		//todo throw?
	}
	glfwSetCursor(m_window, cursor);
}

void Window::ResetAparenceCursor()
{
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	if (cursor == nullptr)
	{
		std::cerr << "Failed to load Window Icon" << std::endl;
		//todo throw?
	}
	glfwSetCursor(m_window, cursor);
}

void Window::SetWindowIcon(const char* path)
{
	GLFWimage icons;
	TemporaryTexture iconLoad(path, &icons.width, &icons.height, 0, 4);
	icons.pixels = iconLoad.pixels;
	if (icons.pixels)
	{
		glfwSetWindowIcon(m_window, 1, &icons);
	}
	else
	{
		std::cerr << "Failed to load Window Icon" << std::endl;
	}
}

void Window::WaitEvents()
{
	glfwWaitEvents();
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::GetKeyEvent(int key, int event)
{
	if(glfwGetKey(m_window,key) == event)
	{
		return true;
	}
	return false;
}

int Window::CreateSurface(SurfaceDescriptor* sDescriptor)
{
	return (int)glfwCreateWindowSurface(sDescriptor->instance, m_window, sDescriptor->allocator, sDescriptor->surface);
}

double Window::GetTime()
{
	return glfwGetTime();
}

void Window::InitCallbackKey() const
{
	//callback for the inputs
	glfwSetKeyCallback(m_window, key_callback);
}

void Window::InitCallbackMousePos() const
{
	//callback for the mouse
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
}

void Window::InitCallbackMouseButton() const
{
	//If you wish to be notified when a mouse button is pressed or released, set a mouse button callback.
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

void Window::InitCallbackScroll() const
{
	//callback for the scroll
	glfwSetScrollCallback(m_window, scroll_callback);
}

void Window::InitCallbackDrop() const
{
	//If you wish to receive the paths of files and/or directories dropped on a window, set a file drop callback
	glfwSetDropCallback(m_window, drop_callback);
}

void Window::InitCallbackFramebufferCallback() const
{
	glfwSetFramebufferSizeCallback(m_window, framebufferSize_callback);
}

void framebufferSize_callback(GLFWwindow* window, int xpos, int ypos)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackFrameBufferSize(xpos, ypos);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackMousePosFunction(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackScrollFunction(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackKeyFunction(key, scancode, action, mods);
}

void cursor_enter_callback(GLFWwindow* /*window*/, int entered)
{
	if (entered)
	{
		// The cursor entered the content area of the window
	}
	else
	{
		// The cursor left the content area of the window
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackMouseButtonFunction(button, action, mods);
}

void drop_callback(GLFWwindow* window, int /*count*/, const char** paths)
{
	Window* myWindow = (Window*)glfwGetWindowUserPointer(window);
	myWindow->m_callbackDrop(paths);
	/*int i;
	for (i = 0; i < count; i++)
		handle_dropped_file(paths[i]);*/
}