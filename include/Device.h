#pragma once

#include <vulkan/vulkan.h>
#include "asl/types"
#include <array>


[[maybe_unused]] constexpr std::array<const char*, 1> g_deviceExts =
{{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
}};


class Instance;

struct QueueFamilyIndices
{
	asl::mut_i32 graphics = -1;
	asl::mut_i32 present = -1;

	bool isComplete() noexcept
	{
		return graphics >= 0
			&& present >= 0;
	}
};

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& dev,
									 const VkSurfaceKHR& surface);

class Device
{
public:
	explicit Device(Instance& instance);
	~Device();

	inline auto getLogicalDevice() const noexcept
	{
		return m_device;
	}


	Device(const Device& rhs) = default;
	Device& operator=(const Device& rhs) = default;

	Device(Device&& rhs) = default;
	Device& operator=(Device&& rhs) = default;


private:
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	Instance& m_instance;
	QueueFamilyIndices m_indices;
	VkQueue m_queue;
};

bool checkDeviceExtensionSupport(VkPhysicalDevice& dev) noexcept;

bool isDeviceSuitable(const VkPhysicalDevice& dev, const VkSurfaceKHR& surface) noexcept;
