#pragma once

#include "GLFW/glfw3.h"

class Window
{
public:
	Window();
	~Window();

	inline bool isWindowClosed() const
	{
		return glfwWindowShouldClose(m_window);
	}

	inline auto getWindow() const
	{
		return m_window;
	}
private:


	GLFWwindow* m_window;
};
