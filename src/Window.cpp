#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(800, 600, "Vk test", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

