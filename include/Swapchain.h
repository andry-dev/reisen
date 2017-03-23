#pragma once

#include <array>

#include <vulkan/vulkan.h>

class Instance;
class Device;

class Swapchain
{
public:
	Swapchain();
	~Swapchain() = default;

	Swapchain(const Swapchain& rhs) = default;
	Swapchain& operator=(const Swapchain& rhs) = default;

	Swapchain(Swapchain&& rhs) = default;
	Swapchain& operator=(Swapchain&& rhs) = default;

	
private:
	
};

