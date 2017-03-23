#pragma once

#include <vulkan/vulkan.h>

#include "Window.h"

class Instance
{
public:
	Instance(const Window& window);

	~Instance();


	inline auto getInstance() const
	{
		return m_instance;
	}

	inline auto getSurface() const
	{
		return m_surface;
	}

private:

	VkInstance m_instance;
	VkDebugReportCallbackEXT m_callback;
	VkSurfaceKHR m_surface;
	const Window& m_window;
};
